SELECT c.champion_name, COUNT(*) AS cnt
FROM participant p
JOIN champ c ON p.champion_id = c.champion_id
WHERE p.position = 'JUNGLE'
GROUP BY c.champion_name
ORDER BY cnt DESC
LIMIT 3;