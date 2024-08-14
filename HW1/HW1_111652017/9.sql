SELECT
    version,
    SUM(win) AS win_cnt,
    SUM(1 - win) AS lose_cnt,
    SUM(win) / (SUM(win) + SUM(1 - win)) AS win_ratio
FROM (
    SELECT
        c.win,
        SUBSTRING_INDEX(m.version, '.', 2) AS version
    FROM (
        SELECT
            participant.match_id,
            stat.win,
            GROUP_CONCAT(participant.champion_id ORDER BY participant.champion_id ASC) AS top_5_champion_ids
        FROM participant
        JOIN stat ON participant.player_id = stat.player_id
        GROUP BY participant.match_id, stat.win
    ) c
    JOIN match_info m ON c.match_id = m.match_id
    WHERE FIND_IN_SET('17', c.top_5_champion_ids) > 0
      AND FIND_IN_SET('64', c.top_5_champion_ids) > 0
) subquery
GROUP BY version
ORDER BY version;