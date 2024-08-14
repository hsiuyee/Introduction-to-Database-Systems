SELECT champion_name, 
       SUM(win)/(SUM(win)+SUM(1-win)) AS win_ratio, 
       SUM(goldearned)/(SUM(duration/60)) AS ave_goldearned,
       (SUM(win)/(SUM(win)+SUM(1-win)))/(SUM(goldearned)/(SUM(duration/60))) AS rate
FROM stat AS s
JOIN participant AS p ON p.player_id = s.player_id
JOIN match_info AS m ON m.match_id = p.match_id
JOIN champ AS c ON c.champion_id=p.champion_id
GROUP BY champion_name
ORDER BY rate DESC
LIMIT 30;