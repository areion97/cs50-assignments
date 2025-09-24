-- Keep a log of any SQL queries you execute as you solve the mystery.

-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.


-- With the query below I wanted to investigate for the description of the event and I found out that
-- theft took place at 10:15am and there are three witnesses
SELECT * FROM crime_scene_reports
WHERE
        street = 'Humphrey Street'
    AND year = 2021
    AND month = 7
    AND day = 28;



-- I look up for the transcript of the witnesses. IDs: 161, 162, 163
SELECT * FROM interviews
WHERE
        year = 2021
    AND month = 7
    AND day = 28;


-- Here are the most important infos:
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- The witness saw the thief at ATM on Leggett Street withdrawing money
-- Phone call: they were planning to take the earliest flight out of Fiftyville the day after


-- With this query I want to find out the people that
-- went at the ATM of Leggett Street on July 28, 2021
-- and exited from the bakery parking lot on July 28, 2021 around 10-11 am
-- and on July 29, 2021 will take a flight from Fiftyville

-- in particular I need the phone number for the next query

SELECT * FROM people
WHERE
    license_plate IN (
        SELECT DISTINCT a.license_plate FROM bakery_security_logs as a
        JOIN
        (
        SELECT * FROM bakery_security_logs

        ) as b

        ON a.license_plate = b.license_plate

        WHERE
        a.year = 2021 AND a.month = 7 AND a.day <= 28 AND a.activity = 'entrance'
        AND
        b.year = 2021 AND b.month = 7 AND b.day = 28 AND b.hour >= 10 AND b.hour <= 11 AND b.minute <= 45 AND b.activity = 'exit'

    )

    AND

    id IN (
        SELECT
            person_id
        FROM
            bank_accounts
        WHERE
            account_number IN (
                SELECT
                    account_number
                FROM
                    atm_transactions
                WHERE
                        year = 2021
                    AND month = 7
                    AND day = 28
                    AND atm_location = 'Leggett Street'
                    AND transaction_type = 'withdraw'
            )
    )
AND
    passport_number IN (
        SELECT
            passport_number
        FROM
            passengers
        WHERE
            flight_id IN (
                SELECT id FROM flights
                WHERE
                    year = 2021 AND month = 7 AND day = 29
                AND
                    origin_airport_id IN (
                        SELECT id FROM airports WHERE city = 'Fiftyville'
                    )
            )
    );



-- I need all phone numbers that called eachother on July 28, 2021 between those of the previous query
-- to find out who is going to be on the same flight on July 29, 2021 from Fiftyville


SELECT DISTINCT caller as phones FROM phone_calls
                WHERE
                    year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration <= 60
                    AND (
                        caller IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(770) 555-1861',
                            '(367) 555-5533'
                        )
                        OR
                        receiver IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(770) 555-1861',
                            '(367) 555-5533'
                        )
                    )

                UNION
                SELECT DISTINCT receiver as phones FROM phone_calls
                WHERE
                    year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration <= 60
                    AND (
                        caller IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(770) 555-1861',
                            '(367) 555-5533'
                        )
                        OR
                        receiver IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(770) 555-1861',
                            '(367) 555-5533'
                        )
                    );




-- Find people who are on the same flight on July 29, 2021 from Fiftyville

SELECT passport_number FROM people
WHERE
    passport_number IN (
        SELECT
            passport_number
        FROM
            passengers
        WHERE
            flight_id IN (
                SELECT id FROM flights
                WHERE
                    year = 2021 AND month = 7 AND day = 29
                AND
                    origin_airport_id IN (
                        SELECT id FROM airports WHERE city = 'Fiftyville'
                    )
            )
    )
    AND
        phone_number IN (
            '(286) 555-6063',
            '(367) 555-5533',
            '(375) 555-8161',
            '(389) 555-5198',
            '(609) 555-5876',
            '(676) 555-6554',
            '(725) 555-3243',
            '(770) 555-1861'
          );

-- With the query below we can find out that Taylor, Luca and Bruce are on the same flight
        SELECT
            *
        FROM
            passengers
        WHERE
            flight_id IN (
                SELECT id FROM flights
                WHERE
                    year = 2021 AND month = 7 AND day = 29
                AND
                    origin_airport_id IN (
                        SELECT id FROM airports WHERE city = 'Fiftyville'
                    )
            )
            AND
            passport_number IN (
                '1988161715',
                '8496433585',
                '3592750733',
                '5773159633'
            );


-- 1)
-- Check on phone calls again who called eachother

            SELECT DISTINCT *  FROM phone_calls
                WHERE
                    year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration <= 60
                    AND (
                        caller IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(367) 555-5533'
                        )
                        OR
                        receiver IN (
                            '(286) 555-6063',
                            '(389) 555-5198',
                            '(367) 555-5533'
                        )
                    );

-- Merge manually all phone numbers

-- I check again who is on the same flight that day between those numbers
       SELECT
            *
        FROM
            passengers
        WHERE
            flight_id IN (
                SELECT id FROM flights
                WHERE
                    year = 2021 AND month = 7 AND day = 29
                AND
                    origin_airport_id IN (
                        SELECT id FROM airports WHERE city = 'Fiftyville'
                    )
            )
            AND
            passport_number IN (
                SELECT passport_number FROM people WHERE phone_number IN (
                    '(367) 555-5533','(375) 555-8161', '(609) 555-5876', '(389) 555-5198', '(286) 555-6063', '(676) 555-6554'
                )

            );

-- I find out that they are still Taylor, Luca and Bruce who are going to New York City

SELECT * FROM airports WHERE id IN (
    SELECT destination_airport_id FROM flights WHERE id = 36
);



-- I want to find out all the people involved in the calls

SELECT * FROM people WHERE phone_number IN ('(367) 555-5533','(375) 555-8161', '(609) 555-5876', '(389) 555-5198', '(286) 555-6063', '(676) 555-6554');


-- Merging results find out who called who
-- Bruce called Robin
-- Kathryn called Luca
-- Taylor called James





-- Passport must be original and personal, otherwise you can't get onto the plane
-- According to previous research let's filter people who are going to New York from Fiftyville on July 29, 2021

SELECT * FROM people WHERE passport_number IN (
    SELECT
        passport_number
    FROM
        passengers
    WHERE
        flight_id = 36
    );


-- 2 phones      1 car   1 credit card
-- they could be stolen
-- cornercase

-- i telefoni chiamanti dei ladri potrebbero essere stati rubati e poi usati per contattarsi
-- così come la macchina potrebbe essere stata rubata per poi fuggire dal parcheggio del panificio
-- e la carta di credito potrebbe essere rubata per poi prelevare