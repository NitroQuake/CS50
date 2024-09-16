-- Keep a log of any SQL queries you execute as you solve the mystery.
--SELECT descrption FROM crime_scene_reports WHERE month = 7 and day = 28 AND street = 'Humphrey Street';
--Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

--SELECT transcript FROM interviews, name WHERE year = 2023 AND month = 7 AND day = 28;
--Ruth said Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
--Eugene said I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
--Raymond said As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

--SELECT license_plate FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 25;
--+---------------+
--| license_plate |
--+---------------+
--| 5P2BI95       |
--| 94KL13X       |
--| 6P58WS2       |
--| 4328GD8       |
--| G412CB7       |
--| L93JTIZ       |
--| 322W7JE       |
--| 0NTHK55       |
--+---------------+

--SELECT account_number, atm_location, transaction_type, amount FROM atm_transactions WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
--+----------------+----------------+------------------+--------+
--| account_number |  atm_location  | transaction_type | amount |
--+----------------+----------------+------------------+--------+
--| 28500762       | Leggett Street | withdraw         | 48     |
--| 28296815       | Leggett Street | withdraw         | 20     |
--| 76054385       | Leggett Street | withdraw         | 60     |
--| 49610011       | Leggett Street | withdraw         | 50     |
--| 16153065       | Leggett Street | withdraw         | 80     |
--| 25506511       | Leggett Street | withdraw         | 20     |
--| 81061156       | Leggett Street | withdraw         | 30     |
--| 26013199       | Leggett Street | withdraw         | 35     |
--+----------------+----------------+------------------+--------+

--SELECT caller, receiver, duration FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60;
--+----------------+----------------+----------+
--|     caller     |    receiver    | duration |
--+----------------+----------------+----------+
--| (130) 555-0289 | (996) 555-8899 | 51       |
--| (499) 555-9472 | (892) 555-8872 | 36       |
--| (367) 555-5533 | (375) 555-8161 | 45       |
--| (499) 555-9472 | (717) 555-1342 | 50       |
--| (286) 555-6063 | (676) 555-6554 | 43       |
--| (770) 555-1861 | (725) 555-3243 | 49       |
--| (031) 555-6622 | (910) 555-3251 | 38       |
--| (826) 555-1652 | (066) 555-9701 | 55       |
--| (338) 555-6650 | (704) 555-2131 | 54       |
--+----------------+----------------+----------+

--SELECT name, passport_number FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WH
--ERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 25);
--+--------+-----------------+
--|  name  | passport_number |
--+--------+-----------------+
--| Sofia  | 1695452385      |
--| Diana  | 3592750733      |
--| Kelsey | 8294398571      |
--| Bruce  | 5773159633      |
--+--------+-----------------+

--SELECT abbreviation, full_nam FROM airports WHERE city = 'Fiftyville';
--+--------------+-----------------------------+
--| abbreviation |          full_name          |
--+--------------+-----------------------------+
--| CSF          | Fiftyville Regional Airport |
--+--------------+-----------------------------+

--SELECT * FROM flights WHERE origin_airport_id IN (SELECT id FROM airports) AND year = 2023 AND month = 7 AND day = 29 ORDER BY hour;
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
--| 43 | 8                 | 1                      | 2023 | 7     | 29  | 9    | 30     |
--| 23 | 8                 | 11                     | 2023 | 7     | 29  | 12   | 15     |
--| 53 | 8                 | 9                      | 2023 | 7     | 29  | 15   | 20     |
--| 18 | 8                 | 6                      | 2023 | 7     | 29  | 16   | 0      |
--+----+-------------------+------------------------+------+-------+-----+------+--------+

--SELECT city FROM airports WHERE id IN (SELECT destination_airport_id FROM flights WHERE origin_airport_id IN (SELECT id FROM airports) AND year = 2023 AND month = 7 AND day = 29 ORDER BY hour LIMIT 1);
--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+

--SELECT * FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE origin_airport_id IN (SELECT id FROM airports) AND year = 2023 AND month = 7 AND day = 29 ORDER BY hour LIMIT 1);
--+-----------+-----------------+------+
--| flight_id | passport_number | seat |
--+-----------+-----------------+------+
--| 36        | 7214083635      | 2A   |
--| 36        | 1695452385      | 3B   |
--| 36        | 5773159633      | 4A   |
--| 36        | 1540955065      | 5C   |
--| 36        | 8294398571      | 6C   |
--| 36        | 1988161715      | 6D   |
--| 36        | 9878712108      | 7A   |
--| 36        | 8496433585      | 7B   |
--+-----------+-----------------+------+

--SELECT * FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 25) AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE origin_airport_id IN (SELECT id FROM airports) AND year = 2023 AND month = 7 AND day = 29 ORDER BY hour LIMIT 1));
--+--------+--------+----------------+-----------------+---------------+
--|   id   |  name  |  phone_number  | passport_number | license_plate |
--+--------+--------+----------------+-----------------+---------------+
--| 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
--| 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
--| 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |

--SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number amount FROM atm_transactions WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');
--+-----------+
--| person_id |
--+-----------+
--| 686048    | <- Bruce
--| 514354    |
--| 458378    |
--| 395717    |
--| 396669    |
--| 467400    |
--| 449774    |
--| 438727    |
--+-----------+

--SELECT name FROM people WHERE phone_number = '(375) 555-8161';
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+