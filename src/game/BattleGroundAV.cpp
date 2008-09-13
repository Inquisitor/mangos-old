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
    m_BgCreatures.resize(AV_CPLACE_MAX);
}

BattleGroundAV::~BattleGroundAV()
{

}

void BattleGroundAV::HandleKillPlayer(Player *player, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
	UpdateScore((player-GetTeam() == ALLIANCE) ? BG_TEAM_HORDE : BG_TEAM_ALLIANCE,-1);
}

void BattleGroundAV::HandleKillUnit(Creature *unit, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    uint32 entry = unit->GetEntry();
    if(entry == BG_AV_CreatureInfo[AV_NPC_A_BOSS][0])
    {
        RewardReputationToTeam(729,BG_AV_REP_BOSS,HORDE);
        RewardHonorToTeam(BG_AV_HONOR_BOSS,HORDE);
        EndBattleGround(HORDE);
    }
    else if ( entry == BG_AV_CreatureInfo[AV_NPC_H_BOSS][0] )
    {
        RewardReputationToTeam(730,BG_AV_REP_BOSS,ALLIANCE);
        RewardHonorToTeam(BG_AV_HONOR_BOSS,ALLIANCE);
        EndBattleGround(ALLIANCE);
    }
    else if(entry == BG_AV_CreatureInfo[AV_NPC_A_CAPTAIN][0])
    {
        RewardReputationToTeam(729,BG_AV_REP_CAPTAIN,HORDE);
        RewardHonorToTeam(BG_AV_HONOR_CAPTAIN,HORDE);
	    UpdateScore(BG_TEAM_ALLIANCE,(-1)*BG_AV_RES_CAPTAIN);
    }
    else if ( entry == BG_AV_CreatureInfo[AV_NPC_H_CAPTAIN][0] )
    {
        RewardReputationToTeam(730,BG_AV_REP_CAPTAIN,ALLIANCE);
        RewardHonorToTeam(BG_AV_HONOR_CAPTAIN,ALLIANCE);
	UpdateScore(BG_TEAM_HORDE,(-1)*BG_AV_RES_CAPTAIN);
    }
//TODO add both mine bosses here.. (and for this "killer" is needed)
}

