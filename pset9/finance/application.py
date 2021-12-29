import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    users = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
    owned_cash = users[0]['cash']

    # Get user currently owned stocks
    summary = db.execute("""SELECT company, symbol, sum(shares) as sum_of_shares
                              FROM transactions
                              WHERE user_id = ?
                              GROUP BY user_id, company, symbol
                              HAVING sum_of_shares > 0;""", session["user_id"])

    # Use lookup API to get the current price for each stock
    summary = [dict(x, **{'price': lookup(x['symbol'])['price']}) for x in summary]

    # Calcuate total price for each stock
    summary = [dict(x, **{'total': x['price']*x['sum_of_shares']}) for x in summary]

    sum_totals = owned_cash + sum([x['total'] for x in summary])

    return render_template("index.html", owned_cash=owned_cash, summary=summary, sum_totals=sum_totals)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        check = lookup(symbol)
        shares = request.form.get("shares")
        if not symbol or not shares:
            return apology("You must enter a valid Symbol and number of Shares to buy!", 403)
        try:
            shares = int(shares)
        except ValueError:
            return apology("INVALID SHARES")

        # Check shares is positive number
        if not (shares > 0):
            return apology("INVALID SHARES")

        # Ensure symbol is valided
        if not (query := lookup(symbol)):
            return apology("INVALID SYMBOL")

        elif symbol and check:
            price = check["price"]
            shares = int(shares)
            total = price*shares
            available = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
            available = available[0]
            available = int(available["cash"])
            if total <= available:
                # update cash
                available += -total
                db.execute("UPDATE users SET cash = :cash WHERE id = :id", id=session["user_id"], cash=available)
                # insert new row to transactions with the stock name etc.
                db.execute("INSERT INTO transactions (user_id, symbol, company, price, shares, operation) VALUES(?,?,?,?,?,?)",
                           session["user_id"], check["symbol"], check["name"], check["price"], shares, "buy")
                flash("Bought!")
            else:
                return apology("You do not have enough funds to buy!", 403)
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?;", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Ensure Symbol is exists
        if not (quoted := lookup(request.form.get("symbol"))):
            return apology("INVALID SYMBOL")

        return render_template("quoted.html", quoted=quoted)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        user_id = request.form.get("username")
        upass = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # Ensure username was submitted
        if not user_id:
            return apology("must provide username", 400)

        # Ensure username does not exist
        check = db.execute("SELECT * FROM users WHERE username = ?", user_id)
        if user_id and check:
            return apology("Username already taken, please choose a different username", 400)

        # Ensure password was submitted
        elif not upass or upass != confirmation:
            return apology("Passwords don't match!", 400)

        else:
            hasher = generate_password_hash(upass, method='pbkdf2:sha256', salt_length=8)
            db.execute("INSERT INTO users (username, hash) VALUES(?,?)", user_id, hasher)
            return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    owned_symbols = db.execute("""SELECT symbol, sum(shares) as sum_of_shares
                                  FROM transactions
                                  WHERE user_id = ?
                                  GROUP BY user_id, symbol
                                  HAVING sum_of_shares > 0;""", session["user_id"])

    if request.method == "POST":
        if not (symbol := request.form.get("symbol")):
            return apology("MISSING SYMBOL")

        if not (shares := request.form.get("shares")):
            return apology("MISSING SHARES")

        # Check share is numeric data type
        try:
            shares = int(shares)
        except ValueError:
            return apology("INVALID SHARES")

        # Check shares is positive number
        if not (shares > 0):
            return apology("INVALID SHARES")

        symbols_dict = {d['symbol']: d['sum_of_shares'] for d in owned_symbols}

        if symbols_dict[symbol] < shares:
            return apology("TOO MANY SHARES")

        query = lookup(symbol)

        # Get user currently owned cash
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Execute a transaction
        db.execute("INSERT INTO transactions(user_id, company, symbol, shares, price) VALUES(?, ?, ?, ?, ?);",
                   session["user_id"], query["name"], symbol, -shares, query["price"])

        # Update user owned cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?;",
                   (rows[0]['cash'] + (query['price'] * shares)), session["user_id"])

        flash("Sold!")

        return redirect("/")

    else:
        return render_template("sell.html", symbols=owned_symbols)


@app.route("/reset", methods=["GET", "POST"])
@login_required
def reset():
    if request.method == "POST":
        if not (password := request.form.get("password")):
            return apology("MISSING OLD PASSWORD")

        new_pass = request.form.get("new_password")
        new_pass = generate_password_hash(new_pass)
        rows = db.execute("SELECT * FROM users WHERE id = ?;", session["user_id"])
        old_pass = rows[0]["hash"]

        if new_pass == old_pass:
            flash("Password reset failed!")
            return redirect("/reset")

        if not check_password_hash(old_pass, request.form.get("password")):
            return apology("INVALID PASSWORD")

        if not (new_password := request.form.get("new_password")):
            return apology("MISSING NEW PASSWORD")

        if not (confirmation := request.form.get("confirmation")):
            return apology("MISSING CONFIRMATION")

        if new_password != confirmation:
            return apology("PASSWORD NOT MATCH")

        db.execute("UPDATE users set hash = ? WHERE id = ?;", new_pass, session["user_id"])
        flash("Password reset successful!")

        return redirect("/")
    else:
        return render_template("passres.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
