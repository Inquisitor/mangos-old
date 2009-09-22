/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Player.h"
#include "BattleGround.h"
#include "BattleGroundAV.h"
#include "BattleGroundMgr.h"
#include "Creature.h"
#include "Chat.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "Language.h"
#include "SpellAuras.h"
#include "Formulas.h"
#include "WorldPacket.h"

BattleGroundAV::BattleGroundAV()
{
    m_StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_AV_START_TWO_MINUTES;
    m_StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_AV_START_ONE_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_AV_START_HALF_MINUTE;
    m_StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_AV_HAS_BEGUN;
}

BattleGroundAV::~BattleGroundAV()
{
}

void BattleGroundAV::HandleKillPlayer(Player *player, Player *killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    BattleGround::HandleKillPlayer(player, killer);
    UpdateScore(GetTeamIndexByTeamId(player->GetTeam()), -1);
}

void BattleGroundAV::HandleKillUnit(Creature *creature, Player *killer)
{
    sLog.outDebug("BattleGroundAV: HandleKillUnit %i", creature->GetEntry());
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
    BattleGroundEventIdx eventId = sBattleGroundMgr.GetCreatureEventIndex(creature->GetDBTableGUIDLow());
    if (eventId.event1 == BG_EVENT_NONE)
        return;
    switch(eventId.event1)
    {
        case BG_AV_BOSS_A:
            CastSpellOnTeam(BG_AV_BOSS_KILL_QUEST_SPELL, HORDE);   // this is a spell which finishes a quest where a player has to kill the boss
            RewardReputationToTeam(729, m_RepBoss, HORDE);
            RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_BOSS), HORDE);
            SendYellToAll(LANG_BG_AV_H_GENERAL_DEAD, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0));
            EndBattleGround(HORDE);
            break;
        case BG_AV_BOSS_H:
            CastSpellOnTeam(BG_AV_BOSS_KILL_QUEST_SPELL, ALLIANCE); // this is a spell which finishes a quest where a player has to kill the boss
            RewardReputationToTeam(730, m_RepBoss, ALLIANCE);
            RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_BOSS), ALLIANCE);
            SendYellToAll(LANG_BG_AV_A_GENERAL_DEAD, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0));
            EndBattleGround(ALLIANCE);
            break;
        case BG_AV_CAPTAIN_A:
            if (IsActiveEvent(BG_AV_NodeEventCaptainDead_A, 0))
                return;
            RewardReputationToTeam(729, m_RepCaptain, HORDE);
            RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_CAPTAIN), HORDE);
            UpdateScore(BG_TEAM_ALLIANCE, (-1) * BG_AV_RES_CAPTAIN);
            // spawn destroyed aura
            SpawnEvent(BG_AV_NodeEventCaptainDead_A, 0, true);
            break;
        case BG_AV_CAPTAIN_H:
            if (IsActiveEvent(BG_AV_NodeEventCaptainDead_H, 0))
                return;
            RewardReputationToTeam(730, m_RepCaptain, ALLIANCE);
            RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_CAPTAIN), ALLIANCE);
            UpdateScore(BG_TEAM_HORDE, (-1) * BG_AV_RES_CAPTAIN);
            // spawn destroyed aura
            SpawnEvent(BG_AV_NodeEventCaptainDead_H, 0, true);
            break;
        case BG_AV_MINE_BOSSES_NORTH:
            ChangeMineOwner(BG_AV_NORTH_MINE, GetTeamIndexByTeamId(killer->GetTeam()));
            break;
        case BG_AV_MINE_BOSSES_SOUTH:
            ChangeMineOwner(BG_AV_SOUTH_MINE, GetTeamIndexByTeamId(killer->GetTeam()));
            break;
    }
}

