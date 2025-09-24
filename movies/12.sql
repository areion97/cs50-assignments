SELECT b.title FROM
(
SELECT
    title
FROM
    people
    JOIN stars
    ON people.id = stars.person_id
    JOIN movies
    ON stars.movie_id = movies.id
WHERE
    people.name = 'Johnny Depp'
) a
JOIN
(
SELECT
    title
FROM
    people
    JOIN stars
    ON people.id = stars.person_id
    JOIN movies
    ON stars.movie_id = movies.id
WHERE
    people.name = 'Helena Bonham Carter'
) b
ON
a.title = b.title;