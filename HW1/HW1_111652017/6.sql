WITH PositionWins AS (
  SELECT
    p.champion_id,
    p.position,
    SUM(s.win) AS total_win,
    RANK() OVER (PARTITION BY p.position ORDER BY SUM(s.win) DESC) AS position_rank
  FROM match_info AS m
  JOIN participant AS p ON m.match_id = p.match_id AND (m.duration BETWEEN 40*60 AND 50*60)
  JOIN stat AS s ON p.player_id = s.player_id
  WHERE p.position IN ('TOP', 'MID', 'JUNGLE', 'DUO_CARRY', 'DUO_SUPPORT')
  GROUP BY p.champion_id, p.position
)
SELECT pw.position, c.champion_name
FROM PositionWins AS pw
JOIN champ AS c ON pw.champion_id = c.champion_id
WHERE position_rank = 1;