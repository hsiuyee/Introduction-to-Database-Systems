WITH top_player_stats AS (
    SELECT 
        s.win, s.kills, s.deaths, s.assists, s.goldearned,
        p.match_id, p.champion_id
    FROM participant AS p
    JOIN stat AS s ON s.player_id = p.player_id AND p.position = 'TOP'
)

, top_player_comparison AS (
    SELECT DISTINCT p1.match_id,
        p1.win, p1.kills, p1.deaths, p1.assists, p1.goldearned,
        p2.win AS win2, p2.kills AS kills2, p2.deaths AS deaths2, p2.assists AS assists2, p2.goldearned AS goldearned2,
        p2.match_id AS match_id2, p2.champion_id AS champion_id2
    FROM top_player_stats AS p1
    JOIN top_player_stats AS p2
    WHERE p1.match_id = p2.match_id AND p1.win <> p2.win
    AND p1.champion_id = '79'
)

SELECT
    c.champion_name AS self_champ_name,
    (SUM(CASE WHEN win2 = 1 THEN 1 ELSE 0 END) / SUM(CASE WHEN win = 1 OR win2 = 1 THEN 1 ELSE 0 END)) AS win_ratio,
    (SUM(kills2 + assists2) / SUM(deaths2)) AS self_kda,
    AVG(goldearned2) AS self_avg_gold,
    'Gragas' AS enemy_champ_name,
    (SUM(kills + assists) / SUM(deaths)) AS enemy_kda,
    AVG(goldearned) AS enemy_avg_gold,
    (SUM(CASE WHEN win = 1 OR win2 = 1 THEN 1 ELSE 0 END)) AS battle_record
FROM top_player_comparison AS t
JOIN champ AS c ON t.champion_id2 = c.champion_id
GROUP BY self_champ_name
HAVING battle_record > 100 AND enemy_champ_name = 'Gragas'
ORDER BY win_ratio DESC
LIMIT 5;