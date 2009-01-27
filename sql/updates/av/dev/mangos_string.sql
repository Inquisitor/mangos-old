delete from mangos_string where entry>736 and entry<770;
INSERT INTO mangos_string VALUES
(737,'Alliance',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(738,'Horde',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(739,'%s was destroyed by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(740,'The %s is under attack! If left unchecked, the %s will destroy it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(741,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(742,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(743,'The %s was taken by the %s!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(744,'The %s is under attack! If left unchecked, the %s will capture it!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(745,'The %s has taken the %s! Its supplies will now be used for reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(746,'Irondeep Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(747,'Coldtooth Mine',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(748,'Stormpike Aid Station',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(749,'Dun Baldar South Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(750,'Dun Baldar North Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(751,'Stormpike Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(752,'Icewing Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(753,'Stonehearth Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(754,'Stonehearth Bunker',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(755,'Snowfall Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(756,'Iceblood Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(757,'Iceblood Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(758,'Tower Point',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(759,'Frostwolf Graveyard',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(760,'East Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(761,'West Frostwolf Tower',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(762,'Frostwolf Relief Hut',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(763,'The Battle for Alterac Valley begins in 1 minute.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(764,'The Battle for Alterac Valley begins in 30 seconds. Prepare yourselves!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(765,'The Battle for Alterac Valley has begun!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(766,'The Alliance Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(767,'The Horde Team is running out of reinforcements!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(768,'The Frostwolf General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),
(769,'The Stormpike General is Dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL)
;

UPDATE mangos_string SET content_loc3='Allianz' WHERE entry=737;
UPDATE mangos_string SET content_loc3='Horde' WHERE entry=738;
UPDATE mangos_string SET content_loc3='%s wurde von der %s zerstört!' WHERE entry=319;
UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn zerstören!' WHERE entry=420;
-- Wenn _sie_ unbewacht bleibt wird allianz _sie_ TODO: don't store towers and gravenames, store complete strings with the names inside it, else i don't know how to implement this feature

UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=741;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=742;
UPDATE mangos_string SET content_loc3='%s wurde von der %s erobert!' WHERE entry=743;

UPDATE mangos_string SET content_loc3='%s wird angegriffen! Wenn er unbewacht bleibt, wird die %s ihn erobern!' WHERE entry=744;
-- Wenn _sie_ unbewacht bleibt ... TODO...

UPDATE mangos_string SET content_loc3='Die %s hat die %s eingenommen! Die dort befindlichen Vorräte werden zur Verstärkung eingesetzt!' WHERE entry=745;

UPDATE mangos_string SET content_loc3='Eisenschachtmine' WHERE entry=746;
UPDATE mangos_string SET content_loc3='Eisbeißermine' WHERE entry=747;
UPDATE mangos_string SET content_loc3='Das Lazarett der Sturmlanzen' WHERE entry=748;
UPDATE mangos_string SET content_loc3='Der Südbunker von Dun Baldar' WHERE entry=749;
UPDATE mangos_string SET content_loc3='Der Nordbunker von Dun Baldar' WHERE entry=750;
UPDATE mangos_string SET content_loc3='Der Friedhof der Sturmlanzen' WHERE entry=751;
UPDATE mangos_string SET content_loc3='Der Eisschwingenbunker' WHERE entry=752;
UPDATE mangos_string SET content_loc3='Der Steinbruchfriedhof' WHERE entry=753;
UPDATE mangos_string SET content_loc3='Der Steinbruchbunker' WHERE entry=754;
UPDATE mangos_string SET content_loc3='Der Schneewehenfriedhof' WHERE entry=755;
UPDATE mangos_string SET content_loc3='Der Eisblutturm' WHERE entry=756;
UPDATE mangos_string SET content_loc3='Der Eisblutfriedhof' WHERE entry=757;
UPDATE mangos_string SET content_loc3='Die Turmstellung' WHERE entry=758;
UPDATE mangos_string SET content_loc3='Der Friedhof der Frostwölfe' WHERE entry=759;
UPDATE mangos_string SET content_loc3='Der östliche Frostwolfturm' WHERE entry=760;
UPDATE mangos_string SET content_loc3='Der westliche Frostwolfturm' WHERE entry=761;
UPDATE mangos_string SET content_loc3='Die Heilerhütte der Frostwölfe' WHERE entry=762;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 1 Minute.' WHERE entry=763;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal beginnt in 30 Sekunden.' WHERE entry=764;
UPDATE mangos_string SET content_loc3='Der Kampf um das Alteractal hat begonnen!' WHERE entry=765;
UPDATE mangos_string SET content_loc3='Die Allianz hat nurnoch wenige Verstärkungspunkte' WHERE entry=766; -- TODO find right msg
UPDATE mangos_string SET content_loc3='Die Horde hat nurnoch wenige Verstärkungspunkte' WHERE entry=767; -- TODO same here
UPDATE mangos_string SET content_loc3='Der General der Allianz ist tot!' WHERE entry=768;
UPDATE mangos_string SET content_loc3='Der General der Horde ist tot!' WHERE entry=769;


-- CHAT_MSG_BG_SYSTEM_ALLIANCE', 'txt': '$N hat den Schneewehenfriedhof besetzt! Wenn niemand eingreift, erlangt die Allianz die Kontrolle \xc3\xbcber ihn
