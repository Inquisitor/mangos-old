UPDATE creature c, creature_template t SET c.curhealth=t.minhealth WHERE c.map=30 AND c.id=t.entry;
UPDATE creature c, creature_template t SET c.curmana=t.minmana WHERE c.map=30 AND c.id=t.entry;
UPDATE creature SET spawndist=0 WHERE MovementType=0 AND map=30;
