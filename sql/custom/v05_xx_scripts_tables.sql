ALTER TABLE quest_end_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;

ALTER TABLE quest_start_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;

ALTER TABLE spell_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;

ALTER TABLE gameobject_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;

ALTER TABLE event_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;


ALTER TABLE gossip_scripts
  ADD COLUMN    requirement_type           int(11) unsigned NOT NULL default '0' AFTER delay,
  ADD COLUMN    requirement_value          int(11) unsigned NOT NULL default '0' AFTER requirement_type;