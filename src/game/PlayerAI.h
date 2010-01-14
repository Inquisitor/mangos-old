#ifndef MANGOS_PLAYERAI_H
#define MANGOS_PLAYERAI_H

#include "Common.h"
#include "Platform/Define.h"
#include "Policies/Singleton.h"

#include "Player.h"

class WorldObject;
class Unit;
class Player;
struct SpellEntry;

class PlayerAI
{
public:
    PlayerAI(Player* player) : m_player(player) {

        currentTarget = NULL;

        switch(m_player->getClass())
        {
        case CLASS_WARRIOR:
            break;
        case CLASS_PALADIN:
            break;
        case CLASS_HUNTER:
            break;
        case CLASS_ROGUE:
            break;
        case CLASS_PRIEST:
            break;
        case CLASS_DEATH_KNIGHT:
            break;
        case CLASS_SHAMAN:
            break;
        case CLASS_MAGE:
            addSpell(42945, 0);
            addSpell(42873, 1);
            addSpell(42833, 2);
            break;
        case CLASS_WARLOCK:
            break;
        case CLASS_DRUID:
            break;
        default:
            //wtf? :O
            m_player->i_AI = NULL;
            delete this;
            break;
        }
    }

    void UpdateAI(const uint32 diff);

    ///== Fields =======================================

    // Pointer to controlled by AI Player
    Player* m_player;

private:

    Player* GetNewTarget();
    void addSpell(uint32 spellId, uint32 slot);

    Unit *currentTarget;

    SpellEntry const *attackSpell[3];

     uint32 attackSpell1Cooldown;
     uint32 attackSpell2Cooldown;
     uint32 attackSpell3Cooldown;

    uint32 healSpell1;
    uint32 healspell2;
    uint32 healHot;

    uint32 attackStartBuff;
};
#endif
