ALTER TABLE creature_ai_scripts
  ADD COLUMN    event_requirement_type           tinyint(5) unsigned NOT NULL default '0' AFTER event_chance,
  ADD COLUMN    event_requirement_value          int(11) unsigned NOT NULL default '0' AFTER event_requirement_type;