void BattleGroundAV::UpdateQuest(uint32 questid, Player *player)
{
    if (GetStatus() != STATUS_WAIT_JOIN)
        return;//maybe we should log this, cause this must be a cheater or a big bug
    uint8 team = (player->GetTeam() == ALLIANCE)? 0 : 1;
    //TODO add reputation, events (including quest not available anymore, next quest availabe, go/npc de/spawning)and maybe honor
    sLog.outError("BG_AV Quest %i completed",questid);
    switch(questid)
    {
        case AV_QUEST_A_SCRAPS1:
        case AV_QUEST_A_SCRAPS2:
        case AV_QUEST_H_SCRAPS1:
        case AV_QUEST_H_SCRAPS2:
            m_Team_QuestStatus[team][0]+=20;
            if(m_Team_QuestStatus[team][0] == 500 || m_Team_QuestStatus[team][0] == 1000 || m_Team_QuestStatus[team][0] == 1500) //25,50,75 turn ins
            {
                sLog.outDebug("BG_AV Quest %i completed starting with unit upgrading..",questid);
                for (uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
                    if (m_Points_Owner[i] == player->GetTeam() && m_Points_State[i] == POINT_CONTROLED)
                        PopulateNode(i); //update all graveyards (depopulate is not important cause addavcreature deletes befor spawning)
                            //maybe this is bad, because it will instantly respawn all creatures on every grave..
            }
            break;
        case AV_QUEST_A_COMMANDER1:
        case AV_QUEST_H_COMMANDER1:
            m_Team_QuestStatus[team][1]++;
            if(m_Team_QuestStatus[team][1] == 30)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_COMMANDER2:
        case AV_QUEST_H_COMMANDER2:
            m_Team_QuestStatus[team][2]++;
            if(m_Team_QuestStatus[team][2] == 60)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_COMMANDER3:
        case AV_QUEST_H_COMMANDER3:
            m_Team_QuestStatus[team][3]++;
            if(m_Team_QuestStatus[team][1] == 120)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_BOSS1:
        case AV_QUEST_H_BOSS1:
            m_Team_QuestStatus[team][4] += 9; //you can turn in 10 or 1 item..
        case AV_QUEST_A_BOSS2:
        case AV_QUEST_H_BOSS2:
            m_Team_QuestStatus[team][4]++;
            if(m_Team_QuestStatus[team][4] >= 200)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_NEAR_MINE:
        case AV_QUEST_H_NEAR_MINE:
            m_Team_QuestStatus[team][5]++;
            if(m_Team_QuestStatus[team][5] == 28)
            {
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
                if(m_Team_QuestStatus[team][6] == 7)
                    sLog.outDebug("BG_AV Quest %i completed (need to implement some events here - ground assault ready",questid);
            }
            break;
        case AV_QUEST_A_OTHER_MINE:
        case AV_QUEST_H_OTHER_MINE:
            m_Team_QuestStatus[team][6]++;
            if(m_Team_QuestStatus[team][6] == 7)
            {
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
                if(m_Team_QuestStatus[team][5] == 20)
                    sLog.outDebug("BG_AV Quest %i completed (need to implement some events here - ground assault ready",questid);
            }
            break;
        case AV_QUEST_A_RIDER_HIDE:
        case AV_QUEST_H_RIDER_HIDE:
            m_Team_QuestStatus[team][7]++;
            if(m_Team_QuestStatus[team][7] == 25)
            {
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
                if(m_Team_QuestStatus[team][8] == 25)
                    sLog.outDebug("BG_AV Quest %i completed (need to implement some events here - rider assault ready",questid);
            }
            break;
        case AV_QUEST_A_RIDER_TAME:
        case AV_QUEST_H_RIDER_TAME:
            m_Team_QuestStatus[team][8]++;
            if(m_Team_QuestStatus[team][8] == 25)
            {
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
                if(m_Team_QuestStatus[team][7] == 25)
                    sLog.outDebug("BG_AV Quest %i completed (need to implement some events here - rider assault ready",questid);
            }
            break;
        default:
            sLog.outDebug("BG_AV Quest %i completed but is not interesting at all",questid);
            return; //was no interesting quest at all
            break;
    }
}


void BattleGroundAV::UpdateScore(uint8 team, int16 points )
{
    if( team == BG_TEAM_ALLIANCE )
    {
        TAScore += points;
	    uint32 TAScore = (m_Team_Scores[0] < 0)?0:m_Team_Scores[0];
	    UpdateWorldState(AV_Alliance_Score, TAScore);
        if(TAScore==0)
            EndBattleGround(HORDE);
    }
    else if ( team == BG_TEAM_HORDE )
    {
        uint32 THScore = (m_Team_Scores[1] < 0)?0:m_Team_Scores[1];
        UpdateWorldState(AV_Horde_Score, THScore);
        if(THScore==0)
            EndBattleGround(ALLIANCE);
    }
    else
        sLog.outError("BG_AV unknown team %i in updatescore",team);

//TODO:get out at which point this message comes and which text will be displayed and also find out, if this can be displayed 2 times in a bg (for both teams)
//and surely it's better to add this code abovee
    if( !m_IsInformedNearVictory && points<0)
    {
        for(uint8 i=0; i<2; i++)
        {
            if( m_Team_Scores[i] < SEND_MSG_NEAR_LOSE )
            {
                if( i == BG_TEAM_ALLIANCE )
                    SendMessageToAll(LANG_BG_AV_A_NEAR_LOSE);
                else
                    SendMessageToAll(LANG_BG_AV_H_NEAR_LOSE);
//                PlaySoundToAll(SOUND_NEAR_VICTORY);
                m_IsInformedNearVictory = true;
            }
        }
    }
}

void BattleGroundAV::InitWorldStates()
{
		UpdateWorldState(AV_Alliance_Score, m_Team_Scores[0]);
		UpdateWorldState(AV_Horde_Score, m_Team_Scores[1]);
		UpdateWorldState(AV_SHOW_H_SCORE, 1);
		UpdateWorldState(AV_SHOW_A_SCORE, 1);
}

Creature* BattleGroundAV::AddAVCreature(uint8 cinfoid, uint16 type)
{
    if(m_BgCreatures[type]) // in v13 of this patch this is not needed, but i think later its not bad..
            DelCreature(type);
    Creature* creature = AddCreature(BG_AV_CreatureInfo[cinfoid][0],type,BG_AV_CreatureInfo[cinfoid][1],BG_AV_CreaturePos[type][0],BG_AV_CreaturePos[type][1],BG_AV_CreaturePos[type][2],BG_AV_CreaturePos[type][3]);
    uint8 level = ( BG_AV_CreatureInfo[cinfoid][2] == BG_AV_CreatureInfo[cinfoid][3] ) ? BG_AV_CreatureInfo[cinfoid][2] : urand(BG_AV_CreatureInfo[cinfoid][2],BG_AV_CreatureInfo[cinfoid][3]);
    level += GetMinLevel()-51; //maybe we can do this more generic for custom level-range.. actually it's blizzlike
    sLog.outError("BG_AV minlevel %i",GetMinLevel());
    creature->SetLevel(level);
    //if is bowman, make unit stand still <--this must be added here..(TODO)
    return creature;
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
            for(uint8 i = 0; i < BG_AV_OBJECT_MAX; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);
            for(uint8 i = BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE ; i++){
                SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+3*i,RESPAWN_IMMEDIATELY);
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            }
            for(uint8 i = BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER ; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            for(uint8 i = BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE; i <= BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER ; i++){
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
                if(i<=BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT)
                    SpawnBGObject(BG_AV_OBJECT_AURA_H_FIRSTAID_STATION+3*GetNodePlace(i),RESPAWN_IMMEDIATELY);
             }

            //snowfall and the doors
            for(uint8 i = BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE; i <= BG_AV_OBJECT_DOOR_A; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            SpawnBGObject(BG_AV_OBJECT_AURA_N_SNOWFALL_GRAVE,RESPAWN_IMMEDIATELY);
            //creatures
			for (uint8 i= BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; i++ )
				PopulateNode(i);
            //boss and captain of both teams:
		AddAVCreature(AV_NPC_H_BOSS,AV_CPLACE_H_BOSS);
		AddAVCreature(AV_NPC_A_BOSS,AV_CPLACE_A_BOSS);
		AddAVCreature(AV_NPC_H_CAPTAIN,AV_CPLACE_H_CAPTAIN);
		AddAVCreature(AV_NPC_A_CAPTAIN,AV_CPLACE_A_CAPTAIN);
		//mainspiritguides:
            //if a player can ressurect before the bg starts, this must stay here...
	        AddSpiritGuide(7, BG_AV_CreaturePos[7][0], BG_AV_CreaturePos[7][1], BG_AV_CreaturePos[7][2], BG_AV_CreaturePos[7][3], ALLIANCE);
		AddSpiritGuide(8, BG_AV_CreaturePos[8][0], BG_AV_CreaturePos[8][1], BG_AV_CreaturePos[8][2], BG_AV_CreaturePos[8][3], HORDE);

            DoorClose(BG_AV_OBJECT_DOOR_A);
            DoorClose(BG_AV_OBJECT_DOOR_H);

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

            for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                if(Player* plr = objmgr.GetPlayer(itr->first))
                    plr->RemoveAurasDueToSpell(SPELL_PREPARATION);
        }
    }
    else if(GetStatus() == STATUS_IN_PROGRESS)
    {
        for(uint32 i = BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; i++)
            if(m_Points_State[i] == POINT_ASSAULTED)
                if(m_Points_Timer[i] <= 0)
                     EventPlayerDestroyedPoint(GetPlaceNode(i));
                else
                    m_Points_Timer[i] -= diff;
    }
}