void BattleGroundAV::HandleQuestComplete(uint32 questid, Player *player)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
    uint8 team = GetTeamIndexByTeamId(player->GetTeam());
    uint32 reputation = 0;                                  // reputation for the whole team (other reputation must be done in db)
    // TODO add events (including quest not available anymore, next quest availabe, go/npc de/spawning)
    // maybe we can do it with sd2?
    sLog.outError("BattleGroundAV: Quest %i completed", questid);
    switch(questid)
    {
        case BG_AV_QUEST_A_SCRAPS1:
        case BG_AV_QUEST_A_SCRAPS2:
        case BG_AV_QUEST_H_SCRAPS1:
        case BG_AV_QUEST_H_SCRAPS2:
            m_Team_QuestStatus[team][0] += 20;
            reputation = 1;
            if( m_Team_QuestStatus[team][0] == 500 || m_Team_QuestStatus[team][0] == 1000 || m_Team_QuestStatus[team][0] == 1500 ) //25,50,75 turn ins
            {
                sLog.outDebug("BattleGroundAV: Quest %i completed starting with unit upgrading..", questid);
                for (BG_AV_Nodes i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
                    if (m_Nodes[i].Owner == team && m_Nodes[i].State == POINT_CONTROLLED)
                        PopulateNode(i);
            }
            break;
        case BG_AV_QUEST_A_COMMANDER1:
        case BG_AV_QUEST_H_COMMANDER1:
            m_Team_QuestStatus[team][1]++;
            reputation = 1;
            if (m_Team_QuestStatus[team][1] == 120)
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
            break;
        case BG_AV_QUEST_A_COMMANDER2:
        case BG_AV_QUEST_H_COMMANDER2:
            m_Team_QuestStatus[team][2]++;
            reputation = 2;
            if (m_Team_QuestStatus[team][2] == 60)
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
            break;
        case BG_AV_QUEST_A_COMMANDER3:
        case BG_AV_QUEST_H_COMMANDER3:
            m_Team_QuestStatus[team][3]++;
            reputation = 5;
            RewardReputationToTeam(team, 1, player->GetTeam());
            if (m_Team_QuestStatus[team][1] == 30)
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
            break;
        case BG_AV_QUEST_A_BOSS1:
        case BG_AV_QUEST_H_BOSS1:
            m_Team_QuestStatus[team][4] += 4;               // there are 2 quests where you can turn in 5 or 1 item.. ( + 4 cause +1 will be done some lines below)
            reputation = 4;
        case BG_AV_QUEST_A_BOSS2:
        case BG_AV_QUEST_H_BOSS2:
            m_Team_QuestStatus[team][4]++;
            reputation += 1;
            if (m_Team_QuestStatus[team][4] >= 200)
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
            break;
        case BG_AV_QUEST_A_NEAR_MINE:
        case BG_AV_QUEST_H_NEAR_MINE:
            m_Team_QuestStatus[team][5]++;
            reputation = 2;
            if (m_Team_QuestStatus[team][5] == 28)
            {
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
                if (m_Team_QuestStatus[team][6] == 7)
                    sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here - ground assault ready", questid);
            }
            break;
        case BG_AV_QUEST_A_OTHER_MINE:
        case BG_AV_QUEST_H_OTHER_MINE:
            m_Team_QuestStatus[team][6]++;
            reputation = 3;
            if (m_Team_QuestStatus[team][6] == 7)
            {
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
                if (m_Team_QuestStatus[team][5] == 20)
                    sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here - ground assault ready", questid);
            }
            break;
        case BG_AV_QUEST_A_RIDER_HIDE:
        case BG_AV_QUEST_H_RIDER_HIDE:
            m_Team_QuestStatus[team][7]++;
            reputation = 1;
            if (m_Team_QuestStatus[team][7] == 25)
            {
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
                if (m_Team_QuestStatus[team][8] == 25)
                    sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here - rider assault ready", questid);
            }
            break;
        case BG_AV_QUEST_A_RIDER_TAME:
        case BG_AV_QUEST_H_RIDER_TAME:
            m_Team_QuestStatus[team][8]++;
            reputation = 1;
            if (m_Team_QuestStatus[team][8] == 25)
            {
                sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here", questid);
                if (m_Team_QuestStatus[team][7] == 25)
                    sLog.outDebug("BattleGroundAV: Quest %i completed (need to implement some events here - rider assault ready", questid);
            }
            break;
        default:
            sLog.outDebug("BattleGroundAV: Quest %i completed but is not interesting for us", questid);
            return;
            break;
    }
    if (reputation)
        RewardReputationToTeam((player->GetTeam() == ALLIANCE) ? 730 : 729, reputation, player->GetTeam());
}

void BattleGroundAV::UpdateScore(uint32 team, int32 points )
{
    // note: to remove reinforcements points must be negative, for adding reinforcements points must be positive
    assert( team == BG_TEAM_ALLIANCE || team == BG_TEAM_HORDE);
    uint8 teamindex = GetTeamIndexByTeamId(team);
    m_TeamScores[teamindex] += points;                      // m_TeamScores is int32 - so no problems here

    if (points < 0)
    {
        if (m_TeamScores[teamindex] < 1)
        {
            m_TeamScores[teamindex] = 0;
            // other team will win:
            EndBattleGround((team == ALLIANCE)? HORDE : ALLIANCE);
        }
        else if (!m_IsInformedNearLose[teamindex] && m_TeamScores[teamindex] < BG_AV_SCORE_NEAR_LOSE)
        {
            SendMessageToAll((teamindex == BG_TEAM_HORDE) ? LANG_BG_AV_H_NEAR_LOSE : LANG_BG_AV_A_NEAR_LOSE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            PlaySoundToAll(BG_AV_SOUND_NEAR_LOSE);
            m_IsInformedNearLose[teamindex] = true;
        }
    }
    // must be called here, else it could display a negative value
    UpdateWorldState(((teamindex == BG_TEAM_HORDE) ? BG_AV_Horde_Score : BG_AV_Alliance_Score), m_TeamScores[teamindex]);
}

void BattleGroundAV::OnObjectDBLoad(Creature* creature)
{
    uint32 level = creature->getLevel();
    if (level != 0)
        level += GetMaxLevel() - 60;                        // maybe we can do this more generic for custom level - range.. actually it's ok
    creature->SetLevel(level);
    BattleGround::OnObjectDBLoad(creature);
}

void BattleGroundAV::OnCreatureRespawn(Creature* creature)
{
    uint32 level = creature->getLevel();
    if (level != 0)
        level += GetMaxLevel() - 60;                        // maybe we can do this more generic for custom level - range.. actually it's ok
    creature->SetLevel(level);
}

void BattleGroundAV::Update(uint32 diff)
{
    BattleGround::Update(diff);
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    // add points from mine owning, and look if the neutral team can reclaim the mine
    for(uint8 mine = 0; mine <2; mine++)
    {
        if (m_Mine_Owner[mine] == BG_TEAM_ALLIANCE || m_Mine_Owner[mine] == BG_TEAM_HORDE)
        {
            m_Mine_Timer[mine] -=diff;
            if (m_Mine_Timer[mine] <= 0)
            {
                UpdateScore(m_Mine_Owner[mine], 1);
                m_Mine_Timer[mine] = BG_AV_MINE_TICK_TIMER;
            }

            if (m_Mine_Reclaim_Timer[mine] > diff)
                m_Mine_Reclaim_Timer[mine] -= diff;
            else
                ChangeMineOwner(mine, BG_AV_NEUTRAL_TEAM);
        }
    }

    // looks for all timers of the nodes and destroy the building (for graveyards the building wont get destroyed, it goes just to the other team
    for(BG_AV_Nodes i = BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; ++i)
    {
        if (m_Nodes[i].State == POINT_ASSAULTED)
        {
            if (m_Nodes[i].Timer > diff)
                m_Nodes[i].Timer -= diff;
            else
                 EventPlayerDestroyedPoint(i);
        }
    }
}

void BattleGroundAV::StartingEventCloseDoors()
{
    sLog.outDebug("BattleGroundAV: entering state STATUS_WAIT_JOIN ...");
}

void BattleGroundAV::StartingEventOpenDoors()
{
    UpdateWorldState(BG_AV_SHOW_H_SCORE, 1);
    UpdateWorldState(BG_AV_SHOW_A_SCORE, 1);

    OpenDoorEvent(BG_EVENT_DOOR_A);
    OpenDoorEvent(BG_EVENT_DOOR_H);
}

void BattleGroundAV::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    // create score and add it to map, default values are set in constructor
    BattleGroundAVScore* sc = new BattleGroundAVScore;
    m_PlayerScores[plr->GetGUID()] = sc;
}

