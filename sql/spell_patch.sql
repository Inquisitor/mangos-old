/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;

DROP TABLE IF EXISTS spell_patch;
CREATE TABLE spell_patch (
  entry INT(11) NOT NULL,
  dbc VARCHAR(255) DEFAULT NULL,
  `data` VARCHAR(255) DEFAULT NULL,
  `comment` VARCHAR(255) DEFAULT NULL,
  PRIMARY KEY (entry)
)
;



INSERT INTO spell_patch VALUES 
  (49036, 'Spell.dbc', '124 80', 'Epidemic-49036 (Update EffectSpellClassMaskA3 )'),
  (49562, 'Spell.dbc', '124 80', 'Epidemic-49562 (Update EffectSpellClassMaskA3 )'),
  (50461, 'Spell.dbc', '40 1', 'Anti-Magic Zone-50461 (Update DurationIndex )'),
  (50508, 'Spell.dbc', '95 255', 'Crypt Fever-50508 (Update EffectApplyAuraName1 )'),
  (50509, 'Spell.dbc', '95 255', 'Crypt Fever-50509 (Update EffectApplyAuraName1 )'),
  (50510, 'Spell.dbc', '95 255', 'Crypt Fever-50510 (Update EffectApplyAuraName1 )'),
  (50782, 'Spell.dbc', '42 0', 'Slam-50782 (Update ManaCost )'),
  (51726, 'Spell.dbc', '95 255 96 87', 'Ebon Plague-51726 (Update EffectApplyAuraName1 EffectApplyAuraName2 )'),
  (51734, 'Spell.dbc', '95 255 96 87', 'Ebon Plague-51734 (Update EffectApplyAuraName1 EffectApplyAuraName2 )'),
  (51735, 'Spell.dbc', '95 255 96 87', 'Ebon Plague-51735 (Update EffectApplyAuraName1 EffectApplyAuraName2 )'),
  (60544, 'Spell.dbc', '72 0', 'Savage Aggression'),
  (60547, 'Spell.dbc', '72 0', 'Hateful Aggression'),
  (60549, 'Spell.dbc', '72 0', 'Deadly Aggression'),
  (60551, 'Spell.dbc', '72 0', 'Furious Gladiator''s Libram of Fortitude'),
  (60553, 'Spell.dbc', '72 0', 'Relentless Aggression'),
  (60555, 'Spell.dbc', '72 0', 'LK Arena 6 Aggression'),
  (60565, 'Spell.dbc', '72 0', 'Savage Magic'),
  (60566, 'Spell.dbc', '72 0', 'Hateful Magic'),
  (60567, 'Spell.dbc', '72 0', 'Deadly Magic'),
  (60568, 'Spell.dbc', '72 0', 'Furious Gladiator''s Idol of Steadfastness'),
  (60569, 'Spell.dbc', '72 0', 'Relentless Survival'),
  (60570, 'Spell.dbc', '72 0', 'LK Arena 6 Magic'),
  (63335, 'Spell.dbc', '122 0 123 2', 'Glyph of Howling Blast-63335 (Update EffectSpellClassMaskA1 EffectSpellClassMaskA2 )'),
  (71132, 'Spell.dbc', '80 0', 'Glyph of Shadow Word: Pain-71132 (Update EffectBasePoints1 )'),
  (71838, 'Spell.dbc', '6 536870912', 'Drain Life-71838 (Update AttributesEx2 )'),
  (71839, 'Spell.dbc', '6 536870912', 'Drain Life-71839 (Update AttributesEx2 )');

-- 
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;