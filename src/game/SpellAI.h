
#ifndef MANGOS_SPELL_AI_H
#define MANGOS_SPELL_AI_H

#include "Common.h"

class Spell;
class WorldObject;
class Unit;
class Creature;
class Player;
class Aura;
struct SpellEntry;

class MANGOS_DLL_SPEC SpellAI
{
    public:

        explicit SpellAI(Spell *spell) : _spell(spell) {}
        virtual ~SpellAI() {}

        // CanCast addition
        virtual SpellCastResult CanCast(bool tolerate=false) { return SPELL_FAILED_SUCCESS; }

        //Before cast and CanCast check
        virtual void PreCasted() {}

        virtual void CastFinish() {}

        virtual void AfterCasted() {}

        Spell * const _spell;
};
#endif