void BattleGroundAV::AddPlayer(Player *plr)
{
    BattleGround::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    //TODO:update the players map, so he can see which nodes are occupied
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
    if( IsTower(GetNodePlace(node)) )
    {
        m_Points_State[GetNodePlace(node)]=POINT_DESTROYED;
        UpdateScore((team == ALLIANCE) ? BG_TEAM_HORDE : BG_TEAM_ALLIANCE, (-1)*BG_AV_RES_TOWER);
        //spawn destroyed aura
        RewardReputationToTeam((team == ALLIANCE)?730:729,BG_AV_REP_TOWER,team);
        RewardHonorToTeam(BG_AV_HONOR_TOWER,team);
    }
    else
    {
	    if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE && !m_Snowfall_Capped)
            m_Snowfall_Capped=true;
        if( team == ALLIANCE )
            SpawnBGObject(node-11, RESPAWN_IMMEDIATELY);
        else
            SpawnBGObject(node+11, RESPAWN_IMMEDIATELY);
        SpawnBGObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+3*GetNodePlace(node),RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+GetTeamIndexByTeamId(team)+3*GetNodePlace(node),RESPAWN_IMMEDIATELY);
        m_Points_State[GetNodePlace(node)]=POINT_CONTROLED;
        PopulateNode(GetNodePlace(node));
    }
    UpdatePointsIcons(GetNodePlace(node));
    //send a nice message to all :)
    char buf[256];
    if( IsTower(GetNodePlace(node)) )
        sprintf(buf, LANG_BG_AV_TOWER_TAKEN , GetNodeName(GetNodePlace(node)));
    else
        sprintf(buf, LANG_BG_AV_GRAVE_TAKEN, GetNodeName(GetNodePlace(node)), ( team == ALLIANCE ) ?  LANG_BG_AV_ALLY : LANG_BG_AV_HORDE  );
    uint8 type = ( team == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
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
    if( i < BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE )
        sLog.outError("BattleGroundAV: cheating? a player used a gameobject which isnt a Tower or a Grave");
    else
        sLog.outError("BattleGroundAV: cheating? a player used a gameobject which isnt spawned");
    return -1;
}

