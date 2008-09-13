/*
 * Copyright (C) 2005-2008 MaNGOS <http://getmangos.com/>
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
#include "BattleGroundAV.h"
#include "Creature.h"
#include "Chat.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Language.h"

BattleGroundAV::BattleGroundAV()
{
    m_BgObjects.resize(BG_AV_OBJECT_MAX);
    m_BgCreatures.resize(BG_AV_CREATURES_MAX);
}

BattleGroundAV::~BattleGroundAV()
{

}

void BattleGroundAV::HandleKillPlayer(Player *player, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 teamIndex = GetTeamIndexByTeamId(player->GetTeam());
	if(teamIndex==0)
        m_Team_Scores[0]--;
    else m_Team_Scores[1]--;

	UpdateScore();
}

void BattleGroundAV::HandleKillUnit(Creature *unit, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    uint32 ressources = 0;
    uint32 reputation = 0;
    uint32 honor = 0;
    //check for general/captain mobs, generals end battleground and captains decrease tickets by 100
    switch(unit->GetEntry())
    {
        case 11948:
            reputation = 389;
            honor = 80;
            EndBattleGround(HORDE);
            break;
        case 11946:
            reputation = 389;
            honor = 80;
            EndBattleGround(ALLIANCE);
            break;
        case 11949:
            reputation = 125;
            honor = 60;
            ressources = 100;
            break;
        case 11947:
            reputation = 125;
            honor = 60;
            ressources = 100;
            break;
        default:
            ressources = 0;
            honor = 0;
            break;
    }
    uint32 faction = 0;
	if(GetTeamIndexByTeamId(killer->GetTeam()) == 0)
        m_Team_Scores[1] -= ressources;
    else m_Team_Scores[0] -= ressources;
    if(killer->GetTeam() == ALLIANCE)
        faction = 730;
    else faction = 729;

    //RewardReputation(faction,reputation, killer->GetTeam(),killer);
	UpdateScore();
}

void BattleGroundAV::UpdateScore()
{
	uint32 TAScore = (m_Team_Scores[0] < 0)?0:m_Team_Scores[0];
	uint32 THScore = (m_Team_Scores[1] < 0)?0:m_Team_Scores[1];


	UpdateWorldState(AV_Alliance_Score, TAScore);
	UpdateWorldState(AV_Horde_Score, THScore);
}

void BattleGroundAV::InitWorldStates()
{
		UpdateWorldState(AV_Alliance_Score, m_Team_Scores[0]);
		UpdateWorldState(AV_Horde_Score, m_Team_Scores[1]);
		UpdateWorldState(AV_SHOW_H_SCORE, 1);
		UpdateWorldState(AV_SHOW_A_SCORE, 1);
}

void BattleGroundAV::Update(time_t diff)
{
    BattleGround::Update(diff);

    if (GetStatus() == STATUS_WAIT_JOIN && GetPlayersSize())
    {
        ModifyStartDelayTime(diff);
		InitWorldStates();

        if (!(m_Events & 0x01))
        {
            m_Events |= 0x01;

            sLog.outDebug("Alterac Valley: entering state STATUS_WAIT_JOIN ...");

            // Initial Nodes
	    //alliance
            for(uint32 i = BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE ; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            for(uint32 i = BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER ; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            for(uint32 i = BG_AV_OBJECT_FLAG_A_ICEBLOOD_GRAVE; i <= BG_AV_OBJECT_FLAG_A_FROSTWOLF_HUT ; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);

            //horde
            for(uint32 i = BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE; i <= BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER ; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            for(uint32 i = BG_AV_OBJECT_FLAG_H_FIRSTAID_STATION; i <= BG_AV_OBJECT_FLAG_H_STONEHEART_GRAVE ; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);
            //contested (both teams)
            for(uint32 i = BG_AV_OBJECT_FLAG_C_A_FIRSTAID_STATION; i <= BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);
            //snowfall and the doors
            for(uint32 i = BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE; i <= BG_AV_OBJECT_DOOR_A; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);

                SpawnBGObject(BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE, RESPAWN_ONE_DAY);
                SpawnBGObject(BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE, RESPAWN_ONE_DAY);
            //Spiritguides
            for(uint32 i = AV_SPIRIT_STORM_AID; i < AV_SPIRIT_SNOWFALL; i++)
                SpawnBGCreature(i, RESPAWN_IMMEDIATELY);

            for(uint32 i = AV_SPIRIT_FROSTWOLF; i <= AV_SPIRIT_MAIN_HORDE; i++)
                SpawnBGCreature(i, RESPAWN_IMMEDIATELY);

            SetStartDelayTime(START_DELAY0);
        }
        // After 1 minute, warning is signalled
        else if (GetStartDelayTime() <= START_DELAY1 && !(m_Events & 0x04))
        {
            m_Events |= 0x04;
            SendMessageToAll(LANG_BG_AV_ONEMINTOSTART);
        }
        // After 1,5 minute, warning is signalled
        else if (GetStartDelayTime() <= START_DELAY2 && !(m_Events & 0x08))
        {
            m_Events |= 0x08;
            SendMessageToAll(LANG_BG_AV_HALFMINTOSTART);
        }
        // After 2 minutes, gates OPEN ! x)
        else if (GetStartDelayTime() <= 0 && !(m_Events & 0x10))
        {
            m_Events |= 0x10;
            SendMessageToAll(LANG_BG_AV_STARTED);

            DoorOpen(BG_AV_OBJECT_DOOR_H);
            DoorOpen(BG_AV_OBJECT_DOOR_A);

            PlaySoundToAll(SOUND_BG_START);
            SetStatus(STATUS_IN_PROGRESS);

            for(std::map<uint64, BattleGroundPlayer>::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
            {
                Player *plr = objmgr.GetPlayer(itr->first);
                if(plr)
                    plr->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
            }
        }
    }
    else if(GetStatus() == STATUS_IN_PROGRESS)
    {
        // Test win condition
        if (m_Team_Scores[1] <= 0)
            EndBattleGround(ALLIANCE);
        if (m_Team_Scores[0] <= 0)
            EndBattleGround(HORDE);

        for(uint32 i = BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; i++)
        {
            if(m_Points_State[i] == POINT_ASSAULTED)
            {
                if(m_Points_Timer[i] <= 0)
                {
                     EventPlayerDestroyedPoint(GetPlaceNode(i));
                } else m_Points_Timer[i] -= diff;
            }
        }
    }
}

void BattleGroundAV::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattleGroundAVScore* sc = new BattleGroundAVScore;

    m_PlayerScores[plr->GetGUID()] = sc;
    InitWorldStates();
}

void BattleGroundAV::RemovePlayer(Player* /*plr*/,uint64 /*guid*/)
{
}

