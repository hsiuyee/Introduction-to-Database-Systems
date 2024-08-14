WITH PositionKDA AS (
    SELECT p.champion_id, p.position, SUM(s.kills + s.assists) / NULLIF(SUM(s.deaths), 0) AS kda,
    RANK() OVER (PARTITION BY p.position ORDER BY SUM(s.kills + s.assists) / NULLIF(SUM(s.deaths), 0) DESC) AS KDA_rank
    FROM stat AS s
    JOIN participant AS p ON p.player_id = s.player_id
    WHERE p.position IN ('TOP', 'MID', 'JUNGLE', 'DUO_CARRY', 'DUO_SUPPORT')
    GROUP BY p.champion_id, p.position
    HAVING KDA IS NOT NULL
)
SELECT pk.position, c.champion_name, pk.kda
FROM PositionKDA AS pk
JOIN champ AS c ON c.champion_id = pk.champion_id
WHERE KDA_rank = 1;