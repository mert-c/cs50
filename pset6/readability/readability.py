from cs50 import get_string


def main():
    # Get input
    ref = get_string("Input String:")
    # Get info
    info = getinfo(ref)
    # Assing info to variables for the index calculation
    [L, W, S] = info

    L = (L/W)*100
    S = (S/W)*100

    # Calculate Coleman-Liau index rounded to nearest integer
    index = round(0.0588 * L - 0.296 * S - 15.8)
    # Calculate the grade level conditions and Print Grade level
    if(index <= 1):
        print("Before Grade 1")
    elif (index >= 16):
        print("Grade 16+")
    else:
        print("Grade", index)


def getinfo(sent):
    # Initialize the counters
    let = 0
    wor = 1
    sen = 0
    # Loop through the string to fill counters
    for i in range(len(sent)):
        if sent[i].isalpha():
            let = let + 1
        if sent[i].isspace():
            wor = wor + 1
        if ((sent[i] == ".") or (sent[i] == "?") or (sent[i] == "!")):
            sen = sen + 1
    # Return the result as array to be assigned to the index variables
    return [let, wor, sen]


# Recall main function:
if __name__ == "__main__":
    main()