void BattleGroundAV::HandleAreaTrigger(Player *Source, uint32 Trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 SpellId = 0;
    switch(Trigger)
    {
        case 95:
        case 2608:
			if(Source->GetTeam() != ALLIANCE)
                Source->GetSession()->SendAreaTriggerMessage("Only The Alliance can use that portal");
            else
                Source->LeaveBattleground();
            break;
        case 2606:
            if(Source->GetTeam() != HORDE)
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
			Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
			//Source->Unmount();
            break;
        default:
            sLog.outError("WARNING: Unhandled AreaTrigger in Battleground: %u", Trigger);
            Source->GetSession()->SendAreaTriggerMessage("Warning: Unhandled AreaTrigger in Battleground: %u", Trigger);
            break;
    }

    if(SpellId)
        Source->CastSpell(Source, SpellId, true);
}

void BattleGroundAV::UpdatePlayerScore(Player* Source, uint32 type, uint32 value)
{

    std::map<uint64, BattleGroundScore*>::iterator itr = m_PlayerScores.find(Source->GetGUID());

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
        case SCORE_MINES_CAPTURED:
            ((BattleGroundAVScore*)itr->second)->MinesCaptured += value;
            break;
        case SCORE_LEADERS_KILLED:
            ((BattleGroundAVScore*)itr->second)->LeadersKilled += value;
            break;
        case SCORE_SECONDARY_OBJECTIVES:
            ((BattleGroundAVScore*)itr->second)->SecondaryObjectives += value;
            break;
        default:
            BattleGround::UpdatePlayerScore(Source,type,value);
            break;
    }
}



void BattleGroundAV::EventPlayerDestroyedPoint(uint32 node)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    uint32 team = m_Points_Owner[GetNodePlace(node)];

    //despawn banner
    SpawnBGObject(node, RESPAWN_ONE_DAY);
    if( IsTower(node) )
    {
        uint32 opponent = (GetTeamIndexByTeamId(team) == BG_TEAM_ALLIANCE) ? opponent=BG_TEAM_HORDE : opponent = BG_TEAM_ALLIANCE;
        m_Team_Scores[opponent] -= BG_AV_SCORE_TOWER_LOST;
        m_Points_State[GetNodePlace(node)]=POINT_DESTROYED;
        UpdateScore();
        //spawn destroyed aura
    }
    else
    {
        if( GetTeamIndexByTeamId(team) == BG_TEAM_ALLIANCE )
            SpawnBGObject(node-11, RESPAWN_IMMEDIATELY);
        else
            SpawnBGObject(node+11, RESPAWN_IMMEDIATELY);
        m_Points_State[GetNodePlace(node)]=POINT_CONTROLED;
        if( GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE )
            m_Snowfall_Capped=true;
    }
    UpdatePointsIcons(node);
    //send a nice message to all :)
    char buf[256];
    if( IsTower(node) )
        sprintf(buf, LANG_BG_AV_TOWER_TAKEN , GetNodeName(node));
    else
        sprintf(buf, LANG_BG_AV_GRAVE_TAKEN, GetNodeName(node), ( GetTeamIndexByTeamId(team) == BG_TEAM_ALLIANCE ) ?  LANG_BG_AV_ALLY : LANG_BG_AV_HORDE  );
    uint8 type = ( GetTeamIndexByTeamId(team) == BG_TEAM_ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
    WorldPacket data;
    ChatHandler::FillMessageData(&data, NULL, type, LANG_UNIVERSAL, NULL, 0, buf, NULL);
    SendPacketToAll(&data);


}

int32 BattleGroundAV::GetNode(uint64 guid)
{
    uint32 i;
    for( i = 0; i <= BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE; i++)
    {
        if(m_BgObjects[i] == guid)
            return i;
    }
    //if you get here, there are two possible errors
    if( i < BG_AV_OBJECT_MAX )
        sLog.outError("BattleGroundAV: cheating? a player used a gameobject which isnt a Tower or a Grave");
    else
        sLog.outError("BattleGroundAV: cheating? a player used a gameobject which isnt spawned");
    return -1;
}

uint32 BattleGroundAV::GetNodePlace(uint32 node)
{
	//as long as we can trust GetNode we can trust that node is in object-range
	if( node <= BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER )
		return node;
	if( node <= BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_HUT )
		return node-11;
	if( node <= BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_WTOWER )
		return node-7;
	if( node <= BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER )
		return node-22;
	if( node <= BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT )
		return node-33;
	if( node <= BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER )
		return node-29;
	if( node == BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE )
		return 3;
	sLog.outError("BattleGroundAV: ERROR! GetPlace got a wrong node :(");
}