void BattleGroundAV::EndBattleGround(uint32 winner)
{
    // calculate bonuskills for both teams:
    uint32 tower_survived[BG_TEAMS_COUNT]  = {0, 0};
    uint32 graves_owned[BG_TEAMS_COUNT]    = {0, 0};
    uint32 mines_owned[BG_TEAMS_COUNT]     = {0, 0};
    // towers all not destroyed:
    for(BG_AV_Nodes i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; ++i)
        if (m_Nodes[i].State == POINT_CONTROLLED)
            if (m_Nodes[i].TotalOwner == BG_TEAM_ALLIANCE)
                ++tower_survived[BG_TEAM_ALLIANCE];
    for(BG_AV_Nodes i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)
        if (m_Nodes[i].State == POINT_CONTROLLED)
            if (m_Nodes[i].TotalOwner == BG_TEAM_HORDE)
                ++tower_survived[BG_TEAM_HORDE];

    // graves all controlled
    for(BG_AV_Nodes i = BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; ++i)
    {
        if (m_Nodes[i].State == POINT_CONTROLLED)
        {
            if (m_Nodes[i].Owner == BG_TEAM_ALLIANCE)
                ++graves_owned[BG_TEAM_ALLIANCE];
            else if (m_Nodes[i].Owner == BG_TEAM_HORDE)
                ++graves_owned[BG_TEAM_HORDE];
        }
    }
    // mines owned
    if (m_Mine_Owner[BG_AV_SOUTH_MINE] == BG_TEAM_ALLIANCE)
        ++mines_owned[BG_TEAM_ALLIANCE];
    else if (m_Mine_Owner[BG_AV_SOUTH_MINE] == BG_TEAM_HORDE)
        ++mines_owned[BG_TEAM_HORDE];
    if (m_Mine_Owner[BG_AV_NORTH_MINE] == BG_TEAM_ALLIANCE)
        ++mines_owned[BG_TEAM_ALLIANCE];
    else if (m_Mine_Owner[BG_AV_NORTH_MINE] == BG_TEAM_HORDE)
        ++mines_owned[BG_TEAM_HORDE];

    // now we have the values give the honor/reputation to the teams:
    uint32 team[2]      = { ALLIANCE, HORDE };
    uint32 faction[2]   = { 730, 729 };
    for(uint32 i = 0; i < BG_TEAMS_COUNT; i++)
    {
        if (tower_survived[i])
        {
            RewardReputationToTeam(faction[i], tower_survived[i] * m_RepSurviveTower, team[i]);
            RewardHonorToTeam(GetBonusHonorFromKill(tower_survived[i] * BG_AV_KILL_SURVIVING_TOWER), team[i]);
        }
        sLog.outDebug("BattleGroundAV: EndbattleGround: bgteam: %u towers:%u honor:%u rep:%u", i, tower_survived[i], GetBonusHonorFromKill(tower_survived[i] * BG_AV_KILL_SURVIVING_TOWER), tower_survived[i] * BG_AV_REP_SURVIVING_TOWER);
        if (graves_owned[i])
            RewardReputationToTeam(faction[i], graves_owned[i] * m_RepOwnedGrave, team[i]);
        if (mines_owned[i])
            RewardReputationToTeam(faction[i], mines_owned[i] * m_RepOwnedMine, team[i]);
        // captain survived?:
        if (!IsActiveEvent(BG_AV_NodeEventCaptainDead_A + GetTeamIndexByTeamId(team[i]), 0))
        {
            RewardReputationToTeam(faction[i], m_RepSurviveCaptain, team[i]);
            RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_SURVIVING_CAPTAIN), team[i]);
        }
    }

    // both teams:
    if (m_HonorMapComplete)
    {
        RewardHonorToTeam(m_HonorMapComplete, ALLIANCE);
        RewardHonorToTeam(m_HonorMapComplete, HORDE);
    }
    BattleGround::EndBattleGround(winner);
}

