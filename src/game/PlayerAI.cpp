#include "PlayerAI.h"
#include "Player.h"
#include "GridNotifiersImpl.h"

void PlayerAI::UpdateAI(const uint32 diff)
{
    if (currentTarget == NULL)
        currentTarget = GetNewTarget();

    if (currentTarget != NULL)
    {
        if (!m_player->IsNonMeleeSpellCasted(false))
            m_player->CastSpell(currentTarget, attackSpell[2], false);
    }
}

void PlayerAI::addSpell(uint32 spellId, uint32 slot)
    {
        SpellEntry const* spellproto = sSpellStore.LookupEntry(spellId);
        if (spellproto == NULL)
            return;


        attackSpell[slot] = spellproto;
        switch (slot)
        {
        case 1:
            if (m_player->GetSpellCooldownDelay(spellId) > 0)
                attackSpell1Cooldown = m_player->GetSpellCooldownDelay(spellId);
        case 2:
            if (m_player->GetSpellCooldownDelay(spellId) > 0)
                attackSpell2Cooldown = m_player->GetSpellCooldownDelay(spellId);
        case 3:
            if (m_player->GetSpellCooldownDelay(spellId) > 0)
                attackSpell3Cooldown = m_player->GetSpellCooldownDelay(spellId);
        }

    }

Player* PlayerAI::GetNewTarget()
{
    Player* pPlayer = NULL;

    /*
    uint32 radius = 50.0f;

    CellPair pair(MaNGOS::ComputeCellPair(m_player->GetPositionX(), m_player->GetPositionY()));
    Cell cell(pair);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    MaNGOS::AnyPlayerInObjectRangeCheck p_check(m_player, radius);
    MaNGOS::PlayerSearcher<MaNGOS::AnyPlayerInObjectRangeCheck>  checker(m_player, pPlayer, p_check);

    CellLock<GridReadGuard> cell_lock(cell, pair);

    Player * const plr = m_player;

    TypeContainerVisitor<MaNGOS::PlayerSearcher<MaNGOS::AnyPlayerInObjectRangeCheck>, WorldTypeMapContainer > world_object_checker(checker);
    cell_lock->Visit(cell_lock, world_object_checker, *m_player->GetMap(), *plr, radius);
    */

    return pPlayer;
}
