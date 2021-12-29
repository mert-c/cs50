-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check the suggested crime scene reports table with the given information:
SELECT * FROM crime_scene_reports
WHERE street = "Chamberlin Street" AND month = 7 AND day = 28;

--Only record returned:
--295 | 2020 | 7 | 28 | Chamberlin Street |
--Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
--Interviews were conducted today with three witnesses who were present at the time —
--each of their interview transcripts mentions the courthouse.

--Following the trail with the interviews:
SELECT * FROM interviews
WHERE month = 7 AND day = 28 AND year = 2020;

--161 | Ruth | 2020 | 7 | 28 |
--Sometime within ten minutes of the theft, I saw the thief get into a car in the
--courthouse parking lot and drive away. If you have security footage from the courthouse parking lot,
--you might want to look for cars that left the parking lot in that time frame.

--162 | Eugene | 2020 | 7 | 28 |
--I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived
--at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.

--163 | Raymond | 2020 | 7 | 28 |
--As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
--In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--The thief then asked the person on the other end of the phone to purchase the flight ticket.


--Checking the logs for the courthouse in the 10 minute frame:
SELECT * FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND minute >15 AND minute < 25;

--license_plate
--5P2BI95
--94KL13X
--6P58WS2
--4328GD8
--G412CB7
--L93JTIZ
--322W7JE
--0NTHK55

--Checking the atm transactions from fifer st.
SELECT * FROM atm_transactions
WHERE atm_location = "Fifer Street" AND year = 2020 AND month = 7 AND day =28 AND transaction_type = "withdraw";
--246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
--264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
--266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
--267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
--269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
--288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
--313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
--336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35

--Checking phone calls during the time frame and duration less than 1 min.
SELECT * FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 and duration < 60;
--221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
--224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
--233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
--251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
--254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
--255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
--261 | (031) 555-6622 | (910) 555-3251 | 2020 | 7 | 28 | 38
--279 | (826) 555-1652 | (066) 555-9701 | 2020 | 7 | 28 | 55
--281 | (338) 555-6650 | (704) 555-2131 | 2020 | 7 | 28 | 54

--Checking the earliest flights 1 day after the theft
SELECT people.name, people.phone_number, people.passport_number, people.license_plate FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.year = 2020 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20 AND flights.origin_airport_id = 8
ORDER BY hour, minute;

--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 7214083635 | 2A
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 1695452385 | 3B
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 5773159633 | 4A
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 1540955065 | 5C
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 8294398571 | 6C
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 1988161715 | 6D
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 9878712108 | 7A
--36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20 | 8 | CSF | Fiftyville Regional Airport | Fiftyville | 36 | 8496433585 | 7B

--Check license plates
SELECT licence_plate FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND minute >15 AND minute < 25 AND license_plate IN (SELECT people.license_plate FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.year = 2020 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20 AND flights.origin_airport_id = 8;


--Combine all of the queries to get the thief:
SELECT license_plate from people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE license_plate IN (SELECT license_plate FROM (SELECT license_plate from people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE people.phone_number IN (SELECT receiver FROM phone_calls) AND year = 2020 AND month = 7 AND day = 28 and duration < 60)
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND year = 2020 AND hour = 10 AND minute >15 AND minute < 25 AND license_plate IN (SELECT people.license_plate FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
JOIN flights ON flights.id = passengers.flight_id
JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.year = 2020 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20 AND flights.origin_airport_id = 8))) AND atm_transactions.transaction_type = "withdraw";

--Use the thief to find the destination airport city
SELECT destination_airport_id from flights
JOIN passengers ON passengers.flight_id = flights.id
JOIN people ON people.passport_number = passengers.passport_number
WHERE people.name = "Ernest" AND flights.year = 2020 AND flights.month = 7 AND flights.day = 29 AND flights.hour = 8 AND flights.minute = 20;

--Use the thief to get the accomplice
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE day = "28" AND month = "7" AND year = "2020" AND duration < "60" AND
    caller = (SELECT phone_number FROM people WHERE name = "Ernest");
