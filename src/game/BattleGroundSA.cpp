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

#include "GameObject.h"
#include "BattleGround.h"
#include "BattleGroundSA.h"
#include "WorldPacket.h"
#include "Language.h"
#include "Player.h"
#include "Object.h"
#include "Creature.h"
#include "BattleGroundMgr.h"
#include "Util.h"
#include "MapManager.h"
#include "ObjectMgr.h"

/*
* BattleGround Strand of the Ancients:
* TODO:
*   - Put Seaforium charges also in last zone, just before last door. But when?
*   - Move all the harcoded variables such coords to header BattleGroundSA.h
*   - Cosmetics & avoid hacks.
*/

BattleGroundSA::BattleGroundSA()
{
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SA_START_TWO_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SA_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SA_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SA_HAS_BEGUN;

    m_BgObjects.resize(BG_SA_MAXOBJ);
    for (int32 i = 0; i <= BG_SA_GATE_MAX; ++i)
        GateStatus[i] = 1;
    TimerEnabled = false;
    countalliance = 0;
    alliance_sc = 0;
    horde_sc = 0;
    controller = HORDE;
    TimeST2Round = 60000;
    Round_timer = 0;
    team = 0;
    Phase = 1;
}

BattleGroundSA::~BattleGroundSA()
{

}

void BattleGroundSA::FillInitialWorldStates(WorldPacket& data, uint32& count)
{
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
    {
        if (m_Gyd[i] == BG_SA_GARVE_STATUS_HORDE_OCCUPIED)
            _GydOccupied(i,HORDE);
        else if (m_Gyd[i] == BG_SA_GARVE_STATUS_ALLY_OCCUPIED)
            _GydOccupied(i,ALLIANCE);
    }
    if (GetController() == HORDE)
        _GydOccupied(3,HORDE);
    else
        _GydOccupied(3,ALLIANCE);

    for (uint32 z = 0; z <= BG_SA_GATE_MAX; ++z)
        FillInitialWorldState(data, count, BG_SA_GateStatus[z], GateStatus[z]);

    //Time will be sent on first update...
    FillInitialWorldState(data, count, BG_SA_ENABLE_TIMER, TimerEnabled ? uint32(1) : uint32(0));
    FillInitialWorldState(data, count, BG_SA_TIMER_MINUTES, uint32(0));
    FillInitialWorldState(data, count, BG_SA_TIMER_10SEC, uint32(0));
    FillInitialWorldState(data, count, BG_SA_TIMER_SEC, uint32(0));
}

void BattleGroundSA::StartShips()
{
    if (shipsStarted)
        return;

    DoorOpen(m_BgObjects[BG_SA_BOAT_ONE]);
    DoorOpen(m_BgObjects[BG_SA_BOAT_TWO]);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end();itr++)
        {
            if (Player* p = sObjectMgr.GetPlayer(itr->first))
            {
                UpdateData data;
                WorldPacket pkt;
                GetBGObject(i)->BuildValuesUpdateBlockForPlayer(&data, p);
                data.BuildPacket(&pkt);
                p->GetSession()->SendPacket(&pkt);
            }
        }
    }
    shipsStarted = true;
}

void BattleGroundSA::ToggleTimer()
{
    TimerEnabled = !TimerEnabled;
    UpdateWorldState(BG_SA_ENABLE_TIMER, (TimerEnabled) ? 1 : 0);
}

void BattleGroundSA::EndBattleGround(uint32 winner)
{
    if (winner == ALLIANCE)
        ++alliance_sc;
    else if (winner == HORDE)
        ++horde_sc;
    Team win = TEAM_NONE;
    if(alliance_sc == 0 && horde_sc == 0)
    {
        RewardHonorToTeam(50, ALLIANCE);
        RewardHonorToTeam(50, HORDE);
    }
    else if(alliance_sc > horde_sc)
    {
        RewardHonorToTeam(100, ALLIANCE);
        RewardHonorToTeam(50, HORDE);
        win = ALLIANCE;
    }
    else if(alliance_sc < horde_sc)
    {
        RewardHonorToTeam(50, ALLIANCE);
        RewardHonorToTeam(100, HORDE);
        win = HORDE;
    }

    if (win)
    {
        RewardXpToTeam(0, 0.8f, win);
        RewardXpToTeam(0, 0.8f, ALLIANCE);
        RewardXpToTeam(0, 0.8f, HORDE);
    }
    BattleGround::EndBattleGround(win);
}

