CREATE TABLE `arena_logs` (
  `team1` int(10) unsigned NOT NULL default '0',
  `team2` int(10) unsigned NOT NULL default '0',
  `winner` int(10) unsigned NOT NULL default '0',
  `timestamp` int(10) unsigned NOT NULL default '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
