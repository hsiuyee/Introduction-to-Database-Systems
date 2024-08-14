SELECT
    CASE WHEN win = 1 THEN 'win' ELSE 'lose' END AS win_lose,
    COUNT(*) AS cnt
FROM (
    SELECT
        p.match_id,
        s.win,
        AVG(s.longesttimespentliving) AS average_longesttimespentliving
    FROM
        stat AS s
    JOIN
        participant AS p
        ON p.player_id = s.player_id
    GROUP BY
        p.match_id, s.win
    HAVING
        AVG(s.longesttimespentliving) >= 1200
) AS subquery
GROUP BY win_lose;