void BattleGroundSA::Update(uint32 diff)
{
    BattleGround::Update(diff);

    if (GetStatus() == STATUS_WAIT_JOIN && !shipsStarted)
        if (Phase == 1)
            if (shipsTimer <= diff)
                StartShips();
            else
                shipsTimer -= diff;

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (Round_timer >= BG_SA_ROUNDLENGTH)
        {
            if(Phase == 1)
            {
                PlaySoundToAll(BG_SA_SOUND_GYD_VICTORY);
                SendMessageToAll(LANG_BG_SA_NETRALL_END_1ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                ResetBattle(0);
            }
            else
            {
                SendMessageToAll(LANG_BG_SA_NETRALL_END_2ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                EndBattleGround(0);
            }
        } 
        else 
            Round_timer += diff;

        for (int gyd = 0; gyd < BG_SA_GRY_MAX; ++gyd)
        {
            // 3 sec delay to spawn new banner instead previous despawned one
            if (m_BannerTimers[gyd].timer)
            {
                if (m_BannerTimers[gyd].timer > diff)
                    m_BannerTimers[gyd].timer -= diff;
                else
                {
                    m_BannerTimers[gyd].timer = 0;
                    _CreateBanner(gyd, m_BannerTimers[gyd].type, m_BannerTimers[gyd].teamIndex, false);
                }
            }
            // 1-minute to occupy a node from contested state
            if (m_GydTimers[gyd])
            {
                if (m_GydTimers[gyd] > diff)
                    m_GydTimers[gyd] -= diff;
                else
                {
                    m_GydTimers[gyd] = 0;
                    // Change from contested to occupied !
                    uint8 teamIndex = m_Gyd[gyd]-1;
                    m_prevGyd[gyd] = m_Gyd[gyd];
                    m_Gyd[gyd] += 2;
                    // create new occupied banner
                    _CreateBanner(gyd, BG_SA_GARVE_TYPE_OCCUPIED, teamIndex, true);
                    //_SendNodeUpdate(node);
                    _GydOccupied(gyd,(teamIndex == 0) ? ALLIANCE:HORDE);
                    // Message to chatlog
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardXpToTeam(0, 0.6f, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    switch(gyd)
                    {
                        case 0: SpawnEvent(SA_EVENT_ADD_VECH_W, 0, true);break;
                        case 1: SpawnEvent(SA_EVENT_ADD_VECH_E, 0, true);break;
                    }
                    if (teamIndex == 0)
                    {
                        // SendMessage2ToAll(LANG_BG_SA_AH_SEIZES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE,NULL,LANG_BG_ALLY,_GydName(gyd));
                        PlaySoundToAll(BG_SA_SOUND_GYD_CAPTURED_ALLIANCE);
                        SendWarningToAllSA(gyd, STATUS_CONQUESTED, ALLIANCE);
                    }
                    else
                    {
                        // SendMessage2ToAll(LANG_BG_SA_AH_SEIZES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE,NULL,LANG_BG_HORDE,_GydName(gyd));
                        PlaySoundToAll(BG_SA_SOUND_GYD_CAPTURED_HORDE);
                        SendWarningToAllSA(gyd, STATUS_CONQUESTED, HORDE);
                    }
                }
            }
        }
        UpdateTimer();
    }
    if (GetStatus() == STATUS_WAIT_JOIN && Phase == 2)
    {
        if (!shipsStarted)
            if (shipsTimer <= diff)
            {
                SendMessageToAll(LANG_BG_SA_START_ONE_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                StartShips();
            }
            else
                shipsTimer -= diff;
        if (TimeST2Round < diff)
        {
            Phase = 2;
            OpenDoorEvent(SA_EVENT_OP_DOOR, 0);
            ToggleTimer();
            SetStatus(STATUS_IN_PROGRESS);
            PlaySoundToAll(SOUND_BG_START);
            SendMessageToAll(LANG_BG_SA_HAS_BEGUN, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
            SendWarningToAll(LANG_BG_SA_HAS_BEGUN);
        }
        else
            TimeST2Round -= diff;
    }
}

void BattleGroundSA::ResetWorldStates()
{
    //Player may enter BEFORE we set up bG - lets update his worldstates anyway...
    /*UpdateWorldState(BG_SA_RIGHT_GY_HORDE , GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == HORDE?1:0);
    UpdateWorldState(BG_SA_LEFT_GY_HORDE , GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == HORDE?1:0);
    UpdateWorldState(BG_SA_CENTER_GY_HORDE , GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == HORDE?1:0);

    UpdateWorldState(BG_SA_RIGHT_GY_ALLIANCE , GraveyardStatus[BG_SA_RIGHT_CAPTURABLE_GY] == ALLIANCE?1:0);
    UpdateWorldState(BG_SA_LEFT_GY_ALLIANCE , GraveyardStatus[BG_SA_LEFT_CAPTURABLE_GY] == ALLIANCE?1:0);
    UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE , GraveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == ALLIANCE?1:0);*/

    if (GetController() == ALLIANCE)
    {
        UpdateWorldState(BG_SA_ALLY_ATTACKS, 1);
        UpdateWorldState(BG_SA_HORDE_ATTACKS, 0);

        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, 1);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, 1);
        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, 0);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, 0);

        UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN,1);
        UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN,0);
    }
    else
    {
        UpdateWorldState(BG_SA_HORDE_ATTACKS, 1);
        UpdateWorldState(BG_SA_ALLY_ATTACKS, 0);

        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, 0);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, 0);
        UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, 1);
        UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, 1);

        UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN,0);
        UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN,1);
    }

    UpdateWorldState(BG_SA_PURPLE_GATEWS, 1);
    UpdateWorldState(BG_SA_RED_GATEWS, 1);
    UpdateWorldState(BG_SA_BLUE_GATEWS, 1);
    UpdateWorldState(BG_SA_GREEN_GATEWS, 1);
    UpdateWorldState(BG_SA_YELLOW_GATEWS, 1);
    UpdateWorldState(BG_SA_ANCIENT_GATEWS, 1);

}

