#!/bin/bash
echo "
-- general stuff
" > full.sql
cat "bg-av.sql" >> full.sql
echo "
-- language
" >> full.sql
cat "mangos_string.sql" >> full.sql
echo "
-- creature stats
" >> full.sql
cat "creature_template.sql" >> full.sql
echo "
-- creature spawns
" >> full.sql
echo "
-- gameobject-spawns
" >> full.sql
cat "gameobjects.sql" >> full.sql
echo "
-- eventai
" >> full.sql
cat "eventai.sql" >> full.sql