void BattleGroundAV::PopulateNode(uint32 node)
{
    uint32 team = m_Points_Owner[node];
	if (team != ALLIANCE && team != HORDE)
        return;//neutral
    uint32 place = AV_CPLACE_DEFENSE_STORM_AID + ( 4 * node );
    uint32 creatureid;
    if(IsTower(node))
        creatureid=(team==ALLIANCE)?AV_NPC_A_TOWERDEFENSE:AV_NPC_H_TOWERDEFENSE;
    else
    {
        uint8 team2 = (team==ALLIANCE)? 0:1;
	if (m_Team_QuestStatus[team2][0] < 500 )
            creatureid = ( team == ALLIANCE )? AV_NPC_A_GRAVEDEFENSE0 : AV_NPC_H_GRAVEDEFENSE0;
        else if ( m_Team_QuestStatus[team2][0] < 1000 )
            creatureid = ( team == ALLIANCE )? AV_NPC_A_GRAVEDEFENSE1 : AV_NPC_H_GRAVEDEFENSE1;
        else if ( m_Team_QuestStatus[team2][0] < 1500 )
            creatureid = ( team == ALLIANCE )? AV_NPC_A_GRAVEDEFENSE2 : AV_NPC_H_GRAVEDEFENSE2;
        else
           creatureid = ( team == ALLIANCE )? AV_NPC_A_GRAVEDEFENSE3 : AV_NPC_H_GRAVEDEFENSE3;
        //spiritguide
        if( m_BgCreatures[node] )
            DelCreature(node);
        if( !AddSpiritGuide(node, BG_AV_CreaturePos[node][0], BG_AV_CreaturePos[node][1], BG_AV_CreaturePos[node][2], BG_AV_CreaturePos[node][3], team))
            sLog.outError("AV: couldn't spawn spiritguide at node %i",node);

    }
    for(uint8 i=0; i<4; i++)
        AddAVCreature(creatureid,place+i);
}
void BattleGroundAV::DePopulateNode(uint32 node)
{
    uint32 team = m_Points_Owner[node];
    if (team != ALLIANCE && team!=HORDE)
        return;//neutral
	uint32 place = AV_CPLACE_DEFENSE_STORM_AID + ( 4 * node );
    for(uint8 i=0; i<4; i++)
        if( m_BgCreatures[place+i] )
            DelCreature(place+i);
    if(IsTower(node))
        return;
    //spiritguide
    // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
    std::vector<uint64> ghost_list = m_ReviveQueue[m_BgCreatures[node]];
    if( !ghost_list.empty() )
    {
        WorldSafeLocsEntry const *ClosestGrave = NULL;
        Player *plr;
        for (std::vector<uint64>::iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
        {
            plr = objmgr.GetPlayer(*ghost_list.begin());
            if( !plr )
                continue;
            if( !ClosestGrave )
                ClosestGrave = GetClosestGraveYard(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 30, plr->GetTeam());

            plr->TeleportTo(30, ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, plr->GetOrientation());
        }
    }
    if( m_BgCreatures[node] )
        DelCreature(node);

}


uint32 BattleGroundAV::GetNodePlace(uint32 node)
{
	//warning GetNodePlace(GetNodePlace(node))!=GetNodePlace(node) in some cases, so watch out that it will not be applied 2 times
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
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_H:
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_A_B:
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_H_B:
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_CONT_A:
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_CONT_H:
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_CONT_A_B:
            if(player->GetTeam() == ALLIANCE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_CONT_H_B:
            if(player->GetTeam() == HORDE)
                return;
            EventPlayerDefendsPoint(player, GetNode(guid));
            break;
        case BG_AV_OBJECTID_BANNER_SNOWFALL_N:
            EventPlayerAssaultsPoint(player, GetNode(guid));
            break;
        default:
            break;
    }

}

void BattleGroundAV::EventPlayerDefendsPoint(Player* player, uint32 node)
{
    if((m_Points_PrevOwner[GetNodePlace(node)] != player->GetTeam() || m_Points_PrevOwner[GetNodePlace(node)] == 0) || m_Points_State[GetNodePlace(node)] != POINT_ASSAULTED)
    {
        sLog.outError("BG_AV wrong node in EventPlayerDefendsPoint (cheater,bug or gm) with node %i and player-guid: %i",node,player->GetGUID());
        return;
    }

    //spawn new go :)
	if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE && !m_Snowfall_Capped)
    { //WARNING watch out that the cheating protection at the top of this function doesn't block this..
        EventPlayerAssaultsPoint(player,node);
        return;
    }
    else
    {
        if(m_Points_Owner[GetNodePlace(node)] == ALLIANCE)
            SpawnBGObject(node+22, RESPAWN_IMMEDIATELY); //spawn horde banner
        else
            SpawnBGObject(node-22, RESPAWN_IMMEDIATELY); //spawn alliance banner
        m_Points_State[GetNodePlace(node)] = POINT_CONTROLED;
    }
    if(!IsTower(GetNodePlace(node)))
    {
        SpawnBGObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+3*GetNodePlace(node),RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+GetTeamIndexByTeamId(player->GetTeam())+3*GetNodePlace(node),RESPAWN_IMMEDIATELY);
    }
    // despawn old go
    SpawnBGObject(node, RESPAWN_ONE_DAY);

    m_Points_PrevOwner[GetNodePlace(node)] = m_Points_Owner[GetNodePlace(node)];
    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    UpdatePointsIcons(GetNodePlace(node));
	//send a nice message to all :)
	char buf[256];
	sprintf(buf, ( IsTower(GetNodePlace(node)) == true ) ? LANG_BG_AV_TOWER_DEFENDED : LANG_BG_AV_GRAVE_DEFENDED, GetNodeName(GetNodePlace(node)));
	uint8 type = ( player->GetTeam() == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
	WorldPacket data;
	ChatHandler::FillMessageData(&data, player->GetSession(), type, LANG_UNIVERSAL, NULL, player->GetGUID(), buf, NULL);
	SendPacketToAll(&data);
	//update the statistic for the defending player
	UpdatePlayerScore(player, ( IsTower(GetNodePlace(node)) == true ) ? SCORE_TOWERS_DEFENDED : SCORE_GRAVEYARDS_DEFENDED, 1);
	PopulateNode(GetNodePlace(node));
}