void BattleGroundSA::UpdateTimer()
{
    uint32 end_of_round = (BG_SA_ROUNDLENGTH - Round_timer);
    UpdateWorldState(BG_SA_TIMER_MINUTES, end_of_round/60000);
    UpdateWorldState(BG_SA_TIMER_10SEC, (end_of_round%60000)/10000);
    UpdateWorldState(BG_SA_TIMER_SEC, ((end_of_round%60000)%10000)/1000);
}

void BattleGroundSA::StartingEventCloseDoors()
{
}

void BattleGroundSA::StartingEventOpenDoors()
{
    SpawnEvent(SA_EVENT_ADD_NPC, 0, true);
    ToggleTimer();
}

void BattleGroundSA::RemovePlayer(Player* /*plr*/, ObjectGuid /*guid*/)
{
}

void BattleGroundSA::RewardMedalsToTeam(uint32 teamid, bool winner)
{
    for (BattleGroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        Player *plr = sObjectMgr.GetPlayer(itr->first);

        if(plr->GetTeam()==teamid)
        {
            if(winner)
                plr->CastSpell(plr, 61160, false);
            else
                plr->CastSpell(plr, 61159, false);
        }
    }
}

void BattleGroundSA::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);

    TeleportPlayerToCorrectLoc(plr);

    BattleGroundSAScore* sc = new BattleGroundSAScore;

    m_PlayerScores[plr->GetObjectGuid()] = sc;
}