void BattleGroundAV::RemovePlayer(Player* plr,uint64 /*guid*/)
{
}

void BattleGroundAV::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 SpellId = 0;
    switch(Trigger)
    {
        case 95:
        case 2608:
            if (Source->GetTeam() != ALLIANCE)
                Source->GetSession()->SendAreaTriggerMessage("Only The Alliance can use that portal");
            else
                Source->LeaveBattleground();
            break;
        case 2606:
            if (Source->GetTeam() != HORDE)
                Source->GetSession()->SendAreaTriggerMessage("Only The Horde can use that portal");
            else
                Source->LeaveBattleground();
            break;
        case 3326:
        case 3327:
        case 3328:
        case 3329:
        case 3330:
        case 3331:
            //Source->Unmount();
            break;
        default:
            sLog.outDebug("BattleGroundAV: WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
//            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }

    if (SpellId)
        Source->CastSpell(Source, SpellId, true);
}

void BattleGroundAV::UpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{

    BattleGroundScoreMap::iterator itr = m_PlayerScores.find(Source->GetGUID());
    if(itr == m_PlayerScores.end())                         // player not found...
        return;

    switch(type)
    {
        case SCORE_GRAVEYARDS_ASSAULTED:
            ((BattleGroundAVScore*)itr->second)->GraveyardsAssaulted += value;
            break;
        case SCORE_GRAVEYARDS_DEFENDED:
            ((BattleGroundAVScore*)itr->second)->GraveyardsDefended += value;
            break;
        case SCORE_TOWERS_ASSAULTED:
            ((BattleGroundAVScore*)itr->second)->TowersAssaulted += value;
            break;
        case SCORE_TOWERS_DEFENDED:
            ((BattleGroundAVScore*)itr->second)->TowersDefended += value;
            break;
        case SCORE_SECONDARY_OBJECTIVES:
            ((BattleGroundAVScore*)itr->second)->SecondaryObjectives += value;
            break;
        default:
            BattleGround::UpdatePlayerScore(Source, type, value);
            break;
    }
}

void BattleGroundAV::EventPlayerDestroyedPoint(BG_AV_Nodes node)
{

    sLog.outDebug("BattleGroundAV: player destroyed point node %i", node);

    // despawn banner
    DestroyNode(node);
    PopulateNode(node);
    UpdateNodeWorldState(node);

    uint32 owner = m_Nodes[node].Owner;
    if (IsTower(node))
    {
        uint8 tmp = node - BG_AV_NODES_DUNBALDAR_SOUTH;
        // despawn marshal (one of those guys protecting the boss)
        SpawnEvent(BG_AV_MARSHAL_A_SOUTH + tmp, 0, false);

        UpdateScore(owner^0x1, (-1) * BG_AV_RES_TOWER);
        RewardReputationToTeam((owner == BG_TEAM_ALLIANCE) ? 730 : 729, m_RepTowerDestruction, owner);
        RewardHonorToTeam(GetBonusHonorFromKill(BG_AV_KILL_TOWER), owner);
        SendYell2ToAll(LANG_BG_AV_TOWER_TAKEN, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0), GetNodeName(node), ( owner == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY : LANG_BG_AV_HORDE);
    }
    else if (IsGrave(node))
    {
        SendYell2ToAll(LANG_BG_AV_GRAVE_TAKEN, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0), GetNodeName(node), ( owner == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY : LANG_BG_AV_HORDE);
    }
}

void BattleGroundAV::ChangeMineOwner(uint8 mine, uint32 team)
{
    m_Mine_Timer[mine] = BG_AV_MINE_TICK_TIMER;
    // TODO implement quest 7122
    // mine=0 northmine, mine=1 southmine
    // TODO changing the owner should result in setting respawntime to infinite for current creatures (they should fight the new ones), spawning new mine owners creatures and changing the chest - objects so that the current owning team can use them
    assert(mine == BG_AV_NORTH_MINE || mine == BG_AV_SOUTH_MINE);
    if (m_Mine_Owner[mine] == team)
        return;

    if (team != BG_TEAM_ALLIANCE && team != BG_TEAM_HORDE)
        team = BG_AV_NEUTRAL_TEAM;

    m_Mine_PrevOwner[mine] = m_Mine_Owner[mine];
    m_Mine_Owner[mine] = team;
    uint32 index;

    SendMineWorldStates(mine);

    SpawnEvent(BG_AV_MINE_EVENT + mine, team, true);
    SpawnEvent(BG_AV_MINE_BOSSES + mine, team, true);

    // because the gameobjects in this mine have changed, update all surrounding players:
    // TODO: add gameobject - update code (currently this is done in a hacky way)
    if (team == BG_TEAM_ALLIANCE || team == BG_TEAM_HORDE)
    {
        PlaySoundToAll((team == BG_TEAM_ALLIANCE) ? BG_AV_SOUND_ALLIANCE_GOOD : BG_AV_SOUND_HORDE_GOOD);
        m_Mine_Reclaim_Timer[mine] = BG_AV_MINE_RECLAIM_TIMER;
        SendYell2ToAll(LANG_BG_AV_MINE_TAKEN , LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0),
            (team == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY : LANG_BG_AV_HORDE,
            (mine == BG_AV_NORTH_MINE) ? LANG_BG_AV_MINE_NORTH : LANG_BG_AV_MINE_SOUTH);
    }
}

bool BattleGroundAV::PlayerCanDoMineQuest(int32 GOId, uint32 team)
{
    if (GOId == BG_AV_OBJECTID_MINE_N)
         return (m_Mine_Owner[BG_AV_NORTH_MINE] == GetTeamIndexByTeamId(team));
    if (GOId == BG_AV_OBJECTID_MINE_S)
         return (m_Mine_Owner[BG_AV_SOUTH_MINE] == GetTeamIndexByTeamId(team));
    return true;                                            // cause it's no mine'object it is ok if this is true
}

/// will spawn and despawn creatures around a node
/// more a wrapper around spawnevent cause graveyards are special
void BattleGroundAV::PopulateNode(BG_AV_Nodes node)
{
    uint32 team = m_Nodes[node].Owner;
    if (IsGrave(node) && team != BG_AV_NEUTRAL_TEAM)
    {
        uint32 graveDefenderType;
        if (m_Team_QuestStatus[team][0] < 500 )
            graveDefenderType = 0;
        else if (m_Team_QuestStatus[team][0] < 1000 )
            graveDefenderType = 1;
        else if (m_Team_QuestStatus[team][0] < 1500 )
            graveDefenderType = 2;
        else
            graveDefenderType = 3;

        if (m_Nodes[node].State == POINT_CONTROLLED) // we can spawn the current owner event
            SpawnEvent(BG_AV_NODES_MAX + node, team * BG_AV_MAX_GRAVETYPES + graveDefenderType, true);
        else // we despawn the event from the prevowner
            SpawnEvent(BG_AV_NODES_MAX + node, m_Nodes[node].PrevOwner * BG_AV_MAX_GRAVETYPES + graveDefenderType, false);
    }
    SpawnEvent(node, (team * BG_AV_MAX_STATES) + m_Nodes[node].State, true);
}

const BG_AV_Nodes BattleGroundAV::GetNodeThroughPlayerPosition(Player* plr)
{
    for(uint32 i=0; i<=BG_AV_NODES_MAX; ++i)
    {
        if (plr->IsWithinDist2d(BG_AV_NodePositions[i][0], BG_AV_NodePositions[i][1], BG_AV_MAX_NODE_DISTANCE))
            return BG_AV_Nodes(i);
    }
    sLog.outError("BattleGroundAV: player isn't near to any node maybe a cheater? or you spawned a banner not near to a node - or there is a bug in the code");
    return BG_AV_NODES_ERROR;
}

/// called when using a banner
void BattleGroundAV::EventPlayerClickedOnFlag(Player *source, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
    sLog.outDebug("BattleGroundAV: using gameobject %i", target_obj->GetEntry());
    BattleGroundEventIdx eventId = sBattleGroundMgr.GetGameObjectEventIndex(target_obj->GetDBTableGUIDLow());
    if (eventId.event1 == BG_EVENT_NONE)
        return;
    switch(eventId.event2 % BG_AV_MAX_STATES)
    {
        case POINT_CONTROLLED:
            EventPlayerAssaultsPoint(source);
            break;
        case POINT_ASSAULTED:
            EventPlayerDefendsPoint(source);
            break;
        default:
            break;
    }
}

void BattleGroundAV::EventPlayerDefendsPoint(Player* player)
{
    assert(GetStatus() == STATUS_IN_PROGRESS);
    BG_AV_Nodes node = GetNodeThroughPlayerPosition(player);
    if (node == BG_AV_NODES_ERROR)
        return;

    uint32 team = GetTeamIndexByTeamId(player->GetTeam());

    if (m_Nodes[node].Owner == team || m_Nodes[node].State != POINT_ASSAULTED)
        return;
    if( m_Nodes[node].TotalOwner == BG_AV_NEUTRAL_TEAM )    // initial snowfall capture
    {
        // until snowfall doesn't belong to anyone it is better handled in assault - code (best would be to have a special function
        // for neutral nodes.. but doing this just for snowfall will be a bit to much i think
        assert(node == BG_AV_NODES_SNOWFALL_GRAVE);         // currently the only neutral grave
        EventPlayerAssaultsPoint(player);
        return;
    }
    sLog.outDebug("BattleGroundAV: player defends node: %i", node);
    if (m_Nodes[node].PrevOwner != team)
    {
        sLog.outError("BattleGroundAV: player defends point which doesn't belong to his team %i", node);
        return;
    }

    DefendNode(node, team);                                 // set the right variables for nodeinfo
    PopulateNode(node);                                     // spawn node-creatures (defender for example)
    UpdateNodeWorldState(node);                             // send new mapicon to the player

    if (IsTower(node))
    {
        SendYell2ToAll( LANG_BG_AV_TOWER_DEFENDED, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0),
            GetNodeName(node),
            ( team == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY:LANG_BG_AV_HORDE);
        UpdatePlayerScore(player, SCORE_TOWERS_DEFENDED, 1);
        PlaySoundToAll(BG_AV_SOUND_BOTH_TOWER_DEFEND);
    }
    else if (IsGrave(node))
    {
        SendYell2ToAll(LANG_BG_AV_GRAVE_DEFENDED, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0),
            GetNodeName(node),
            ( team == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY:LANG_BG_AV_HORDE);
        UpdatePlayerScore(player, SCORE_GRAVEYARDS_DEFENDED, 1);
    // update the statistic for the defending player
        PlaySoundToAll((team == BG_TEAM_ALLIANCE)?BG_AV_SOUND_ALLIANCE_GOOD:BG_AV_SOUND_HORDE_GOOD);
    }
}

void BattleGroundAV::EventPlayerAssaultsPoint(Player* player)
{
    // TODO implement quest 7101, 7081
    BG_AV_Nodes node = GetNodeThroughPlayerPosition(player);
    if (node == BG_AV_NODES_ERROR)
        return;

    m_assault_in_progress = node;                           // to disable getclosestgrave for this node

    uint32 team  = GetTeamIndexByTeamId(player->GetTeam());
    sLog.outDebug("BattleGroundAV: player assaults node %i", node);
    if (m_Nodes[node].Owner == team || team == m_Nodes[node].TotalOwner)
        return;

    // creatures from graveyard will be teleported away in
    // spawnbgobject-function, when spiritguide is despawned
    // - so n need to check here
    AssaultNode(node, team);                          // update nodeinfo variables
    UpdateNodeWorldState(node);                             // send mapicon
    PopulateNode(node);

    if (IsTower(node))
    {
        SendYell2ToAll(LANG_BG_AV_TOWER_ASSAULTED, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0),
            GetNodeName(node),
            ( team == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY:LANG_BG_AV_HORDE);
        UpdatePlayerScore(player, SCORE_GRAVEYARDS_ASSAULTED, 1);
    }
    else if (IsGrave(node))
    {
        SendYell2ToAll(LANG_BG_AV_GRAVE_ASSAULTED, LANG_UNIVERSAL, GetSingleCreatureGuid(BG_AV_HERALD, 0),
            GetNodeName(node),
            ( team == BG_TEAM_ALLIANCE ) ? LANG_BG_AV_ALLY:LANG_BG_AV_HORDE);
        // update the statistic for the assaulting player
        UpdatePlayerScore(player, SCORE_GRAVEYARDS_ASSAULTED, 1);
    }

    PlaySoundToAll((team == BG_TEAM_ALLIANCE) ? BG_AV_SOUND_ALLIANCE_ASSAULTS : BG_AV_SOUND_HORDE_ASSAULTS);
    m_assault_in_progress = BG_AV_NODES_ERROR;
}

void BattleGroundAV::FillInitialWorldStates(WorldPacket& data)
{
    bool stateok;
    for (uint32 i = BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; ++i)
    {
        for (uint8 j = 0; j < BG_AV_MAX_STATES; j++)
        {
            stateok = (m_Nodes[i].State == j);
            data << uint32(BG_AV_NodeWorldStates[i][GetWorldStateType(j, BG_TEAM_ALLIANCE)]) << uint32((m_Nodes[i].Owner == BG_TEAM_ALLIANCE && stateok) ? 1 : 0);
            data << uint32(BG_AV_NodeWorldStates[i][GetWorldStateType(j, BG_TEAM_HORDE)]) << uint32((m_Nodes[i].Owner == BG_TEAM_HORDE && stateok) ? 1 : 0);
        }
    }

    if( m_Nodes[BG_AV_NODES_SNOWFALL_GRAVE].Owner == BG_AV_NEUTRAL_TEAM )   // cause neutral teams aren't handled generic
        data << uint32(AV_SNOWFALL_N) << uint32(1);

    data << uint32(BG_AV_Alliance_Score)  << uint32(m_TeamScores[BG_TEAM_ALLIANCE]);
    data << uint32(BG_AV_Horde_Score) << uint32(m_TeamScores[BG_TEAM_HORDE]);
    if( GetStatus() == STATUS_IN_PROGRESS )                 // only if game is running the teamscores are displayed
    {
        data << uint32(BG_AV_SHOW_A_SCORE) << uint32(1);
        data << uint32(BG_AV_SHOW_H_SCORE) << uint32(1);
    }
    else
    {
        data << uint32(BG_AV_SHOW_A_SCORE) << uint32(0);
        data << uint32(BG_AV_SHOW_H_SCORE) << uint32(0);
    }
    SendMineWorldStates(BG_AV_NORTH_MINE);
    SendMineWorldStates(BG_AV_SOUTH_MINE);
}

void BattleGroundAV::UpdateNodeWorldState(BG_AV_Nodes node)
{
    UpdateWorldState(BG_AV_NodeWorldStates[node][GetWorldStateType(m_Nodes[node].State,m_Nodes[node].Owner)], 1);
    if( m_Nodes[node].PrevOwner == BG_AV_NEUTRAL_TEAM )     // currently only snowfall is supported as neutral node
        UpdateWorldState(AV_SNOWFALL_N, 0);
    else
        UpdateWorldState(BG_AV_NodeWorldStates[node][GetWorldStateType(m_Nodes[node].PrevState,m_Nodes[node].PrevOwner)], 0);
}

void BattleGroundAV::SendMineWorldStates(uint32 mine)
{
    assert(mine == BG_AV_NORTH_MINE || mine == BG_AV_SOUTH_MINE);
    assert(m_Mine_PrevOwner[mine] == BG_TEAM_ALLIANCE || m_Mine_PrevOwner[mine] == BG_TEAM_HORDE || m_Mine_PrevOwner[mine] == BG_AV_NEUTRAL_TEAM);
    assert(m_Mine_Owner[mine] == BG_TEAM_ALLIANCE || m_Mine_Owner[mine] == BG_TEAM_HORDE || m_Mine_Owner[mine] == BG_AV_NEUTRAL_TEAM);

    UpdateWorldState(BG_AV_MineWorldStates[mine][m_Mine_Owner[mine]], 1);
    if (m_Mine_Owner[mine] != m_Mine_PrevOwner[mine])
        UpdateWorldState(BG_AV_MineWorldStates[mine][m_Mine_PrevOwner[mine]], 0);
}

WorldSafeLocsEntry const* BattleGroundAV::GetClosestGraveYard(Player *plr)
{
    float x = plr->GetPositionX();
    float y = plr->GetPositionY();
    uint32 team = GetTeamIndexByTeamId(plr->GetTeam());
    WorldSafeLocsEntry const* good_entry = NULL;
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        // Is there any occupied node for this team?
        float mindist = 9999999.0f;
        for(uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
        {
            if (i == m_assault_in_progress)
                continue;
            if (m_Nodes[i].Owner != team || m_Nodes[i].State != POINT_CONTROLLED)
                continue;
            WorldSafeLocsEntry const * entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[i] );
            if (!entry)
                continue;
            float dist = (entry->x - x) * (entry->x - x) + (entry->y - y) * (entry->y - y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
    }
    // If not, place ghost in the starting-cave
    if (!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[team + 7] );

    return good_entry;
}

bool BattleGroundAV::SetupBattleGround()
{
    return true;
}

uint32 BattleGroundAV::GetNodeName(BG_AV_Nodes node)
{
    switch (node)
    {
        case BG_AV_NODES_FIRSTAID_STATION:  return LANG_BG_AV_NODE_GRAVE_STORM_AID;
        case BG_AV_NODES_DUNBALDAR_SOUTH:   return LANG_BG_AV_NODE_TOWER_DUN_S;
        case BG_AV_NODES_DUNBALDAR_NORTH:   return LANG_BG_AV_NODE_TOWER_DUN_N;
        case BG_AV_NODES_STORMPIKE_GRAVE:   return LANG_BG_AV_NODE_GRAVE_STORMPIKE;
        case BG_AV_NODES_ICEWING_BUNKER:    return LANG_BG_AV_NODE_TOWER_ICEWING;
        case BG_AV_NODES_STONEHEART_GRAVE:  return LANG_BG_AV_NODE_GRAVE_STONE;
        case BG_AV_NODES_STONEHEART_BUNKER: return LANG_BG_AV_NODE_TOWER_STONE;
        case BG_AV_NODES_SNOWFALL_GRAVE:    return LANG_BG_AV_NODE_GRAVE_SNOW;
        case BG_AV_NODES_ICEBLOOD_TOWER:    return LANG_BG_AV_NODE_TOWER_ICE;
        case BG_AV_NODES_ICEBLOOD_GRAVE:    return LANG_BG_AV_NODE_GRAVE_ICE;
        case BG_AV_NODES_TOWER_POINT:       return LANG_BG_AV_NODE_TOWER_POINT;
        case BG_AV_NODES_FROSTWOLF_GRAVE:   return LANG_BG_AV_NODE_GRAVE_FROST;
        case BG_AV_NODES_FROSTWOLF_ETOWER:  return LANG_BG_AV_NODE_TOWER_FROST_E;
        case BG_AV_NODES_FROSTWOLF_WTOWER:  return LANG_BG_AV_NODE_TOWER_FROST_W;
        case BG_AV_NODES_FROSTWOLF_HUT:     return LANG_BG_AV_NODE_GRAVE_FROST_HUT;
        default: return 0; break;
    }
}

void BattleGroundAV::AssaultNode(BG_AV_Nodes node, uint32 team)
{
    assert(team<3);
    assert(m_Nodes[node].TotalOwner != team);
    assert(m_Nodes[node].Owner != team);
    assert(m_Nodes[node].State != POINT_ASSAULTED || !m_Nodes[node].TotalOwner ); // only assault an assaulted node if no totalowner exists
    // the timer gets another time, if the previous owner was 0 == Neutral
    m_Nodes[node].Timer      = (m_Nodes[node].PrevOwner != BG_AV_NEUTRAL_TEAM) ? BG_AV_CAPTIME : BG_AV_SNOWFALL_FIRSTCAP;
    m_Nodes[node].PrevOwner  = m_Nodes[node].Owner;
    m_Nodes[node].Owner      = team;
    m_Nodes[node].PrevState  = m_Nodes[node].State;
    m_Nodes[node].State      = POINT_ASSAULTED;
}

void BattleGroundAV::DestroyNode(BG_AV_Nodes node)
{
    assert(m_Nodes[node].State == POINT_ASSAULTED);

    m_Nodes[node].TotalOwner = m_Nodes[node].Owner;
    m_Nodes[node].PrevOwner  = m_Nodes[node].Owner;
    m_Nodes[node].PrevState  = m_Nodes[node].State;
    m_Nodes[node].State      = POINT_CONTROLLED;
    m_Nodes[node].Timer      = 0;
}

void BattleGroundAV::InitNode(BG_AV_Nodes node, uint32 team, bool tower)
{
    assert(team<3);
    m_Nodes[node].TotalOwner = team;
    m_Nodes[node].Owner      = team;
    m_Nodes[node].PrevOwner  = team;
    m_Nodes[node].State      = POINT_CONTROLLED;
    m_Nodes[node].PrevState  = m_Nodes[node].State;
    m_Nodes[node].State      = POINT_CONTROLLED;
    m_Nodes[node].Timer      = 0;
    m_Nodes[node].Tower      = tower;
    for (int i = 0; i < 3*BG_AV_MAX_STATES; ++i)
        m_ActiveEvents[node][i] = false;
    m_ActiveEvents[node][team * BG_AV_MAX_STATES + m_Nodes[node].State] = true;
    if (IsGrave(node))
    {
        for (int i = 0; i < BG_TEAMS_COUNT * BG_AV_MAX_GRAVETYPES; ++i)
            m_ActiveEvents[node + BG_AV_NODES_MAX][i] = false;
        m_ActiveEvents[node + BG_AV_NODES_MAX][team * BG_AV_MAX_GRAVETYPES] = true;
    }
}

void BattleGroundAV::DefendNode(BG_AV_Nodes node, uint32 team)
{
    assert(team<3);
    assert(m_Nodes[node].TotalOwner == team);
    assert(m_Nodes[node].Owner != team);
    assert(m_Nodes[node].State != POINT_CONTROLLED);
    m_Nodes[node].PrevOwner  = m_Nodes[node].Owner;
    m_Nodes[node].Owner      = team;
    m_Nodes[node].PrevState  = m_Nodes[node].State;
    m_Nodes[node].State      = POINT_CONTROLLED;
    m_Nodes[node].Timer      = 0;
}

void BattleGroundAV::Reset()
{
    BattleGround::Reset();
    // set the reputation and honor variables:
    bool isBGWeekend = false;                               // TODO FIXME - call sBattleGroundMgr.IsBGWeekend(m_TypeID); - you must also implement that call!

    m_HonorMapComplete    = (isBGWeekend) ? BG_AV_KILL_MAP_COMPLETE_HOLIDAY : BG_AV_KILL_MAP_COMPLETE;
    m_RepTowerDestruction = (isBGWeekend) ? BG_AV_REP_TOWER_HOLIDAY         : BG_AV_REP_TOWER;
    m_RepCaptain          = (isBGWeekend) ? BG_AV_REP_CAPTAIN_HOLIDAY       : BG_AV_REP_CAPTAIN;
    m_RepBoss             = (isBGWeekend) ? BG_AV_REP_BOSS_HOLIDAY          : BG_AV_REP_BOSS;
    m_RepOwnedGrave       = (isBGWeekend) ? BG_AV_REP_OWNED_GRAVE_HOLIDAY   : BG_AV_REP_OWNED_GRAVE;
    m_RepSurviveCaptain   = (isBGWeekend) ? BG_AV_REP_SURVIVING_CAPTAIN_HOLIDAY : BG_AV_REP_SURVIVING_CAPTAIN;
    m_RepSurviveTower     = (isBGWeekend) ? BG_AV_REP_SURVIVING_TOWER_HOLIDAY : BG_AV_REP_SURVIVING_TOWER;

    m_assault_in_progress = BG_AV_NODES_ERROR;
    for(uint8 i = 0; i < BG_TEAMS_COUNT; i++)
    {
        for(uint8 j = 0; j < 9; j++)                        // 9 quests getting tracked
            m_Team_QuestStatus[i][j] = 0;
        m_TeamScores[i]         = BG_AV_SCORE_INITIAL_POINTS;
        m_IsInformedNearLose[i] = false;
        m_ActiveEvents[BG_AV_NodeEventCaptainDead_A + i][0] = false;
    }

    // cycle through both mines
    for(uint8 i = 0; i < 2; i++)
    {
        m_Mine_Owner[i] = BG_AV_NEUTRAL_TEAM;
        m_Mine_PrevOwner[i] = m_Mine_Owner[i];
        for (uint8 j=0; j<BG_TEAMS_COUNT; j++)
        {
            m_ActiveEvents[BG_AV_MINE_EVENT + i][j]   = false;
            m_ActiveEvents[BG_AV_MINE_BOSSES+ i][j]   = false;
        }
        m_ActiveEvents[BG_AV_MINE_BOSSES+ i][BG_AV_NEUTRAL_TEAM] = true;
        m_ActiveEvents[BG_AV_MINE_EVENT + i][BG_AV_NEUTRAL_TEAM] = true;
        m_Mine_Timer[i] = BG_AV_MINE_TICK_TIMER;
    }

    m_ActiveEvents[BG_AV_CAPTAIN_A][0] = true;
    m_ActiveEvents[BG_AV_CAPTAIN_H][0] = true;
    m_ActiveEvents[BG_AV_HERALD][0] = true;
    m_ActiveEvents[BG_AV_BOSS_A][0] = true;
    m_ActiveEvents[BG_AV_BOSS_H][0] = true;
    for(BG_AV_Nodes i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)   // towers
        m_ActiveEvents[BG_AV_MARSHAL_A_SOUTH + i - BG_AV_NODES_DUNBALDAR_SOUTH][0] = true;

    for(BG_AV_Nodes i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_STONEHEART_GRAVE; ++i)   // alliance graves
        InitNode(i, BG_TEAM_ALLIANCE, false);
    for(BG_AV_Nodes i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; ++i)   // alliance towers
        InitNode(i, BG_TEAM_ALLIANCE, true);

    for(BG_AV_Nodes i = BG_AV_NODES_ICEBLOOD_GRAVE; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)        // horde graves
        InitNode(i, BG_TEAM_HORDE, false);
    for(BG_AV_Nodes i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)     // horde towers
        InitNode(i, BG_TEAM_HORDE, true);

    InitNode(BG_AV_NODES_SNOWFALL_GRAVE, BG_AV_NEUTRAL_TEAM, false);                            // give snowfall neutral owner

}