void BattleGroundAV::EventPlayerAssaultsPoint(Player* player, uint32 node)
{
    if(m_Points_Owner[GetNodePlace(node)] == player->GetTeam())//no idea if someone want's to assault it's own points (:
        return;

    SpawnBGObject(node, RESPAWN_ONE_DAY);
	if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE && m_Points_Owner[BG_AV_NODES_SNOWFALL_GRAVE] == 0)
    {
        if( player->GetTeam() == ALLIANCE )
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
        else
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
         m_Points_Timer[GetNodePlace(node)] = BG_AV_SNOWFALL_FIRSTCAP; //it seems that the timer is randomly 4.00,4.05, 5.00 and 5.05
         m_Points_State[GetNodePlace(node)] = POINT_ASSAULTED;
    }
    else
    {
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

        if(!IsTower(GetNodePlace(node)) &&  m_Points_Timer[GetNodePlace(node)] != BG_AV_SNOWFALL_FIRSTCAP )
        {
            //the aura doesn't change at snowfall_firstcap (neutral->neutral)
            SpawnBGObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+3*GetNodePlace(node),RESPAWN_IMMEDIATELY);
            SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+GetTeamIndexByTeamId(m_Points_Owner[GetNodePlace(node)])+3*GetNodePlace(node),RESPAWN_ONE_DAY);
        }
        DePopulateNode(GetNodePlace(node));//also moves ressurecting players to next graveyard
    }
    m_Points_PrevOwner[GetNodePlace(node)] = m_Points_Owner[GetNodePlace(node)];
    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    m_Points_State[GetNodePlace(node)] = POINT_ASSAULTED;

    UpdatePointsIcons(GetNodePlace(node));
    if( GetNodePlace(node) != BG_AV_NODES_SNOWFALL_GRAVE )
        m_Points_Timer[GetNodePlace(node)] = BG_AV_CAPTIME;

    //send a nice message to all :)
    char buf[256];
    sprintf(buf, ( IsTower(GetNodePlace(node)) ) ? LANG_BG_AV_TOWER_ASSAULTED : LANG_BG_AV_GRAVE_ASSAULTED, GetNodeName(GetNodePlace(node)),  ( player->GetTeam() == ALLIANCE ) ?  LANG_BG_AV_ALLY : LANG_BG_AV_HORDE );
    uint8 type = ( player->GetTeam() == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE;
    WorldPacket data;
    ChatHandler::FillMessageData(&data, player->GetSession(), type, LANG_UNIVERSAL, NULL, player->GetGUID(), buf, NULL);
    SendPacketToAll(&data);
    //update the statistic for the assaulting player
    UpdatePlayerScore(player, ( IsTower(GetNodePlace(node)) ) ? SCORE_TOWERS_ASSAULTED : SCORE_GRAVEYARDS_ASSAULTED, 1);
}