uint32 BattleGroundAV::GetPlaceNode(uint32 node)
{ //this function is the counterpart to getnodeplace()
   if( m_Points_Owner[node] == ALLIANCE )
   {
       if( m_Points_State[node] == POINT_ASSAULTED )
       {
            if( node <= BG_AV_NODES_FROSTWOLF_HUT )
                return node+11;
            if( node >= BG_AV_NODES_ICEBLOOD_TOWER && node <= BG_AV_NODES_FROSTWOLF_WTOWER)
                return node+7;
       }
       else if ( m_Points_State[node] == POINT_CONTROLED )
           if( node <= BG_AV_NODES_STONEHEART_BUNKER )
               return node;
   }
   else if ( m_Points_Owner[node] == HORDE )
   {
       if( m_Points_State[node] == POINT_ASSAULTED )
           if( node <= BG_AV_NODES_STONEHEART_BUNKER )
               return node+22;
       else if ( m_Points_State[node] == POINT_CONTROLED )
       {
           if( node <= BG_AV_NODES_FROSTWOLF_HUT )
               return node+33;
           if( node >= BG_AV_NODES_ICEBLOOD_TOWER && node <= BG_AV_NODES_FROSTWOLF_WTOWER)
               return node+29;
       }
   }
   else if ( m_Points_State[node] == POINT_NEUTRAL )
       return BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE;
   sLog.outError("BattleGroundAV: Error! GetPlaceNode couldn't resolve node %i",node);
}


