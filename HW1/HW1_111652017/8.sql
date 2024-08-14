SELECT champion_name
FROM champ
WHERE champion_id NOT IN (
    SELECT champion_id
    FROM teamban
    WHERE teamban.match_id IN (
        SELECT match_id
        FROM match_info
        WHERE match_info.version LIKE '7.7.%'
    )
);