void BattleGroundAV::UpdatePointsIcons(uint32 node)
{
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
            } else if (m_Points_State[node] == POINT_ASSAULTED)
            {
                UpdateWorldState(AV_DUNS_CONTROLLED, 0);
                UpdateWorldState(AV_DUNS_DESTROYED, 0);
                UpdateWorldState(AV_DUNS_ASSAULTED, 1);
            } else if (m_Points_State[node] == POINT_DESTROYED)
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


WorldSafeLocsEntry const* BattleGroundAV::GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team)
{
    WorldSafeLocsEntry const* good_entry = NULL;
    if( GetStatus() != STATUS_IN_PROGRESS) //TODO: get out, if this is right (if a player dies before game starts and gets ressurected in main graveyard)
    {
        // Is there any occupied node for this team?
        std::vector<uint8> nodes;
        for (uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
            if (m_Points_Owner[i] == team && m_Points_State[i] == POINT_CONTROLED)
                nodes.push_back(i);

        // If so, select the closest node to place ghost on
        if( !nodes.empty() )
        {
            float mindist = 999999.0f;
            for (uint8 i = 0; i < nodes.size(); ++i)
            {
                WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[i] );
                if( !entry )
                    continue;
                float dist = (entry->x - x)*(entry->x - x)+(entry->y - y)*(entry->y - y);
                if( mindist > dist )
                {
                    mindist = dist;
                    good_entry = entry;
                }
            }
            nodes.clear();
        }
    }
    // If not, place ghost on starting location
    if( !good_entry )
        good_entry = sWorldSafeLocsStore.LookupEntry( BG_AV_GraveyardIds[GetTeamIndexByTeamId(team)+7] );

    return good_entry;
}


