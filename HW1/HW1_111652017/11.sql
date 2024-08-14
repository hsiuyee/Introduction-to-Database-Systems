SELECT
  CASE
    WHEN p.ss1 > p.ss2 THEN p.ss2
    ELSE p.ss1
  END AS new_ss1,
  CASE
    WHEN p.ss1 > p.ss2 THEN p.ss1
    ELSE p.ss2
  END AS new_ss2,
  SUM(win) / (SUM(1 - win) + SUM(win)) AS win_ratio,
  (SUM(1 - win) + SUM(win)) AS total_times
FROM stat AS s
JOIN participant AS p ON s.player_id = p.player_id
  AND p.position='TOP'
GROUP BY new_ss1, new_ss2
HAVING total_times>100
ORDER BY win_ratio DESC;
