alter table `battleground_template`
    add column `view_range` int(10) UNSIGNED DEFAULT '80' NOT NULL after `HordeStartO`;

alter table `instance_template`
    add column `view_range` int(10) UNSIGNED DEFAULT '80' NOT NULL after `script`;