bool BattleGroundAV::SetupBattleGround()
{
    for(uint8 i=0; i<2;i++)
    {
        m_Team_Scores[i]=BG_AV_SCORE_INITIAL_POINTS;
        for(uint8 j=0; j<5;j++)
            m_Team_QuestStatus[i][j]=0;
    }
    m_IsInformedNearVictory=false;
    // Create starting objects
    if(
       // alliance gates
        !AddObject(BG_AV_OBJECT_DOOR_A, BG_AV_OBJECTID_GATE_A, BG_AV_DoorPositons[0][0],BG_AV_DoorPositons[0][1],BG_AV_DoorPositons[0][2],BG_AV_DoorPositons[0][3],0,0,sin(BG_AV_DoorPositons[0][3]/2),cos(BG_AV_DoorPositons[0][3]/2),RESPAWN_IMMEDIATELY)
        // horde gates
        || !AddObject(BG_AV_OBJECT_DOOR_H, BG_AV_OBJECTID_GATE_H, BG_AV_DoorPositons[1][0],BG_AV_DoorPositons[1][1],BG_AV_DoorPositons[1][2],BG_AV_DoorPositons[1][3],0,0,sin(BG_AV_DoorPositons[1][3]/2),cos(BG_AV_DoorPositons[1][3]/2),RESPAWN_IMMEDIATELY))
    {
        sLog.outErrorDb("BatteGroundAV: Failed to spawn some object BattleGround not created!");
        return false;
    }

//spawn graveyard flags
    for (int i = BG_AV_NODES_FIRSTAID_STATION ; i <= BG_AV_NODES_FROSTWOLF_WTOWER; ++i)
    {
        if( i <= BG_AV_NODES_FROSTWOLF_HUT )
        {
            if(    !AddObject(i,BG_AV_OBJECTID_BANNER_A_B,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+11,BG_AV_OBJECTID_BANNER_CONT_A_B,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+33,BG_AV_OBJECTID_BANNER_H_B,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+22,BG_AV_OBJECTID_BANNER_CONT_H_B,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                //aura
                || !AddObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_N,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_A,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(BG_AV_OBJECT_AURA_H_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_H,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY))
            {
                sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!");
                return false;
            }
        }
        else
        {
            if( i <= BG_AV_NODES_STONEHEART_BUNKER )
            {
                if(   !AddObject(i,BG_AV_OBJECTID_BANNER_A,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(i+22,BG_AV_OBJECTID_BANNER_CONT_H,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!");
                    return false;
                }
            }
            else
            {
                if(     !AddObject(i+7,BG_AV_OBJECTID_BANNER_CONT_A,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(i+29,BG_AV_OBJECTID_BANNER_H,BG_AV_NodePositions[i][0],BG_AV_NodePositions[i][1],BG_AV_NodePositions[i][2],BG_AV_NodePositions[i][3], 0, 0, sin(BG_AV_NodePositions[i][3]/2), cos(BG_AV_NodePositions[i][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!");
                    return false;
                }
            }
        }
    }
   if(!AddObject(BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_SNOWFALL_N ,BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][0],BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][1],BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][2],BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][3],0,0,sin(BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][3]/2), cos(BG_AV_NodePositions[BG_AV_NODES_SNOWFALL_GRAVE][3]/2), RESPAWN_ONE_DAY))
   {
        sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!");
        return false;
   }
   return true;
}

const char* BattleGroundAV::GetNodeName(uint32 node)
{
    switch (node)
    {
	case BG_AV_NODES_FIRSTAID_STATION: return "Stormpike Aid Station";
	case BG_AV_NODES_DUNBALDAR_SOUTH: return "Dun Baldar South Bunker";
	case BG_AV_NODES_DUNBALDAR_NORTH: return "Dun Baldar North Bunker";
	case BG_AV_NODES_STORMPIKE_GRAVE: return "Stormpike Graveyard";
	case BG_AV_NODES_ICEWING_BUNKER: return "Icewing Bunker";
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
    for(uint8 i=0; i<2; i++)
    {
        for(uint8 j=0; j<9; j++)
            m_Team_QuestStatus[i][j]=0;
	m_Team_Scores[i]=BG_AV_SCORE_INITIAL_POINTS;
    }
    m_Snowfall_Capped=false;

    for(uint32 i = 0; i <= BG_AV_NODES_STONEHEART_GRAVE; i++)
    {
        m_Points_Owner[i] = ALLIANCE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
    }
	for(uint32 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; i++)
    {
        m_Points_Owner[i] = ALLIANCE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
    }

    for(uint32 i = BG_AV_NODES_ICEBLOOD_GRAVE; i <= BG_AV_NODES_FROSTWOLF_HUT; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
    }
    for(uint32 i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
    }

    m_Points_Owner[BG_AV_NODES_SNOWFALL_GRAVE] = 0;
    m_Points_PrevOwner[BG_AV_NODES_SNOWFALL_GRAVE] = 0;
    m_Points_State[BG_AV_NODES_SNOWFALL_GRAVE] = POINT_NEUTRAL;

    for(uint8 i = 0; i < AV_CPLACE_MAX; i++)
        if(m_BgCreatures[i])
            DelCreature(i);


}