//called when using banner
void BattleGroundAV::EventPlayerClaimsPoint(Player *player, uint64 guid, uint32 entry)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    if(GetNode(guid) < 0)
        return;

    switch(entry)
    {
        case BG_AV_OBJECTID_BANNER_A:
        {
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_H:
        {
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_A_B:
        {
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_H_B:
        {
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        }
        //node is assaulted by alliance
        case BG_AV_OBJECTID_BANNER_CONT_A:
        {
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        }
        //node is assaulted by horde
        case BG_AV_OBJECTID_BANNER_CONT_H:
        {
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_CONT_A_B:
        {
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_CONT_H_B:
        {
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        }
        case BG_AV_OBJECTID_BANNER_SNOWFALL_N:
        {
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        }
        default:
            break;
    }

}

void BattleGroundAV::EventPlayerDefendsPoint(Player* player, uint32 node)
{
    // despawn old go
    SpawnBGObject(node, RESPAWN_ONE_DAY);
    //spawn new go :)
    if( GetNodePlace(node) != BG_AV_NODES_SNOWFALL_GRAVE )
    {
        if(m_Points_Owner[GetNodePlace(node)] == ALLIANCE)
            SpawnBGObject(node+22, RESPAWN_IMMEDIATELY);
        else if(m_Points_Owner[GetNodePlace(node)] == ALLIANCE)
            SpawnBGObject(node-22, RESPAWN_IMMEDIATELY);
        m_Points_State[GetNodePlace(node)] = POINT_CONTROLED;
    }
    else
    {
        if( !m_Snowfall_Capped ){
            if( player->GetTeam() == ALLIANCE )
                SpawnBGObject(BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
            else
                SpawnBGObject(BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
             m_Points_Timer[GetNodePlace(node)] = BG_AV_SNOWFALL_FIRSTCAP;
             //the state stays the same...
        }else{
             if( player->GetTeam() == ALLIANCE )
                 SpawnBGObject(BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
             else
                 SpawnBGObject(BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
             m_Points_State[GetNodePlace(node)] = POINT_CONTROLED;
        }
    }
    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    UpdatePointsIcons(node);
    //send a nice message to all :)
    char buf[256];
    sprintf(buf, ( IsTower(node) == true ) ? LANG_BG_AV_TOWER_DEFENDED : LANG_BG_AV_GRAVE_DEFENDED, GetNodeName(node));
    uint8 type = ( player->GetTeam() == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
    WorldPacket data;
    ChatHandler::FillMessageData(&data, player->GetSession(), type, LANG_UNIVERSAL, NULL, player->GetGUID(), buf, NULL);
    SendPacketToAll(&data);
    //update the statistic for the defending player
    UpdatePlayerScore(player, ( IsTower(node) == true ) ? SCORE_TOWERS_DEFENDED : SCORE_GRAVEYARDS_DEFENDED, 1);
}

void BattleGroundAV::EventPlayerAssaultsPoint(Player* player, uint32 node)
{
    SpawnBGObject(node, RESPAWN_ONE_DAY);
    if(m_Points_Owner[GetNodePlace(node)] == HORDE){
        SpawnBGObject(node-22, RESPAWN_IMMEDIATELY);
    }else if(m_Points_Owner[GetNodePlace(node)] == ALLIANCE){
        SpawnBGObject(node+22, RESPAWN_IMMEDIATELY);
    }else{ //snowfall
        if( player->GetTeam() == ALLIANCE )
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
        else
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
         m_Points_Timer[GetNodePlace(node)] = BG_AV_SNOWFALL_FIRSTCAP;
    }

    // TODO:we need to add some code here which moves all ressurecting players to a near graveyard

    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    m_Points_State[GetNodePlace(node)] = POINT_ASSAULTED;

    UpdatePointsIcons(node);
    if( GetNodePlace(node) != BG_AV_NODES_SNOWFALL_GRAVE )
        m_Points_Timer[GetNodePlace(node)] = BG_AV_CAPTIME;

    //send a nice message to all :)
    char buf[256];
    sprintf(buf, ( IsTower(node) == true ) ? LANG_BG_AV_TOWER_ASSAULTED : LANG_BG_AV_GRAVE_ASSAULTED, GetNodeName(node),  ( player->GetTeam() == ALLIANCE ) ?  LANG_BG_AV_ALLY : LANG_BG_AV_HORDE );
    uint8 type = ( player->GetTeam() == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
    WorldPacket data;
    ChatHandler::FillMessageData(&data, player->GetSession(), type, LANG_UNIVERSAL, NULL, player->GetGUID(), buf, NULL);
    SendPacketToAll(&data);
    //update the statistic for the assaulting player
    UpdatePlayerScore(player, ( IsTower(node) == true ) ? SCORE_TOWERS_ASSAULTED : SCORE_GRAVEYARDS_ASSAULTED, 1);
}

void BattleGroundAV::UpdatePointsIcons(uint32 node)
{
	node = GetNodePlace(node);
    switch(node)
    {
        case BG_AV_NODES_FIRSTAID_STATION:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_AID_A_C, 1);
                UpdateWorldState(AV_AID_A_A, 0);
                UpdateWorldState(AV_AID_H_C, 0);
                UpdateWorldState(AV_AID_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_AID_A_C, 0);
                UpdateWorldState(AV_AID_A_A, 0);
                UpdateWorldState(AV_AID_H_C, 1);
                UpdateWorldState(AV_AID_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_AID_A_C, 0);
                UpdateWorldState(AV_AID_A_A, 1);
                UpdateWorldState(AV_AID_H_C, 0);
                UpdateWorldState(AV_AID_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_AID_A_C, 0);
                UpdateWorldState(AV_AID_A_A, 0);
                UpdateWorldState(AV_AID_H_C, 0);
                UpdateWorldState(AV_AID_H_A, 1);
            }
        break;
        }
        case BG_AV_NODES_DUNBALDAR_SOUTH:
        {
            if(m_Points_Owner[node] == ALLIANCE)
            {
                UpdateWorldState(AV_DUNS_CONTROLLED, 1);
                UpdateWorldState(AV_DUNS_DESTROYED, 0);
                UpdateWorldState(AV_DUNS_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_DUNS_CONTROLLED, 0);
                UpdateWorldState(AV_DUNS_DESTROYED, 0);
                UpdateWorldState(AV_DUNS_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_DUNS_CONTROLLED, 0);
                UpdateWorldState(AV_DUNS_DESTROYED, 1);
                UpdateWorldState(AV_DUNS_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_DUNBALDAR_NORTH:
        {
            if(m_Points_Owner[node] == ALLIANCE)
            {
                UpdateWorldState(AV_DUNN_CONTROLLED, 1);
                UpdateWorldState(AV_DUNN_DESTROYED, 0);
                UpdateWorldState(AV_DUNN_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_DUNN_CONTROLLED, 0);
                UpdateWorldState(AV_DUNN_DESTROYED, 0);
                UpdateWorldState(AV_DUNN_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_DUNN_CONTROLLED, 0);
                UpdateWorldState(AV_DUNN_DESTROYED, 1);
                UpdateWorldState(AV_DUNN_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_STORMPIKE_GRAVE:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_PIKEGRAVE_A_C, 1);
                UpdateWorldState(AV_PIKEGRAVE_A_A, 0);
                UpdateWorldState(AV_PIKEGRAVE_H_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_A_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_PIKEGRAVE_A_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_A_A, 0);
                UpdateWorldState(AV_PIKEGRAVE_H_C, 1);
                UpdateWorldState(AV_PIKEGRAVE_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_PIKEGRAVE_A_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_A_A, 1);
                UpdateWorldState(AV_PIKEGRAVE_H_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_PIKEGRAVE_A_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_A_A, 0);
                UpdateWorldState(AV_PIKEGRAVE_H_C, 0);
                UpdateWorldState(AV_PIKEGRAVE_H_A, 1);
            }
        break;
        }
        case BG_AV_NODES_ICEWING_BUNKER:
        {
            if(m_Points_Owner[node] == ALLIANCE)
            {
                UpdateWorldState(AV_ICEWING_CONTROLLED, 1);
                UpdateWorldState(AV_ICEWING_DESTROYED, 0);
                UpdateWorldState(AV_ICEWING_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_ICEWING_CONTROLLED, 0);
                UpdateWorldState(AV_ICEWING_DESTROYED, 0);
                UpdateWorldState(AV_ICEWING_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_ICEWING_CONTROLLED, 0);
                UpdateWorldState(AV_ICEWING_DESTROYED, 1);
                UpdateWorldState(AV_ICEWING_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_STONEHEART_GRAVE:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_STONEHEART_A_C, 1);
                UpdateWorldState(AV_STONEHEART_A_A, 0);
                UpdateWorldState(AV_STONEHEART_H_C, 0);
                UpdateWorldState(AV_STONEHEART_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_STONEHEART_A_C, 0);
                UpdateWorldState(AV_STONEHEART_A_A, 0);
                UpdateWorldState(AV_STONEHEART_H_C, 1);
                UpdateWorldState(AV_STONEHEART_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_STONEHEART_A_C, 0);
                UpdateWorldState(AV_STONEHEART_A_A, 1);
                UpdateWorldState(AV_STONEHEART_H_C, 0);
                UpdateWorldState(AV_STONEHEART_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_STONEHEART_A_C, 0);
                UpdateWorldState(AV_STONEHEART_A_A, 0);
                UpdateWorldState(AV_STONEHEART_H_C, 0);
                UpdateWorldState(AV_STONEHEART_H_A, 1);
            }
        break;
        }
        case BG_AV_NODES_STONEHEART_BUNKER:
        {
            if(m_Points_Owner[node] == ALLIANCE)
            {
                UpdateWorldState(AV_STONEH_CONTROLLED, 1);
                UpdateWorldState(AV_STONEH_DESTROYED, 0);
                UpdateWorldState(AV_STONEH_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_STONEH_CONTROLLED, 0);
                UpdateWorldState(AV_STONEH_DESTROYED, 0);
                UpdateWorldState(AV_STONEH_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_STONEH_CONTROLLED, 0);
                UpdateWorldState(AV_STONEH_DESTROYED, 1);
                UpdateWorldState(AV_STONEH_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_SNOWFALL_GRAVE:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_SNOWFALL_N, 0);
                UpdateWorldState(AV_SNOWFALL_A_C, 1);
                UpdateWorldState(AV_SNOWFALL_A_A, 0);
                UpdateWorldState(AV_SNOWFALL_H_C, 0);
                UpdateWorldState(AV_SNOWFALL_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_SNOWFALL_N, 0);
                UpdateWorldState(AV_SNOWFALL_A_C, 0);
                UpdateWorldState(AV_SNOWFALL_A_A, 0);
                UpdateWorldState(AV_SNOWFALL_H_C, 1);
                UpdateWorldState(AV_SNOWFALL_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_SNOWFALL_N, 0);
                UpdateWorldState(AV_SNOWFALL_A_C, 0);
                UpdateWorldState(AV_SNOWFALL_A_A, 1);
                UpdateWorldState(AV_SNOWFALL_H_C, 0);
                UpdateWorldState(AV_SNOWFALL_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_SNOWFALL_N, 0);
                UpdateWorldState(AV_SNOWFALL_A_C, 0);
                UpdateWorldState(AV_SNOWFALL_A_A, 0);
                UpdateWorldState(AV_SNOWFALL_H_C, 0);
                UpdateWorldState(AV_SNOWFALL_H_A, 1);
            }

        break;
        }
        case BG_AV_NODES_ICEBLOOD_TOWER:
        {
            if(m_Points_Owner[node] == ALLIANCE)
            {
                UpdateWorldState(AV_ICEBLOOD_CONTROLLED, 1);
                UpdateWorldState(AV_ICEBLOOD_DESTROYED, 0);
                UpdateWorldState(AV_ICEBLOOD_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_ICEBLOOD_CONTROLLED, 0);
                UpdateWorldState(AV_ICEBLOOD_DESTROYED, 0);
                UpdateWorldState(AV_ICEBLOOD_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_ICEBLOOD_CONTROLLED, 0);
                UpdateWorldState(AV_ICEBLOOD_DESTROYED, 1);
                UpdateWorldState(AV_ICEBLOOD_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_ICEBLOOD_GRAVE:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_ICEBLOOD_A_C, 1);
                UpdateWorldState(AV_ICEBLOOD_A_A, 0);
                UpdateWorldState(AV_ICEBLOOD_H_C, 0);
                UpdateWorldState(AV_ICEBLOOD_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_ICEBLOOD_A_C, 0);
                UpdateWorldState(AV_ICEBLOOD_A_A, 0);
                UpdateWorldState(AV_ICEBLOOD_H_C, 1);
                UpdateWorldState(AV_ICEBLOOD_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_ICEBLOOD_A_C, 0);
                UpdateWorldState(AV_ICEBLOOD_A_A, 1);
                UpdateWorldState(AV_ICEBLOOD_H_C, 0);
                UpdateWorldState(AV_ICEBLOOD_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_ICEBLOOD_A_C, 0);
                UpdateWorldState(AV_ICEBLOOD_A_A, 0);
                UpdateWorldState(AV_ICEBLOOD_H_C, 0);
                UpdateWorldState(AV_ICEBLOOD_H_A, 1);
            }
        break;
        }
        case BG_AV_NODES_TOWER_POINT:
        {
            if(m_Points_Owner[node] == HORDE)
            {
                UpdateWorldState(AV_TOWERPOINT_CONTROLLED, 1);
                UpdateWorldState(AV_TOWERPOINT_DESTROYED, 0);
                UpdateWorldState(AV_TOWERPOINT_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_TOWERPOINT_CONTROLLED, 0);
                UpdateWorldState(AV_TOWERPOINT_DESTROYED, 0);
                UpdateWorldState(AV_TOWERPOINT_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_TOWERPOINT_CONTROLLED, 0);
                UpdateWorldState(AV_TOWERPOINT_DESTROYED, 1);
                UpdateWorldState(AV_TOWERPOINT_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_FROSTWOLF_GRAVE:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_FROSTWOLF_A_C, 1);
                UpdateWorldState(AV_FROSTWOLF_A_A, 0);
                UpdateWorldState(AV_FROSTWOLF_H_C, 0);
                UpdateWorldState(AV_FROSTWOLF_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_FROSTWOLF_A_C, 0);
                UpdateWorldState(AV_FROSTWOLF_A_A, 0);
                UpdateWorldState(AV_FROSTWOLF_H_C, 1);
                UpdateWorldState(AV_FROSTWOLF_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLF_A_C, 0);
                UpdateWorldState(AV_FROSTWOLF_A_A, 1);
                UpdateWorldState(AV_FROSTWOLF_H_C, 0);
                UpdateWorldState(AV_FROSTWOLF_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLF_A_C, 0);
                UpdateWorldState(AV_FROSTWOLF_A_A, 0);
                UpdateWorldState(AV_FROSTWOLF_H_C, 0);
                UpdateWorldState(AV_FROSTWOLF_H_A, 1);
            }
        break;
        }
        case BG_AV_NODES_FROSTWOLF_ETOWER:
        {
            if(m_Points_Owner[node] == HORDE)
            {
                UpdateWorldState(AV_FROSTWOLFE_CONTROLLED, 1);
                UpdateWorldState(AV_FROSTWOLFE_DESTROYED, 0);
                UpdateWorldState(AV_FROSTWOLFE_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLFE_CONTROLLED, 0);
                UpdateWorldState(AV_FROSTWOLFE_DESTROYED, 0);
                UpdateWorldState(AV_FROSTWOLFE_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_FROSTWOLFE_CONTROLLED, 0);
                UpdateWorldState(AV_FROSTWOLFE_DESTROYED, 1);
                UpdateWorldState(AV_FROSTWOLFE_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_FROSTWOLF_WTOWER:
        {
            if(m_Points_Owner[node] == HORDE)
            {
                UpdateWorldState(AV_FROSTWOLFW_CONTROLLED, 1);
                UpdateWorldState(AV_FROSTWOLFW_DESTROYED, 0);
                UpdateWorldState(AV_FROSTWOLFW_ASSAULTED, 0);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLFW_CONTROLLED, 0);
                UpdateWorldState(AV_FROSTWOLFW_DESTROYED, 0);
                UpdateWorldState(AV_FROSTWOLFW_ASSAULTED, 1);
            } else if (m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_DESTROYED)
            {
                UpdateWorldState(AV_FROSTWOLFW_CONTROLLED, 0);
                UpdateWorldState(AV_FROSTWOLFW_DESTROYED, 1);
                UpdateWorldState(AV_FROSTWOLFW_ASSAULTED, 0);
            }
        break;
        }
        case BG_AV_NODES_FROSTWOLF_HUT:
        {
            if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_FROSTWOLFHUT_A_C, 1);
                UpdateWorldState(AV_FROSTWOLFHUT_A_A, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_CONTROLED)
            {
                UpdateWorldState(AV_FROSTWOLFHUT_A_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_A_A, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_C, 1);
                UpdateWorldState(AV_FROSTWOLFHUT_H_A, 0);
            }  else if(m_Points_Owner[node] == ALLIANCE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLFHUT_A_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_A_A, 1);
                UpdateWorldState(AV_FROSTWOLFHUT_H_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_A, 0);
            }  else if(m_Points_Owner[node] == HORDE && m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_FROSTWOLFHUT_A_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_A_A, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_C, 0);
                UpdateWorldState(AV_FROSTWOLFHUT_H_A, 1);
            }
        break;
        }
        default:
            break;
    }
}

bool BattleGroundAV::IsTower(uint32 node)
{
	node = GetNodePlace(node);
    if(   node != BG_AV_NODES_FIRSTAID_STATION
       && node != BG_AV_NODES_STORMPIKE_GRAVE
       && node != BG_AV_NODES_STONEHEART_GRAVE
       && node != BG_AV_NODES_SNOWFALL_GRAVE
       && node != BG_AV_NODES_ICEBLOOD_GRAVE
       && node != BG_AV_NODES_FROSTWOLF_GRAVE
       && node != BG_AV_NODES_FROSTWOLF_HUT)
    {
        return true;
    }else return false;
}



WorldSafeLocsEntry const* BattleGroundAV::SelectGraveYard(Player* player)
{
    uint8 teamIndex = GetTeamIndexByTeamId(player->GetTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> nodes;
    for (uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
        if (m_Points_Owner[i] == player->GetTeam() && !IsTower(i) && m_Points_State[i] == POINT_CONTROLED)//!IsTower is not realy needed
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    // If so, select the closest node to place ghost on
    if (!nodes.empty())
    {
        float x = player->GetPositionX();
        float y = player->GetPositionY();    // let alone z coordinate...
        float mindist = 999999.0f;
        for (uint8 i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[i] );
            if(!entry){
                sLog.outString("BG:AV : Couldnt SelectGraveYard with %i",i);
                continue;
            }
            float dist = (entry->x - x)*(entry->x - x) + (entry->y - y)*(entry->y - y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }
    // If not, place ghost on starting location

    if(!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[teamIndex+8] );

    return good_entry;
}

bool BattleGroundAV::SetupBattleGround()
{
	m_Team_Scores[0]=BG_AV_SCORE_INITIAL_POINTS;
	m_Team_Scores[1]=BG_AV_SCORE_INITIAL_POINTS;
    m_Snowfall_Capped=false;
    // Create starting objects
    if(    !AddObject(BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH, BG_AV_OBJECTID_BANNER_A, 552.585,-77.9304,51.9273,5.10115,0,0,0.557207,-0.830373, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION, BG_AV_OBJECTID_BANNER_A_B, 623.184,-42.7891,42.2024,5.13256,0,0,0.544096,-0.839023, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_DUNBALDAR_NORTH, BG_AV_OBJECTID_BANNER_A, 673.021,-144.669,63.6473,0.907122,0,0,0.438169,0.898892, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_STORMPIKE_GRAVE, BG_AV_OBJECTID_BANNER_A_B, 668.013,-292.967,30.2909,2.76067,0,0,0.981917,0.189313, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_ICEWING_BUNKER, BG_AV_OBJECTID_BANNER_A, 202.515,-359.258,56.3768,5.34463,0,0,0.452241,-0.891896, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE, BG_AV_OBJECTID_BANNER_A_B, 78.3916,-405.245,47.1657,1.64148,0,0,0.73165,0.68168, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER, BG_AV_OBJECTID_BANNER_A, -151.864,-440.838,40.3897,4.1744,0,0,0.869601,-0.493754, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_SNOWFALL_N, -202.776,-112.4,78.4893,2.01061,0,0,0.844326,0.53583, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_ICEBLOOD_TOWER, BG_AV_OBJECTID_BANNER_H, -572.723,-262.062,75.0087,5.44671,0,0,0.406149,-0.913807, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE, BG_AV_OBJECTID_BANNER_H_B, -611.962,-396.17,60.8351,2.53682,0,0,0.954629,0.297799, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_TOWER_POINT, BG_AV_OBJECTID_BANNER_H, -768.907,-363.71,90.8949,1.07991,0,0,0.514096,0.857733, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_FROSTWOLF_GRAVE, BG_AV_OBJECTID_BANNER_H_B, -1083.37,-340.922,55.2714,4.99119,0,0,0.601994,-0.798501, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_FROSTWOLF_ETOWER, BG_AV_OBJECTID_BANNER_H, -1297.5,-266.767,114.15,3.31044,0,0,0.996438,-0.0843258, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER, BG_AV_OBJECTID_BANNER_H, -1302.89,-316.92,113.867,2.01453,0,0,0.845375,0.534173, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT, BG_AV_OBJECTID_BANNER_H_B, -1403.93,-308.839,89.3659,5.24645,0,0,0.495464,-0.868629, RESPAWN_ONE_DAY)
        // alliance gates
        || !AddObject(BG_AV_OBJECT_DOOR_A, BG_AV_OBJECTID_GATE_A, 780.487, -493.024, 99.9553, 3.0976, 0.0, 0.0, 0.999758, 0.0219952, RESPAWN_IMMEDIATELY)
        // horde gates
        || !AddObject(BG_AV_OBJECT_DOOR_H, BG_AV_OBJECTID_GATE_H, -1375.193, -538.981, 55.2824, 0.72178, 0.0, 0.0, 0.883025, 0.469326, RESPAWN_IMMEDIATELY)
        //finished initial setup
        //can only be used by horde(alliance has attacked this node)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_FIRSTAID_STATION, BG_AV_OBJECTID_BANNER_CONT_A_B, 623.184,-42.7891,42.2024,5.13256,0,0,0.544096,-0.839023, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_STORMPIKE_GRAVE, BG_AV_OBJECTID_BANNER_CONT_A_B, 668.013,-292.967,30.2909,2.76067,0,0,0.981917,0.189313, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_STONEHEART_GRAVE, BG_AV_OBJECTID_BANNER_CONT_A_B, 78.3916,-405.245,47.1657,1.64148,0,0,0.73165,0.68168, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_CONT_A_B, -202.776,-112.4,78.4893,2.01061,0,0,0.844326,0.53583, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_TOWER, BG_AV_OBJECTID_BANNER_CONT_A, -572.723,-262.062,75.0087,5.44671,0,0,0.406149,-0.913807, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_GRAVE, BG_AV_OBJECTID_BANNER_CONT_A_B, -611.962,-396.17,60.8351,2.53682,0,0,0.954629,0.297799, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_TOWER_POINT, BG_AV_OBJECTID_BANNER_CONT_A, -768.907,-363.71,90.8949,1.07991,0,0,0.514096,0.857733, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_GRAVE, BG_AV_OBJECTID_BANNER_CONT_A_B, -1083.37,-340.922,55.2714,4.99119,0,0,0.601994,-0.798501, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_ETOWER, BG_AV_OBJECTID_BANNER_CONT_A, -1297.5,-266.767,114.15,3.31044,0,0,0.996438,-0.0843258, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_WTOWER, BG_AV_OBJECTID_BANNER_CONT_A, -1302.89,-316.92,113.867,2.01453,0,0,0.845375,0.534173, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_HUT, BG_AV_OBJECTID_BANNER_CONT_A_B, -1403.93,-308.839,89.3659,5.24645,0,0,0.495464,-0.868629, RESPAWN_ONE_DAY)
        //can only be used by alliance(horde has attacked this node)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_SOUTH, BG_AV_OBJECTID_BANNER_CONT_H, 552.585,-77.9304,51.9273,5.10115,0,0,0.557207,-0.830373, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_FIRSTAID_STATION, BG_AV_OBJECTID_BANNER_CONT_H_B, 623.184,-42.7891,42.2024,5.13256,0,0,0.544096,-0.839023, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_NORTH, BG_AV_OBJECTID_BANNER_CONT_H, 673.021,-144.669,63.6473,0.907122,0,0,0.438169,0.898892, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_STORMPIKE_GRAVE, BG_AV_OBJECTID_BANNER_CONT_H_B, 668.013,-292.967,30.2909,2.76067,0,0,0.981917,0.189313, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_ICEWING_BUNKER, BG_AV_OBJECTID_BANNER_CONT_H, 202.515,-359.258,56.3768,5.34463,0,0,0.452241,-0.891896, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_STONEHEART_GRAVE, BG_AV_OBJECTID_BANNER_CONT_H_B, 78.3916,-405.245,47.1657,1.64148,0,0,0.73165,0.68168, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER, BG_AV_OBJECTID_BANNER_CONT_H, -151.864,-440.838,40.3897,4.1744,0,0,0.869601,-0.493754, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_CONT_H_B, -202.776,-112.4,78.4893,2.01061,0,0,0.844326,0.53583, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_ICEBLOOD_GRAVE, BG_AV_OBJECTID_BANNER_CONT_H_B, -611.962,-396.17,60.8351,2.53682,0,0,0.954629,0.297799, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_GRAVE, BG_AV_OBJECTID_BANNER_CONT_H_B, -1083.37,-340.922,55.2714,4.99119,0,0,0.601994,-0.798501, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_HUT, BG_AV_OBJECTID_BANNER_CONT_H_B, -1403.93,-308.839,89.3659,5.24645,0,0,0.495464,-0.868629, RESPAWN_ONE_DAY)
	//overtaken nodes (graveyards)
        || !AddObject(BG_AV_OBJECT_FLAG_H_FIRSTAID_STATION, BG_AV_OBJECTID_BANNER_H_B, 623.184,-42.7891,42.2024,5.13256,0,0,0.544096,-0.839023, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_STORMPIKE_GRAVE, BG_AV_OBJECTID_BANNER_H_B, 668.013,-292.967,30.2909,2.76067,0,0,0.981917,0.189313, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_STONEHEART_GRAVE, BG_AV_OBJECTID_BANNER_H_B, 78.3916,-405.245,47.1657,1.64148,0,0,0.73165,0.68168, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_H_B, -202.776,-112.4,78.4893,2.01061,0,0,0.844326,0.53583, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_A_B, -202.776,-112.4,78.4893,2.01061,0,0,0.844326,0.53583, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_ICEBLOOD_GRAVE, BG_AV_OBJECTID_BANNER_A_B, -611.962,-396.17,60.8351,2.53682,0,0,0.954629,0.297799, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_FROSTWOLF_GRAVE, BG_AV_OBJECTID_BANNER_A_B, -1083.37,-340.922,55.2714,4.99119,0,0,0.601994,-0.798501, RESPAWN_ONE_DAY)
        || !AddObject(BG_AV_OBJECT_FLAG_A_FROSTWOLF_HUT, BG_AV_OBJECTID_BANNER_A_B, -1403.93,-308.839,89.3659,5.24645,0,0,0.495464,-0.868629, RESPAWN_ONE_DAY))

    {
        sLog.outErrorDb("BatteGroundAV: Failed to spawn some object BattleGround not created!");
        return false;
    }

    WorldSafeLocsEntry const *sg = NULL;
    uint16 team = 0;
    for(uint32 i = 0; i < 10; i++)//7 graveyards + the 2 main graveyard
    {
        if(BG_AV_GraveyardIds[i]!=0)
        {
		if(i<8)         	team = m_Points_Owner[i];
		else if(i == 8) team = ALLIANCE;
		else if(i == 9) team = HORDE;
            sg = sWorldSafeLocsStore.LookupEntry(BG_AV_GraveyardIds[i]);
            if(team != 0)
                if(!sg || !AddSpiritGuide(i, sg->x, sg->y, sg->z, 3.193953, team))
                {
                    sLog.outErrorDb("Failed to spawn spirit guide! BattleGround not created!");
                    return false;
                }
        }
    }
    return true;
}

const char* BattleGroundAV::GetNodeName(uint32 node)
{
	node = GetNodePlace(node);
    switch (node)
    {
	case BG_AV_NODES_FIRSTAID_STATION: return "Stormpike Aid Station";
	case BG_AV_NODES_DUNBALDAR_SOUTH: return "Dun Baldar South Bunker";
	case BG_AV_NODES_DUNBALDAR_NORTH: return "Dun Baldar North Bunker";
	case BG_AV_NODES_STORMPIKE_GRAVE: return "Stormpike Graveyard";
	case BG_AV_NODES_ICEWING_BUNKER: return "The Icewing Bunker";
	case BG_AV_NODES_STONEHEART_GRAVE: return "Stonehearth Graveyard";
	case BG_AV_NODES_STONEHEART_BUNKER: return "Stonehearth Bunker";
	case BG_AV_NODES_SNOWFALL_GRAVE: return "Snowfall Graveyard";
	case BG_AV_NODES_ICEBLOOD_TOWER: return "Iceblood Tower";
	case BG_AV_NODES_ICEBLOOD_GRAVE: return "Iceblood Graveyard";
	case BG_AV_NODES_TOWER_POINT: return "Tower Point";
	case BG_AV_NODES_FROSTWOLF_GRAVE: return "Frostwolf Graveyard";
	case BG_AV_NODES_FROSTWOLF_ETOWER: return "East Frostwolf Tower";
	case BG_AV_NODES_FROSTWOLF_WTOWER: return "West Frostwolf Tower";
	case BG_AV_NODES_FROSTWOLF_HUT: return "Frostwolf Relief Hut";
        default:
            {
            sLog.outError("tried to get name for node %u%",node);
            return "Unknown";
            break;
            }
    }
    return "";
}

void BattleGroundAV::ResetBGSubclass()
{
	m_Team_Scores[0]=BG_AV_SCORE_INITIAL_POINTS;
	m_Team_Scores[1]=BG_AV_SCORE_INITIAL_POINTS;

    for(uint8 i = 0; i <= BG_AV_NODES_STONEHEART_GRAVE; i++)
    {
        m_Points_Owner[i] = ALLIANCE;
        m_Points_State[i] = POINT_CONTROLED;
    }
	for(uint8 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; i++)
    {
        m_Points_Owner[i] = ALLIANCE;
        m_Points_State[i] = POINT_CONTROLED;
    }

    for(uint8 i = BG_AV_NODES_ICEBLOOD_GRAVE; i <= BG_AV_NODES_FROSTWOLF_HUT; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_State[i] = POINT_CONTROLED;
    }
    for(uint8 i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_State[i] = POINT_CONTROLED;
    }

    m_Points_Owner[BG_AV_NODES_SNOWFALL_GRAVE] = 0;
    m_Points_State[BG_AV_NODES_SNOWFALL_GRAVE] = POINT_NEUTRAL;

    for(uint8 i = AV_SPIRIT_STORM_AID; i < BG_AV_CREATURES_MAX; i++)
    {
        if(m_BgCreatures[i])
            DelCreature(i);
    }

    for(uint8 i = BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION; i < BG_AV_OBJECT_MAX; i++)
    {
        if(m_BgObjects[i]){
            DelObject(i);
        }

    }
}
