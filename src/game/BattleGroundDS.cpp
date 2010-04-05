/*
 * Copyright (C) 2005-2010 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Object.h"
#include "Player.h"
#include "BattleGround.h"
#include "BattleGroundDS.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "WorldPacket.h"

BattleGroundDS::BattleGroundDS()
{
    m_StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    m_StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    m_StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    m_StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set messageIds
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattleGroundDS::~BattleGroundDS()
{

}

void BattleGroundDS::Update(uint32 diff)
{
    BattleGround::Update(diff);

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        // push people from the tubes
        if(pushbackCheck)
            if(m_uiKnockback < diff)
            {
                for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if(Player *plr = sObjectMgr.GetPlayer(itr->first))
                    {
                        if(plr->GetPositionZ() < 11.0f)
                            continue;

                        float angle = (plr->GetBGTeam() == ALLIANCE /* gold */) ? plr->GetAngle(1259.58f, 764.43f) : plr->GetAngle(1325.84f, 817.304f);

                        WorldPacket data(SMSG_MOVE_KNOCK_BACK, 8+4+4+4+4+4);
                        data << plr->GetPackGUID();
                        data << uint32(0);                                  // Sequence
                        data << float(cos(angle));                          // x direction
                        data << float(sin(angle));                          // y direction
                        data << float(55);                                  // Horizontal speed
                        data << float(-7);                                  // Z Movement speed (vertical)
                        plr->GetSession()->SendPacket(&data);
                    }
                }

                pushbackCheck = false;
            }
            else
                m_uiKnockback -= diff;

        // in case pushback failed
        if(teleportCheck)
            if(m_uiTeleport < diff)
            {
                for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if(Player *plr = sObjectMgr.GetPlayer(itr->first))
                    {
                        if(plr->GetPositionZ() < 11.0f)
                            continue;

                        float x, y;
                        if (plr->GetBGTeam() == ALLIANCE)
                        {
                            x = 1259.58f;
                            y = 764.43f;
                        }
                        else
                        {
                            x = 1325.84f;
                            y = 817.304f;
                        }

                        plr->TeleportTo(GetMapId(), x + urand(0,2), y + urand(0,2), 3.15f, plr->GetOrientation());
                    }
                }

                teleportCheck = false;

                // close the gate
                OpenDoorEvent(BG_EVENT_DOOR);
            }
            else
                m_uiTeleport -= diff;
    }
}

void BattleGroundDS::StartingEventCloseDoors()
{
}

void BattleGroundDS::StartingEventOpenDoors()
{
    OpenDoorEvent(BG_EVENT_DOOR);
}

void BattleGroundDS::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattleGroundDSScore* sc = new BattleGroundDSScore;

    m_PlayerScores[plr->GetGUID()] = sc;

    UpdateWorldState(0xe11, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(0xe10, GetAlivePlayersCountByTeam(HORDE));
}

void BattleGroundDS::RemovePlayer(Player* /*plr*/, uint64 /*guid*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateWorldState(0xe11, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(0xe10, GetAlivePlayersCountByTeam(HORDE));

    CheckArenaWinConditions();
}

void BattleGroundDS::HandleKillPlayer(Player *player, Player *killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog.outError("Killer player not found");
        return;
    }

    BattleGround::HandleKillPlayer(player,killer);

    UpdateWorldState(0xe11, GetAlivePlayersCountByTeam(ALLIANCE));
    UpdateWorldState(0xe10, GetAlivePlayersCountByTeam(HORDE));

    CheckArenaWinConditions();
}
 
bool BattleGroundDS::HandlePlayerUnderMap(Player *player)
{
    player->TeleportTo(GetMapId(),1299.046f, 784.825f, 9.338f, player->GetOrientation());
    return true;
}
 
void BattleGroundDS::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch(Trigger)
    {
        case 5347:
        case 5348:
            break;
        default:
            sLog.outError("WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }
}
 
void BattleGroundDS::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(0xe11) << uint32(GetAlivePlayersCountByTeam(ALLIANCE));           // 7
    data << uint32(0xe10) << uint32(GetAlivePlayersCountByTeam(HORDE));           // 8
    data << uint32(0xe1a) << uint32(1);           // 9
}

void BattleGroundDS::Reset()
{
    BattleGround::Reset();

    m_uiTeleport = 20000;
    teleportCheck = true;

    m_uiKnockback = 15000;
    pushbackCheck = true;
}

bool BattleGroundDS::SetupBattleGround()
{
    return true;
}