void BattleGroundSA::HandleAreaTrigger(Player * /*Source*/, uint32 /*Trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattleGroundSA::UpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{
    BattleGroundScoreMap::iterator itr = m_PlayerScores.find(Source->GetGUID());
    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    switch(type)
    {
        case SCORE_DEMOLISHERS_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->DemolishersDestroyed += value; 
            break;
        case SCORE_GATES_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->GatesDestroyed += value; 
            break;
        default:
            BattleGround::UpdatePlayerScore(Source,type,value);
            break;
    }
}

void BattleGroundSA::VirtualUpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{
    BattleGroundScoreMap::iterator itr = m_PlayerScores.find(Source->GetObjectGuid());
    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    switch(type)
    {
        case SCORE_DEMOLISHERS_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->DemolishersDestroyed += value; 
            break;
        case SCORE_GATES_DESTROYED:
            ((BattleGroundSAScore*)itr->second)->GatesDestroyed += value; 
            break;
        default:
            BattleGround::UpdatePlayerScore(Source,type,value);
            break;
    }
}

void BattleGroundSA::ResetBattle(uint32 vinner)
{
    if (vinner == ALLIANCE)
        ++alliance_sc;
    else if (vinner == HORDE)
        ++horde_sc;

    Phase = 2;
    shipsTimer = 60000;
    shipsStarted = false;
    
    for (int32 i = 0; i <= BG_SA_GATE_MAX; ++i)
        GateStatus[i] = 1;

    SetStartTime(0);
    countalliance = 0;
    controller = ALLIANCE;
    ToggleTimer();

    SetupShips();

    for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player *plr = sObjectMgr.GetPlayer(itr->first))
            TeleportPlayerToCorrectLoc(plr, true);
    }

    UpdatePhase();
}

void BattleGroundSA::Reset()
{
    //call parent's class reset
    BattleGround::Reset();
    m_ActiveEvents[SA_EVENT_ADD_GO] = BG_EVENT_NONE;
    m_ActiveEvents[SA_EVENT_ADD_NPC] = BG_EVENT_NONE;
    m_ActiveEvents[SA_EVENT_ADD_SPIR] = BG_EVENT_NONE;
    m_ActiveEvents[SA_EVENT_ADD_BOMB] = BG_EVENT_NONE;
    m_ActiveEvents[SA_EVENT_ADD_VECH_E] = BG_EVENT_NONE;
    m_ActiveEvents[SA_EVENT_ADD_VECH_W] = BG_EVENT_NONE;
    // spiritguides and flags not spawned at beginning
    UpdatePhase();
}

void BattleGroundSA::UpdatePhase()
{
    if (Phase == 2)
    {
        SpawnEvent(SA_EVENT_ADD_VECH_E, 0, false);
        SpawnEvent(SA_EVENT_ADD_VECH_W, 0, false);
        SpawnEvent(SA_EVENT_ADD_BOMB, 0, false);
        SpawnEvent(SA_EVENT_ADD_BOMB, 1, false);
        SpawnEvent(SA_EVENT_ADD_NPC, 0, false);
        OpenDoorEvent(SA_EVENT_OP_DOOR, 0);
    }
    if (GetController() == HORDE)
    {
        _GydOccupied(3,HORDE);
        m_ActiveEvents[5] = BG_SA_GARVE_STATUS_HORDE_OCCUPIED;
        SpawnEvent(SA_EVENT_ADD_BOMB, 0, true);
        for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
        {
            m_prevGyd[i] = 0;
            m_GydTimers[i] = 0;
            m_BannerTimers[i].timer = 0;
            m_Gyd[i] = BG_SA_GARVE_STATUS_HORDE_OCCUPIED;
            m_ActiveEvents[i] = BG_SA_GARVE_STATUS_HORDE_OCCUPIED;
            _GydOccupied(i,HORDE);
        }
    }
    if (GetController() == ALLIANCE)
    {
        _GydOccupied(3,ALLIANCE);
        SpawnEvent(SA_EVENT_ADD_SPIR, BG_SA_GARVE_STATUS_HORDE_OCCUPIED, false);
        SpawnEvent(SA_EVENT_ADD_SPIR, BG_SA_GARVE_STATUS_ALLY_OCCUPIED, true);
        m_ActiveEvents[5] = BG_SA_GARVE_STATUS_ALLY_OCCUPIED;
        SpawnEvent(SA_EVENT_ADD_BOMB, 1, true);
        SpawnEvent(SA_EVENT_ADD_NPC, 0, true);
        for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
        {
            for (uint8 z = 1; z < 5; ++z)
                SpawnEvent(i, z, false);

            m_prevGyd[i] = 0;
            m_GydTimers[i] = 0;
            m_BannerTimers[i].timer = 0;
            SpawnEvent(i, 3, true); 
            m_Gyd[i] = BG_SA_GARVE_STATUS_ALLY_OCCUPIED;
            m_ActiveEvents[i] = BG_SA_GARVE_STATUS_ALLY_OCCUPIED;
            _GydOccupied(i,ALLIANCE);
        }
    }

    for (uint32 z = 0; z <= BG_SA_GATE_MAX; ++z)
        UpdateWorldState(BG_SA_GateStatus[z], GateStatus[z]);

    if (Phase == 2)
    {
        Round_timer = 0;
        SetStatus(STATUS_WAIT_JOIN);
        SendMessageToAll(LANG_BG_SA_START_ONE_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
    }

    SpawnEvent(SA_EVENT_ADD_GO, 0, false);
    SpawnEvent(SA_EVENT_ADD_GO, 0, true);
}

bool BattleGroundSA::SetupBattleGround()
{
    return SetupShips();
}

bool BattleGroundSA::SetupShips()
{
    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player *plr = sObjectMgr.GetPlayer(itr->first))
                SendTransportsRemove(plr);
    for (uint8 i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        uint32 boatid=0;
        switch (i)
        {
            case BG_SA_BOAT_ONE:
                boatid = GetController() == ALLIANCE ? BG_SA_BOAT_ONE_H : BG_SA_BOAT_ONE_A;
                break;
            case BG_SA_BOAT_TWO:
                boatid = GetController() == ALLIANCE ? BG_SA_BOAT_TWO_H : BG_SA_BOAT_TWO_A;
                break;
        }
        if (!(AddObject(i, boatid, BG_SA_START_LOCATIONS[i + 5][0], BG_SA_START_LOCATIONS[i + 5][1], BG_SA_START_LOCATIONS[i + 5][2]+ (GetController() == ALLIANCE ? -3.750f: 0) , BG_SA_START_LOCATIONS[i + 5][3], 0, 0, 0, 0, RESPAWN_ONE_DAY)))
        {
            sLog.outError("SA_ERROR: Can't spawn ships!");
            return false;
        }
    }

    GetBGObject(BG_SA_BOAT_ONE)->UpdateRotationFields(1.0f, 0.0002f);
    GetBGObject(BG_SA_BOAT_TWO)->UpdateRotationFields(1.0f, 0.00001f);
    SpawnBGObject(m_BgObjects[BG_SA_BOAT_ONE], RESPAWN_IMMEDIATELY);
    SpawnBGObject(m_BgObjects[BG_SA_BOAT_TWO], RESPAWN_IMMEDIATELY);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
        for (BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player *plr = sObjectMgr.GetPlayer(itr->first))
                SendTransportInit(plr);
    return true;
}

/*  type: 0-neutral, 1-contested, 3-occupied
    team: 0-ally, 1-horde                        */
void BattleGroundSA::_CreateBanner(uint8 gry, uint8 type, uint8 teamIndex, bool delay)
{
    // Just put it into the queue
    if (delay)
    {
        m_BannerTimers[gry].timer = 2000;
        m_BannerTimers[gry].type = type;
        m_BannerTimers[gry].teamIndex = teamIndex;
        return;
    }

    // cause the node-type is in the generic form
    // please see in the headerfile for the ids
    type += teamIndex;

    SpawnEvent(gry, type, true);                           // will automaticly despawn other events
}

void BattleGroundSA::EventPlayerClickedOnFlag(Player *source, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 event = (sBattleGroundMgr.GetGameObjectEventIndex(target_obj->GetDBTableGUIDLow())).event1;
    if (event >= BG_SA_GRY_MAX)                           // not a node
        return;
    BG_SA_GraveYard gyd = BG_SA_GraveYard(event);

    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(source->GetTeam());

    // Check if player really could use this banner, not cheated
    if (!(m_Gyd[gyd] == 0 || teamIndex == m_Gyd[gyd] % 2))
        return;
    uint32 sound = 0;

    if ((m_Gyd[gyd] == BG_SA_GARVE_STATUS_ALLY_CONTESTED) || (m_Gyd[gyd] == BG_SA_GARVE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevGyd[gyd] < BG_SA_GARVE_TYPE_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GARVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GARVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(source, SCORE_BASES_DEFENDED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GARVE_TYPE_OCCUPIED;
            // create new occupied banner
            _CreateBanner(gyd, BG_SA_GARVE_TYPE_OCCUPIED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = 0;
            //_NodeOccupied(node,(teamIndex == BG_TEAM_ALLIANCE) ? ALLIANCE:HORDE);

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
        }
        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_SA_SOUND_GYD_ASSAULTED_ALLIANCE : BG_SA_SOUND_GYD_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else if (controller == HORDE)
    {
        if (m_Gyd[gyd] == BG_SA_GARVE_STATUS_HORDE_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GARVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GARVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
            sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_SA_SOUND_GYD_ASSAULTED_ALLIANCE : BG_SA_SOUND_GYD_ASSAULTED_HORDE;
        }
    }
    else if (controller == ALLIANCE)
    {
        if (m_Gyd[gyd] == BG_SA_GARVE_STATUS_ALLY_OCCUPIED)
        {
            //UpdatePlayerScore(source, SCORE_BASES_ASSAULTED, 1);
            m_prevGyd[gyd] = m_Gyd[gyd];
            m_Gyd[gyd] = teamIndex + BG_SA_GARVE_TYPE_CONTESTED;
            // create new contested banner
            _CreateBanner(gyd, BG_SA_GARVE_TYPE_CONTESTED, teamIndex, true);
            //_SendNodeUpdate(node);
            m_GydTimers[gyd] = BG_SA_FLAG_CAPTURING_TIME;

            if (teamIndex == BG_TEAM_ALLIANCE)
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_ALLIANCE, source, LANG_BG_ALLY, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, ALLIANCE);
            }
            else
            {
                // SendMessage2ToAll(LANG_BG_SA_AH_PRECIPITATES_GRAVEYARD,CHAT_MSG_BG_SYSTEM_HORDE, source, LANG_BG_HORDE, _GydName(gyd));
                SendWarningToAllSA(gyd, STATUS_CLAIMED, HORDE);
            }
            sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_SA_SOUND_GYD_ASSAULTED_ALLIANCE : BG_SA_SOUND_GYD_ASSAULTED_HORDE;
        }
    }
   PlaySoundToAll(sound);
}

void BattleGroundSA::EventSpawnGOSA(Player *owner, Creature* obj, float x, float y, float z)
{
	SendMessageToAll(LANG_BG_SA_INSTALL_BOMB, (controller == ALLIANCE) ? CHAT_MSG_BG_SYSTEM_HORDE : CHAT_MSG_BG_SYSTEM_ALLIANCE , owner);
}

void BattleGroundSA::SendMessageSA(Player *player, uint32 type, uint32 name)
{
    uint32 entryMSG = 0;
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());
    switch (type)
    {
        case 0: entryMSG = LANG_BG_SA_GATE_ATTACK; break;
        case 1: entryMSG = LANG_BG_SA_GATE_DAMAGE; break;
        case 2: entryMSG = LANG_BG_SA_GATE_DETROYED; break;
    }
    if (teamIndex == BG_TEAM_ALLIANCE)
        SendMessage2ToAll(entryMSG,CHAT_MSG_BG_SYSTEM_ALLIANCE, player, name);
    else
        SendMessage2ToAll(entryMSG,CHAT_MSG_BG_SYSTEM_HORDE, player, name);
}

void BattleGroundSA::EventPlayerDamageGO(Player *player, GameObject* target_obj, uint32 eventId)
{
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    uint32 type = NULL;
    switch (target_obj->GetEntry())
    {
        case BG_SA_GO_GATES_ROOM_ANCIENT_SHRINE:
        {
            type = BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19836:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19837:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 75, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_GREEN_EMERALD:
        {
            type = BG_SA_GO_GATES_T_GREEN_EMERALD;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19041:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19046:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_BLUE_SAPHIRE:
        {
            type = BG_SA_GO_GATES_T_BLUE_SAPHIRE;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19040:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19045:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_MAUVE_AMETHYST:
        {
            type = BG_SA_GO_GATES_T_MAUVE_AMETHYST;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19043:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19048:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_RED_SUN:
        {
            type = BG_SA_GO_GATES_T_RED_SUN;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19042:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19047:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_GATES_YELLOW_MOON:
        {
            type = BG_SA_GO_GATES_T_YELLOW_MOON;
            switch (eventId)
            {
                case 21630:
                    SendMessageSA(player, BG_SA_ATTACK, _GatesName(target_obj));
                    break;
                case 19044:
                    // SendMessageSA(player, BG_SA_DAMAGE, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DAMAGE);
                    break;
                case 19049:
                    // SendMessageSA(player, BG_SA_DESTROY, _GatesName(target_obj));
                    SendWarningToAllSA(NULL, NULL, TEAM_NONE, true, type, true);
                    UpdateWorldState(BG_SA_GateStatus[type], GateStatus[type] = BG_SA_GO_GATES_DESTROY);
                    UpdatePlayerScore(player, SCORE_GATES_DESTROYED, 1);
                    RewardHonorToTeam(85, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 65, (teamIndex == 0) ? ALLIANCE:HORDE);
                    break;
            }
            break;
        }
        case BG_SA_GO_TITAN_RELIC:
        {
            if (eventId == 22097 && player->GetTeam() != GetController())
                if(Phase == 1)
                {
                    PlaySoundToAll(BG_SA_SOUND_GYD_VICTORY);
                    SendMessageToAll(LANG_BG_SA_ALLIANCE_END_1ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                    RewardHonorToTeam(150, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    ResetBattle(player->GetTeam());
                }
                else
                {
                    SendMessageToAll(LANG_BG_SA_HORDE_END_2ROUND, CHAT_MSG_BG_SYSTEM_NEUTRAL, NULL);
                    RewardHonorToTeam(150, (teamIndex == 0) ? ALLIANCE:HORDE);
                    RewardReputationToTeam((teamIndex == 0) ? 1050:1085, 100, (teamIndex == 0) ? ALLIANCE:HORDE);
                    EndBattleGround(player->GetTeam());
                }
            break;
        }
    }
}

void BattleGroundSA::HandleKillUnit(Creature* unit, Player* killer)
{
    if(!unit)
        return;

    if(unit->GetEntry() == 28781)  //Demolisher
        UpdatePlayerScore(killer, SCORE_DEMOLISHERS_DESTROYED, 1);
}

int32 BattleGroundSA::_GatesName(GameObject* obj)
{
    if (!obj)
        return 0;
    switch (obj->GetEntry())
    {
        case BG_SA_GO_GATES_ROOM_ANCIENT_SHRINE:  return LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE;
        case BG_SA_GO_GATES_GREEN_EMERALD:  return LANG_BG_SA_GATE_GREEN_EMERALD;
        case BG_SA_GO_GATES_BLUE_SAPHIRE:  return LANG_BG_SA_GATE_BLUE_SAPHIRE;
        case BG_SA_GO_GATES_MAUVE_AMETHYST:  return LANG_BG_SA_GATE_MAUVE_AMETHYST;
        case BG_SA_GO_GATES_RED_SUN:  return LANG_BG_SA_GATE_RED_SUN_;
        case BG_SA_GO_GATES_YELLOW_MOON:  return LANG_BG_SA_GATE_YELLOW_MOON;
        default:
            MANGOS_ASSERT(0);
    }
    return 0;
}

int32 BattleGroundSA::_GydName(uint8 gyd)
{
    switch (gyd)
    {
        case 0:  return LANG_BG_SA_EAST_GRAVEYARD;
        case 1:  return LANG_BG_SA_WEST_GRAVEYARD;
        case 2:  return LANG_BG_SA_SOUTH_GRAVEYARD;
        default:
            MANGOS_ASSERT(0);
    }
    return 0;
}

WorldSafeLocsEntry const* BattleGroundSA::GetClosestGraveYard(Player* player)
{
    BattleGroundTeamIndex teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> gyd;
    for (uint8 i = 0; i < BG_SA_GRY_MAX; ++i)
        if (m_Gyd[i] == teamIndex + 3)
            gyd.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    // If so, select the closest node to place ghost on
    if (!gyd.empty())
    {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < gyd.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIdsPhase[gyd[i]]);
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x)+(entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        gyd.clear();
    }
    // If not, place ghost on starting location
    if (!good_entry)
    {
        if (GetController() == HORDE)
        {
            if (teamIndex == 0)
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[1]);
            else
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[0]);
        }
        if (GetController() == ALLIANCE)
        {
            if (teamIndex == 0)
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[0]);
            else
                good_entry = sWorldSafeLocsStore.LookupEntry(BG_SA_GraveyardIds[1]);
        }
    }
    return good_entry;
}

void BattleGroundSA::_GydOccupied(uint8 node,Team team)
{
    if (node >= 0 && node < 3)
    {
        switch (team)
        {
            case ALLIANCE:
            {
                UpdateWorldState(GrraveYardWS[node][1], 0);
                UpdateWorldState(GrraveYardWS[node][0], 1);
                break;
            }
            case HORDE: 
            {
                UpdateWorldState(GrraveYardWS[node][0], 0);
                UpdateWorldState(GrraveYardWS[node][1], 1);
                break;
            }
        }
    }
    else if (node == 3)
    {
        switch (team)
        {
            case HORDE: 
            {
                for (int8 i = 0; i <= BG_SA_MAX_WS; ++i)
                {
                    UpdateWorldState(BG_SA_WorldStatusH[i], 1);
                    UpdateWorldState(BG_SA_WorldStatusA[i], 0);
                }
                break;
            }
            case ALLIANCE: 
            {
                for (int8 i = 0; i <= BG_SA_MAX_WS; ++i)
                {
                    UpdateWorldState(BG_SA_WorldStatusH[i], 0);
                    UpdateWorldState(BG_SA_WorldStatusA[i], 1);
                }
                break;
            }
        }
    }
}

void BattleGroundSA::SendWarningToAllSA(uint8 gyd, int status, Team team, bool isDoor, int door, bool destroyed)
{
    if (!isDoor)
    {
        switch(status)
        {
            case STATUS_CLAIMED:
            {
                if (team == HORDE)
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_HORDE_EAST_CLAIMED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_HORDE_WEST_CLAIMED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_HORDE_SOUTH_CLAIMED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                else
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_ALLIANCE_EAST_CLAIMED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_ALLIANCE_WEST_CLAIMED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_ALLIANCE_SOUTH_CLAIMED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                break;
            }
            case STATUS_CONQUESTED:
            {
                if (team == HORDE)
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_HORDE_EAST_CONQUESTED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_HORDE_WEST_CONQUESTED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_HORDE_SOUTH_CONQUESTED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                else
                {
                    switch(gyd)
                    {
                        case 0: SendWarningToAll(LANG_BG_SA_ALLIANCE_EAST_CONQUESTED); break;
                        case 1: SendWarningToAll(LANG_BG_SA_ALLIANCE_WEST_CONQUESTED); break;
                        case 2: SendWarningToAll(LANG_BG_SA_ALLIANCE_SOUTH_CONQUESTED); break;
                        default: sLog.outError("Error in SA strings: Unknow graveyard %s", gyd); break;
                    }
                }
                break;
            }
            default:
                sLog.outError("Error in SA strings: Unknow status %s", status); break;
        }
    }
    else
    {
        if(destroyed)
        {
            switch(door)
            {
                case BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE: SendWarningToAll(LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE_DESTROYED); break;
                case BG_SA_GO_GATES_T_GREEN_EMERALD: SendWarningToAll(LANG_BG_SA_GATE_GREEN_EMERALD_DESTROYED); break;
                case BG_SA_GO_GATES_T_BLUE_SAPHIRE: SendWarningToAll(LANG_BG_SA_GATE_BLUE_SAPHIRE_DESTROYED); break;
                case BG_SA_GO_GATES_T_MAUVE_AMETHYST: SendWarningToAll(LANG_BG_SA_GATE_MAUVE_AMETHYST_DESTROYED); break;
                case BG_SA_GO_GATES_T_RED_SUN: SendWarningToAll(LANG_BG_SA_GATE_RED_SUN_DESTROYED); break;
                case BG_SA_GO_GATES_T_YELLOW_MOON: SendWarningToAll(LANG_BG_SA_GATE_YELLOW_MOON_DESTROYED); break;
                default:
                    sLog.outError("Error in SA strings: Unknow door %s", door); break;
            }
        }
        else
        {
            switch(door)
            {
                case BG_SA_GO_GATES_T_ROOM_ANCIENT_SHRINE: SendWarningToAll(LANG_BG_SA_GATE_ROOM_ANCIENT_SHRINE_DAMAGED); break;
                case BG_SA_GO_GATES_T_GREEN_EMERALD: SendWarningToAll(LANG_BG_SA_GATE_GREEN_EMERALD_DAMAGED); break;
                case BG_SA_GO_GATES_T_BLUE_SAPHIRE: SendWarningToAll(LANG_BG_SA_GATE_BLUE_SAPHIRE_DAMAGED); break;
                case BG_SA_GO_GATES_T_MAUVE_AMETHYST: SendWarningToAll(LANG_BG_SA_GATE_MAUVE_AMETHYST_DAMAGED); break;
                case BG_SA_GO_GATES_T_RED_SUN: SendWarningToAll(LANG_BG_SA_GATE_RED_SUN_DAMAGED); break;
                case BG_SA_GO_GATES_T_YELLOW_MOON: SendWarningToAll(LANG_BG_SA_GATE_YELLOW_MOON_DAMAGED); break;
                default:
                    sLog.outError("Error in SA strings: Unknow door %s", door); break;
            }
        }
    }
}

void BattleGroundSA::TeleportPlayerToCorrectLoc(Player *plr, bool resetBattle)
{
    if (!plr)
        return;

    if (!shipsStarted)
    {
        if (plr->GetTeam() != GetController())
        {
            plr->CastSpell(plr,12438,true);//Without this player falls before boat loads...

            if (urand(0,1))
                plr->TeleportTo(607, 2682.936f, -830.368f, 50.0f, 2.895f, 0);
            else
                plr->TeleportTo(607, 2577.003f, 980.261f, 50.0f, 0.807f, 0);

        }
        else
            plr->TeleportTo(607, 1209.7f, -65.16f, 70.1f, 0.0f, 0);
    }
    else
    {
        if (plr->GetTeam() != GetController())
            plr->TeleportTo(607, 1600.381f, -106.263f, 8.8745f, 3.78f, 0);
        else
            plr->TeleportTo(607, 1209.7f, -65.16f, 70.1f, 0.0f, 0);
    }
    SendTransportInit(plr);
    if (resetBattle)
    {
        if (!plr->isAlive())
        {
            plr->ResurrectPlayer(1.0f);
            plr->SpawnCorpseBones();
        }

        plr->SetHealth(plr->GetMaxHealth());
        plr->SetPower(POWER_MANA, plr->GetMaxPower(POWER_MANA));
        plr->CombatStopWithPets(true);
    }
}

void BattleGroundSA::SendTransportInit(Player *player)
{
    if (GetBGObject(BG_SA_BOAT_ONE) || GetBGObject(BG_SA_BOAT_TWO))
    {
        UpdateData transData;
        if (GetBGObject(BG_SA_BOAT_ONE))
            GetBGObject(BG_SA_BOAT_ONE)->BuildCreateUpdateBlockForPlayer(&transData, player);
        if (GetBGObject(BG_SA_BOAT_TWO))
            GetBGObject(BG_SA_BOAT_TWO)->BuildCreateUpdateBlockForPlayer(&transData, player);
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

void BattleGroundSA::SendTransportsRemove(Player * player)
{
    if (GetBGObject(BG_SA_BOAT_ONE) || GetBGObject(BG_SA_BOAT_TWO))
    {
        UpdateData transData;
        if (GetBGObject(BG_SA_BOAT_ONE))
            GetBGObject(BG_SA_BOAT_ONE)->BuildOutOfRangeUpdateBlock(&transData);
        if (GetBGObject(BG_SA_BOAT_TWO))
            GetBGObject(BG_SA_BOAT_TWO)->BuildOutOfRangeUpdateBlock(&transData);
        WorldPacket packet;
        transData.BuildPacket(&packet);
        player->GetSession()->SendPacket(&packet);
    }
}

uint32 BattleGroundSA::GetCorrectFactionSA(uint8 vehicleType) const
{
    if (GetStatus() != STATUS_WAIT_JOIN)
    {
        switch(vehicleType)
        {
            case VEHICLE_SA_DEMOLISHER:
            {
                if (GetController() == ALLIANCE)
                    return VEHICLE_FACTION_HORDE;
                else
                    return VEHICLE_FACTION_ALLIANCE;
            }
            case VEHICLE_SA_CANNON:
            {
                if (GetController() == ALLIANCE)
                    return VEHICLE_FACTION_ALLIANCE;
                else
                    return VEHICLE_FACTION_HORDE;
            }
            default:
                return VEHICLE_FACTION_NEUTRAL;
        }
    }
    return VEHICLE_FACTION_NEUTRAL;
}