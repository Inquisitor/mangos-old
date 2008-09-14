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

#include "Player.h"
#include "BattleGround.h"
#include "BattleGroundAV.h"
#include "Creature.h"
#include "Chat.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "Language.h"
#include "SpellAuras.h"

BattleGroundAV::BattleGroundAV()
{

    m_BgObjects.resize(BG_AV_OBJECT_MAX+AV_STATICOPLACE_MAX);
    m_BgCreatures.resize(AV_CPLACE_MAX+AV_STATICCPLACE_MAX);
}

BattleGroundAV::~BattleGroundAV()
{

}

const uint16 BattleGroundAV::GetBonusHonor(uint8 kills)
{
    //bonushonor is calculated through "kills" and a kill is in every levelrange other honor worth
    uint8 honor;
    if(m_MaxLevel==70)
        return 21*kills;
    else
        return 14*kills;
}

void BattleGroundAV::HandleKillPlayer(Player *player, Player *killer)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
	UpdateScore(player->GetTeam(),-1);
}

void BattleGroundAV::HandleKillUnit(Creature *unit, Player *killer)
{
    sLog.outDebug("bg_av HandleKillUnit %i",unit->GetEntry());
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
        RewardHonorToTeam(GetBonusHonor(BG_AV_KILL_CAPTAIN),HORDE);
	    UpdateScore(ALLIANCE,(-1)*BG_AV_RES_CAPTAIN);
        //spawn destroyed aura
        for(uint8 i=0; i<=9; i++)
            SpawnBGObject(BG_AV_OBJECT_BURN_BUILDING_ALLIANCE+i,RESPAWN_IMMEDIATELY);
    }
    else if ( entry == BG_AV_CreatureInfo[AV_NPC_H_CAPTAIN][0] )
    {
        RewardReputationToTeam(730,BG_AV_REP_CAPTAIN,ALLIANCE);
        RewardHonorToTeam(GetBonusHonor(BG_AV_KILL_CAPTAIN),ALLIANCE);
	UpdateScore(HORDE,(-1)*BG_AV_RES_CAPTAIN);
        //spawn destroyed aura
        for(uint8 i=0; i<=9; i++)
            SpawnBGObject(BG_AV_OBJECT_BURN_BUILDING_HORDE+i,RESPAWN_IMMEDIATELY);
    }
    else if ( entry == BG_AV_CreatureInfo[AV_NPC_N_MINE_N_4][0] || entry == BG_AV_CreatureInfo[AV_NPC_N_MINE_A_4][0] || entry == BG_AV_CreatureInfo[AV_NPC_N_MINE_H_4][0])
    {
        DePopulateMine(AV_NORTH_MINE);
        m_Mine_Owner[AV_NORTH_MINE] = killer->GetTeam();
        PopulateMine(AV_NORTH_MINE);
    }
    else if ( entry == BG_AV_CreatureInfo[AV_NPC_S_MINE_N_4][0] || entry == BG_AV_CreatureInfo[AV_NPC_S_MINE_A_4][0] || entry == BG_AV_CreatureInfo[AV_NPC_S_MINE_H_4][0])
    {
        DePopulateMine(AV_SOUTH_MINE);
        m_Mine_Owner[AV_SOUTH_MINE] = killer->GetTeam();
        PopulateMine(AV_SOUTH_MINE);
    }
}

void BattleGroundAV::UpdateQuest(uint32 questid, Player *player)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
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
                    {
                        DePopulateNode(i);
                        PopulateNode(i);
                            //maybe this is bad, because it will instantly respawn all creatures on every grave..
                     }
            }
            break;
        case AV_QUEST_A_COMMANDER1:
        case AV_QUEST_H_COMMANDER1:
            m_Team_QuestStatus[team][1]++;
            RewardReputationToTeam(team,1,player->GetTeam());
            if(m_Team_QuestStatus[team][1] == 30)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_COMMANDER2:
        case AV_QUEST_H_COMMANDER2:
            m_Team_QuestStatus[team][2]++;
            RewardReputationToTeam(team,1,player->GetTeam());
            if(m_Team_QuestStatus[team][2] == 60)
                sLog.outDebug("BG_AV Quest %i completed (need to implement some events here",questid);
            break;
        case AV_QUEST_A_COMMANDER3:
        case AV_QUEST_H_COMMANDER3:
            m_Team_QuestStatus[team][3]++;
            RewardReputationToTeam(team,1,player->GetTeam());
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


void BattleGroundAV::UpdateScore(uint16 team, int16 points )
{
    if( team == ALLIANCE )
    {
        m_Team_Scores[0] += points;
        if( m_Team_Scores[0] < 0)
        {
            m_Team_Scores[0]=0;
            EndBattleGround(HORDE);
        }
	    UpdateWorldState(AV_Alliance_Score, m_Team_Scores[0]);
    }
    else if ( team == HORDE )
    {
        m_Team_Scores[1] += points;
        if( m_Team_Scores[1] < 0)
        {
            m_Team_Scores[1]=0;
            EndBattleGround(ALLIANCE);
        }
	    UpdateWorldState(AV_Horde_Score, m_Team_Scores[1]);
    }
    else
    {
        sLog.outError("BG_AV unknown team %i in updatescore",team);
        return;
    }

//TODO:get out at which point this message comes and which text will be displayed and also find out, if this can be displayed 2 times in a bg (for both teams)
//and surely it's better to add this code abovee
    if( points < 0 && !m_IsInformedNearVictory)
    {
        for(uint8 i=0; i<2; i++)
        {
            if( m_Team_Scores[i] < SEND_MSG_NEAR_LOSE )
            {
                if( i == BG_TEAM_ALLIANCE )
                    SendMessageToAll(LANG_BG_AV_A_NEAR_LOSE);
                else
                    SendMessageToAll(LANG_BG_AV_H_NEAR_LOSE);
//                PlaySoundToAll(SOUND_NEAR_VICTORY); - will this be played?
                m_IsInformedNearVictory = true;
            }
        }
    }
}


bool BattleGroundAV::AddAVCreature(uint8 cinfoid, uint16 type )
{
    uint32 level;
    Creature* creature;
    if(type > AV_CPLACE_MAX + AV_STATICCPLACE_MAX)
    {
        sLog.outError("BG_AV: addavcreature received strange type %i",type); //hopefully everything works..
        return false;
    }
    if(type>=AV_CPLACE_MAX) //static
    {
        type-=(AV_CPLACE_MAX);
        cinfoid=int(BG_AV_StaticCreaturePos[type][4]);

        if(cinfoid==25 || cinfoid==31) //TODO: remove this (but don't forget to remove this from the header-file
            return true;
        creature = AddCreature(BG_AV_StaticCreatureInfo[cinfoid][0],(type+AV_CPLACE_MAX),BG_AV_StaticCreatureInfo[cinfoid][1],BG_AV_StaticCreaturePos[type][0],BG_AV_StaticCreaturePos[type][1],BG_AV_StaticCreaturePos[type][2],BG_AV_StaticCreaturePos[type][3]);
        level = ( BG_AV_StaticCreatureInfo[cinfoid][2] == BG_AV_StaticCreatureInfo[cinfoid][3] ) ? BG_AV_StaticCreatureInfo[cinfoid][2] : urand(BG_AV_StaticCreatureInfo[cinfoid][2],BG_AV_StaticCreatureInfo[cinfoid][3]);
        CreatureData &data = objmgr.NewOrExistCreatureData(creature->GetDBTableGUIDLow());
        data.posX = BG_AV_StaticCreaturePos[type][0]; //this is needed, else they have a wrong aggrorange
        data.posY = BG_AV_StaticCreaturePos[type][1];
        data.posZ = BG_AV_StaticCreaturePos[type][2];
    }
    else
    {
        creature = AddCreature(BG_AV_CreatureInfo[cinfoid][0],type,BG_AV_CreatureInfo[cinfoid][1],BG_AV_CreaturePos[type][0],BG_AV_CreaturePos[type][1],BG_AV_CreaturePos[type][2],BG_AV_CreaturePos[type][3]);
        level = ( BG_AV_CreatureInfo[cinfoid][2] == BG_AV_CreatureInfo[cinfoid][3] ) ? BG_AV_CreatureInfo[cinfoid][2] : urand(BG_AV_CreatureInfo[cinfoid][2],BG_AV_CreatureInfo[cinfoid][3]);
        CreatureData &data = objmgr.NewOrExistCreatureData(creature->GetDBTableGUIDLow());
        data.posX = BG_AV_CreaturePos[type][0]; //this is needed, else they have a wrong aggrorange
        data.posY = BG_AV_CreaturePos[type][1];
        data.posZ = BG_AV_CreaturePos[type][2];
    }
    creature->LoadCreaturesAddon(true); //currently it's only for the bowman, so they have the entangling-aura.. but later it's needed for the watchdogs
    if(level != 0)
        level += m_MaxLevel-60; //maybe we can do this more generic for custom level-range.. actually it's blizzlike
    creature->SetLevel(level);
    return true;
}

void BattleGroundAV::Update(time_t diff)
{
    BattleGround::Update(diff);
    if (GetStatus() == STATUS_WAIT_JOIN && GetPlayersSize())
    {
        ModifyStartDelayTime(diff);

        if (!(m_Events & 0x01))
        {
            m_Events |= 0x01;
            uint16 i;
            sLog.outDebug("Alterac Valley: entering state STATUS_WAIT_JOIN ...");
            // Initial Nodes
            for(i = 0; i < BG_AV_OBJECT_MAX; i++)
                SpawnBGObject(i, RESPAWN_ONE_DAY);
            for(uint8 i = BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE ; i++){
                SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+3*i,RESPAWN_IMMEDIATELY);
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            }
            for(i = BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH; i <= BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER ; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            for(i = BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE; i <= BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER ; i++){
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
                if(i<=BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT)
                    SpawnBGObject(BG_AV_OBJECT_AURA_H_FIRSTAID_STATION+3*GetNodePlace(i),RESPAWN_IMMEDIATELY);
            }
            for(i = BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH; i <= BG_AV_OBJECT_TFLAG_A_STONEHEART_BUNKER; i+=2)
            {
                SpawnBGObject(i, RESPAWN_IMMEDIATELY); //flag
                SpawnBGObject(i+16, RESPAWN_IMMEDIATELY); //aura
            }
            for(i = BG_AV_OBJECT_TFLAG_H_ICEBLOOD_TOWER; i <= BG_AV_OBJECT_TFLAG_H_FROSTWOLF_WTOWER; i+=2)
            {
                SpawnBGObject(i, RESPAWN_IMMEDIATELY); //flag
                SpawnBGObject(i+16, RESPAWN_IMMEDIATELY); //aura
            }
            //snowfall and the doors
            for(i = BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE; i <= BG_AV_OBJECT_DOOR_A; i++)
                SpawnBGObject(i, RESPAWN_IMMEDIATELY);
            SpawnBGObject(BG_AV_OBJECT_AURA_N_SNOWFALL_GRAVE,RESPAWN_IMMEDIATELY);

            //creatures
            sLog.outDebug("BG_AV start poputlating nodes");
			for(i= BG_AV_NODES_FIRSTAID_STATION; i < BG_AV_NODES_MAX; i++ )
				PopulateNode(i);
            //all creatures which don't get despawned through the script are static
            sLog.outDebug("BG_AV: start spawning static creatures");
            for(i=0; i < AV_STATICCPLACE_MAX; i++ )
                AddAVCreature(0,i+AV_CPLACE_MAX);
            //the same for gameobjects:
            for(i=0; i < AV_STATICOPLACE_MAX; i++)
                SpawnBGObject(BG_AV_OBJECT_MAX+i,RESPAWN_IMMEDIATELY);
		//mainspiritguides:
            sLog.outDebug("BG_AV: start spawning spiritguides creatures");
	        AddSpiritGuide(7, BG_AV_CreaturePos[7][0], BG_AV_CreaturePos[7][1], BG_AV_CreaturePos[7][2], BG_AV_CreaturePos[7][3], ALLIANCE);
		AddSpiritGuide(8, BG_AV_CreaturePos[8][0], BG_AV_CreaturePos[8][1], BG_AV_CreaturePos[8][2], BG_AV_CreaturePos[8][3], HORDE);
            //spawn the marshals (those who get deleted, if a tower gets destroyed)
            sLog.outDebug("BG_AV: start spawning marshal creatures");
            for(i=AV_NPC_A_MARSHAL_SOUTH; i<= AV_NPC_H_MARSHAL_WTOWER; i++)
                AddAVCreature(i,AV_CPLACE_A_MARSHAL_SOUTH+(i-AV_NPC_A_MARSHAL_SOUTH));

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
            UpdateWorldState(AV_SHOW_H_SCORE, 1);
            UpdateWorldState(AV_SHOW_A_SCORE, 1);
            m_Events |= 0x10;
            SendMessageToAll(LANG_BG_AV_STARTED);

            sLog.outDebug("BG_AV: start spawning mine stuff");
            for(uint16 i= BG_AV_OBJECT_MINE_SUPPLY_N_MIN; i<=BG_AV_OBJECT_MINE_SUPPLY_N_MAX;i++)
                SpawnBGObject(i,RESPAWN_IMMEDIATELY);
            for(uint16 i= BG_AV_OBJECT_MINE_SUPPLY_S_MIN; i<=BG_AV_OBJECT_MINE_SUPPLY_S_MAX;i++)
                SpawnBGObject(i,RESPAWN_IMMEDIATELY);
            PopulateMine(AV_NORTH_MINE);
            PopulateMine(AV_SOUTH_MINE);
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
        m_Mine_Timer -=diff;
        if(m_Mine_Owner[AV_SOUTH_MINE] != BG_AV_CreatureInfo[AV_NPC_S_MINE_N_4][1])
        {
            m_Mine_Reclaim_Timer[AV_SOUTH_MINE] -= diff;
            if( m_Mine_Timer <= 0)
                UpdateScore(m_Mine_Owner[AV_SOUTH_MINE],1);
            if( m_Mine_Reclaim_Timer[AV_SOUTH_MINE] <= 0)
            {
                m_Mine_Owner[AV_SOUTH_MINE] = BG_AV_CreatureInfo[AV_NPC_S_MINE_N_4][1];
                PopulateMine(AV_SOUTH_MINE);
            }
        }
        if(m_Mine_Owner[AV_NORTH_MINE] != BG_AV_CreatureInfo[AV_NPC_N_MINE_N_4][1])
        {
            m_Mine_Reclaim_Timer[AV_NORTH_MINE] -= diff;
            if( m_Mine_Timer <= 0)
                UpdateScore(m_Mine_Owner[AV_NORTH_MINE],1);
            if( m_Mine_Reclaim_Timer[AV_NORTH_MINE] <= 0)
            {
                m_Mine_Owner[AV_NORTH_MINE] = BG_AV_CreatureInfo[AV_NPC_N_MINE_N_4][1];
                PopulateMine(AV_NORTH_MINE);
            }
        }
        if( m_Mine_Timer <= 0)
            m_Mine_Timer=AV_MINE_TICK_TIMER;

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
    BattleGroundAVScore* sc = new BattleGroundAVScore;
    m_PlayerScores[plr->GetGUID()] = sc;
    if(m_MaxLevel==0)
        m_MaxLevel=(plr->getLevel()%10 == 0)? plr->getLevel() : (plr->getLevel()-(plr->getLevel()%10))+10; //TODO: just look at the code \^_^/ --but queue-info should provide this information..

    m_PlayerScores[plr->GetGUID()] = sc;
}

void BattleGroundAV::RemovePlayer(Player* /*plr*/,uint64 /*guid*/)
{
   /*if(!plr)
    {
        sLog.outError("bg_AV no player at remove");
        return;
    }*/
    //TODO search more buffs
    plr->RemoveAurasDueToSpell(AV_BUFF_ARMOR);

    //all items found :)
    plr->DestroyItemCount( AV_ITEM_BLOOD, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_SCRAPS, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_CRYSTAL, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_A_SOLDIER, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_A_LIEUTNANT, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_A_COMMANDER, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_H_SOLDIER, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_H_LIEUTNANT, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_H_COMMANDER, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_A_HIDE, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_H_HIDE, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_IRONDEEP, 99999, true, false);
    plr->DestroyItemCount( AV_ITEM_COLDTOOTH, 99999, true, false);

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
    uint16 team = m_Points_Owner[GetNodePlace(node)];

    //despawn banner
    SpawnBGObject(node, RESPAWN_ONE_DAY);
    m_Points_PrevState[GetNodePlace(node)] = m_Points_State[GetNodePlace(node)];
    m_Points_PrevOwner[GetNodePlace(node)] = m_Points_Owner[GetNodePlace(node)];
    if( IsTower(GetNodePlace(node)) )
    {
        //despawn general
        if(m_BgCreatures[AV_CPLACE_A_MARSHAL_SOUTH+(node-BG_AV_NODES_DUNBALDAR_SOUTH)])
            DelCreature(AV_CPLACE_A_MARSHAL_SOUTH+(node-BG_AV_NODES_DUNBALDAR_SOUTH));
        else
            sLog.outError("BG_AV: playerdestroyedpoint: marshal %i doesn't exist",AV_CPLACE_A_MARSHAL_SOUTH+(node-BG_AV_NODES_DUNBALDAR_SOUTH));
        m_Points_State[GetNodePlace(node)]=POINT_DESTROYED;
        UpdateScore((team == ALLIANCE) ? HORDE : ALLIANCE, (-1)*BG_AV_RES_TOWER);
        //spawn destroyed aura
        for(uint8 i=0; i<=9; i++)
            SpawnBGObject(BG_AV_OBJECT_BURN_DUNBALDAR_SOUTH+((GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+i,RESPAWN_IMMEDIATELY);
        RewardReputationToTeam((team == ALLIANCE)?730:729,BG_AV_REP_TOWER,team);
        RewardHonorToTeam(GetBonusHonor(BG_AV_KILL_TOWER),team);
        //despawn big banner+aura on top of tower
        SpawnBGObject(BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH+((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?0:1)+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH+((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?0:1)+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),RESPAWN_ONE_DAY);
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
        if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE) //snowfall eyecandy
        {
            for(uint8 i = 0; i < 4; i++)
            {
                SpawnBGObject(((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_PA : BG_AV_OBJECT_SNOW_EYECANDY_PH)+i,RESPAWN_ONE_DAY);
                SpawnBGObject(((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_A : BG_AV_OBJECT_SNOW_EYECANDY_H)+i,RESPAWN_IMMEDIATELY);
            }
        }
    }
    UpdatePointsIcons(GetNodePlace(node));
    //send a nice message to all :)
    char buf[256];
    if( IsTower(GetNodePlace(node)) )
        sprintf(buf, LANG_BG_AV_TOWER_TAKEN , GetNodeName(GetNodePlace(node)));
    else
        sprintf(buf, LANG_BG_AV_GRAVE_TAKEN, GetNodeName(GetNodePlace(node)), ( team == ALLIANCE ) ?  LANG_BG_AV_ALLY : LANG_BG_AV_HORDE  );
    WorldPacket data;
    ChatHandler::FillMessageData(&data, NULL,( team == ALLIANCE ) ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, LANG_UNIVERSAL, NULL, 0, buf, NULL);
    SendPacketToAll(&data);
}

void BattleGroundAV::DePopulateMine(uint8 mine)
{ //mine=0 northmine mine=1 southmin
    sLog.outDebug("bg_av depopulating mine %i (0=north,1=south)",mine);
    if(mine==AV_SOUTH_MINE)
        for(uint16 i=AV_CPLACE_MINE_S_S_MIN; i <= AV_CPLACE_MINE_S_S_MAX; i++)
            if( m_BgCreatures[i] )
                DelCreature(i);
    for(uint16 i=((mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_1_MIN:AV_CPLACE_MINE_S_1_MIN); i <= ((mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_3:AV_CPLACE_MINE_S_3); i++)
        if( m_BgCreatures[i] )
            DelCreature(i);
    return;
}
//depopulate is not needed.. (afaik)
void BattleGroundAV::PopulateMine(uint8 mine)
{ //mine=0 northmine mine=1 southmin
    sLog.outDebug("bg_av populating mine %i (0=north,1=south)",mine);
    uint32 team = m_Mine_Owner[mine];
    uint16 miner;
    //also neutral team exists.. after a big time, the neutral team tries to conquer the mine
    SendMineWorldStates(mine);
    if(mine==AV_NORTH_MINE)
    {
        if(team == ALLIANCE)
            miner = AV_NPC_N_MINE_A_1;
        else if (team == HORDE)
            miner = AV_NPC_N_MINE_H_1;
        else
            miner = AV_NPC_N_MINE_N_1;
    }
    else
    {
        uint16 cinfo;
        if(team == ALLIANCE)
            miner = AV_NPC_S_MINE_A_1;
        else if (team == HORDE)
            miner = AV_NPC_S_MINE_H_1;
        else
            miner = AV_NPC_S_MINE_N_1;
       //vermin
        sLog.outDebug("spawning vermin");
        if(team == ALLIANCE)
            cinfo = AV_NPC_S_MINE_A_3;
        else if (team == HORDE)
            cinfo = AV_NPC_S_MINE_H_3;
        else
            cinfo = AV_NPC_S_MINE_N_S;
        for(uint16 i=AV_CPLACE_MINE_S_S_MIN; i <= AV_CPLACE_MINE_S_S_MAX; i++)
            AddAVCreature(cinfo,i);
    }
    for(uint16 i=( (mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_1_MIN:AV_CPLACE_MINE_S_1_MIN ); i <= ((mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_1_MAX:AV_CPLACE_MINE_S_1_MAX); i++)
        AddAVCreature(miner,i);
    //the next chooses randomly between 2 cretures
    for(uint16 i=((mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_2_MIN:AV_CPLACE_MINE_S_2_MIN); i <= ((mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_2_MAX:AV_CPLACE_MINE_S_2_MAX); i++)
        AddAVCreature(miner+(urand(1,2)),i);
    AddAVCreature(miner+3,(mine==AV_NORTH_MINE)?AV_CPLACE_MINE_N_3:AV_CPLACE_MINE_S_3);
    //because the gameobjects in this mine have changed, update all surrounding players:
/*
 * there is something wrong with plr-GetPetGUID or even with getting the right players, i don't know
 * without this code the player must force a gameobject update (walking out of the mine and back)
 * but i don't know if this code can help.. maybe someone knows an "resend_go_stuff()" function :)
    UpdateData transDataA, transDataH;
    bool gotData=false;;
    Player* plrA = NULL;
    Player* plrH = NULL;
    //cause i need a player to build the packets i make the first loop to get one(after that one the loop should be left)  player, if i got one, i make a loop where i get the objects in the mine and update those
    for(BattleGroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if(Player* plr = objmgr.GetPlayer(itr->first))
        {
            if(plr->GetTeam() == ALLIANCE && !plrA)
                plrA = plr;
            if(plr->GetTeam() == HORDE && !plrH)
                plrH = plr;
            if(plrA && plrH)
                break; //get out of the loop
        }
    }
    if( plrA || plrH )
    {
        for(uint16 i = ((mine==AV_NORTH_MINE)?BG_AV_OBJECT_MINE_SUPPLY_N_MIN:BG_AV_OBJECT_MINE_SUPPLY_N_MIN); i <= ((mine==AV_NORTH_MINE)?BG_AV_OBJECT_MINE_SUPPLY_N_MAX:BG_AV_OBJECT_MINE_SUPPLY_N_MAX); i++)
        {
            GameObject *go = GetBGObject(i);
            if(!go)
                sLog.outError("error in getting gameobject %i %i for mine %i",((m_BgObjects[i])),i,mine);
            else
            {
                //sLog.outError("starting updating objects in av");
                if(plrA)
                {
                //    sLog.outError("update alliance gos in av");
                    go->BuildCreateUpdateBlockForPlayer(&transDataA, plrA); //i get a segfault in this function with target->GetPetGUID()
                }
                if(plrH)
                {
                //    sLog.outError("update horde gos in av");
                    go->BuildCreateUpdateBlockForPlayer(&transDataH, plrH);
                }
            }
        }
    }
    WorldPacket data;
    if(plrA->GetPetGUID())
    {
        transDataA.BuildPacket(&data);
        SendPacketToTeam(plrA->GetTeam(), &data, plrA, true);
    }
    if(plrH->GetPetGUID())
    {
        transDataH.BuildPacket(&data);
        SendPacketToTeam(plrH->GetTeam(), &data, plrH, true);
    }
*/
    if(m_Mine_Owner[mine] != ALLIANCE && m_Mine_Owner[mine] != HORDE)
        m_Mine_Reclaim_Timer[mine]=AV_MINE_RECLAIM_TIMER;
    return;
}

bool BattleGroundAV::PlayerCanDoMineQuest(int32 GOId,uint32 team)
{
    if(GOId != BG_AV_OBJECTID_MINE_N)
        if(GOId != BG_AV_OBJECTID_MINE_S)
            return true; //cause it's no mine'object it is ok if this is true
    uint8 mine = (GOId==BG_AV_OBJECTID_MINE_N)?AV_NORTH_MINE:AV_SOUTH_MINE;
    return (m_Mine_Owner[mine]==team);
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
    if( m_BgCreatures[node] )
        DelCreature(node);
}


const uint8 BattleGroundAV::GetNodePlace(uint32 node)
{
	//warning GetNodePlace(GetNodePlace(node))!=GetNodePlace(node) in some cases, so watch out that it will not be applied 2 times
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
    return 0; //i don't know what i should return, maybe I should crash the server at this point :>
}

const uint32 BattleGroundAV::GetPlaceNode(uint8 node)
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
   return 0; //i don't know what i should return, maybe I should crash the server at this point :>
}


//called when using banner
void BattleGroundAV::EventPlayerClaimsPoint(Player *player, uint64 guid, uint32 entry)
{
    if(GetStatus() != STATUS_IN_PROGRESS)
        return;
    if(GetObjectType(guid) < 0)
        return;
    sLog.outDebug("BG_AV: EventPlayerClaimsPoint with guid %i",guid);
    switch(entry)
    {
        case BG_AV_OBJECTID_BANNER_A:
        case BG_AV_OBJECTID_BANNER_A_B:
        case BG_AV_OBJECTID_BANNER_H:
        case BG_AV_OBJECTID_BANNER_H_B:
        case BG_AV_OBJECTID_BANNER_SNOWFALL_N:
            EventPlayerAssaultsPoint(player, GetObjectType(guid));
            break;
        case BG_AV_OBJECTID_BANNER_CONT_A:
        case BG_AV_OBJECTID_BANNER_CONT_A_B:
        case BG_AV_OBJECTID_BANNER_CONT_H:
        case BG_AV_OBJECTID_BANNER_CONT_H_B:
            EventPlayerDefendsPoint(player, GetObjectType(guid));
            break;
        default:
            break;
    }
}

void BattleGroundAV::EventPlayerDefendsPoint(Player* player, uint32 node)
{
    if(m_Points_Owner[GetNodePlace(node)] == player->GetTeam() || m_Points_State[GetNodePlace(node)] != POINT_ASSAULTED)
        return;
    if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE && !m_Snowfall_Capped)
    {
        EventPlayerAssaultsPoint(player,node);
        return;
    }
    if(m_Points_PrevOwner[GetNodePlace(node)] != player->GetTeam())
        return;
   //spawn new go :)
    if(m_Points_Owner[GetNodePlace(node)] == ALLIANCE)
        SpawnBGObject(node+22, RESPAWN_IMMEDIATELY); //spawn horde banner
    else
        SpawnBGObject(node-22, RESPAWN_IMMEDIATELY); //spawn alliance banner

    if(!IsTower(GetNodePlace(node)))
    {
        SpawnBGObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+3*GetNodePlace(node),RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+GetTeamIndexByTeamId(player->GetTeam())+3*GetNodePlace(node),RESPAWN_IMMEDIATELY);
    }
        // despawn old go
    SpawnBGObject(node, RESPAWN_ONE_DAY);

    m_Points_PrevState[GetNodePlace(node)] = m_Points_State[GetNodePlace(node)];
    m_Points_PrevOwner[GetNodePlace(node)] = m_Points_Owner[GetNodePlace(node)];
    m_Points_State[GetNodePlace(node)] = POINT_CONTROLED;
    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    if(IsTower(GetNodePlace(node)))
    {
        //spawn big flag+aura on top of tower
        SpawnBGObject(BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==ALLIANCE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==HORDE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==ALLIANCE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==HORDE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
    }
    else if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE) //snowfall eyecandy
    {
        for(uint8 i = 0; i < 4; i++)
        {
            if(m_Snowfall_Capped)
            {
                if( m_Points_PrevState[GetNodePlace(node)] == POINT_ASSAULTED )
                    SpawnBGObject(((m_Points_PrevOwner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_PA : BG_AV_OBJECT_SNOW_EYECANDY_PH)+i,RESPAWN_ONE_DAY);
                else
                    SpawnBGObject(((m_Points_PrevOwner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_A : BG_AV_OBJECT_SNOW_EYECANDY_H)+i,RESPAWN_ONE_DAY);
            }
            SpawnBGObject(((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_A : BG_AV_OBJECT_SNOW_EYECANDY_H)+i,RESPAWN_IMMEDIATELY);
        }
    }
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
    if(m_Points_Owner[GetNodePlace(node)] == player->GetTeam())
        return;

    SpawnBGObject(node, RESPAWN_ONE_DAY);
    if(node == BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE && m_Points_Owner[BG_AV_NODES_SNOWFALL_GRAVE] == 0)
    {
        if( player->GetTeam() == ALLIANCE )
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
        else
            SpawnBGObject(BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE, RESPAWN_IMMEDIATELY);
         m_Points_Timer[GetNodePlace(node)] = BG_AV_SNOWFALL_FIRSTCAP; //it seems that the timer is randomly 4.00,4.05, 5.00 and 5.05
    }
    else
    {
        if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE && !m_Snowfall_Capped)
            if(m_Points_Owner[GetNodePlace(node)] == HORDE)
                SpawnBGObject(node-11, RESPAWN_IMMEDIATELY);
            else
                SpawnBGObject(node+11, RESPAWN_IMMEDIATELY);
        else
        {
            if(m_Points_Owner[GetNodePlace(node)] == HORDE)
                SpawnBGObject(node-22, RESPAWN_IMMEDIATELY);
            else
                SpawnBGObject(node+22, RESPAWN_IMMEDIATELY);
            if(!IsTower(GetNodePlace(node)))
            {
                SpawnBGObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+3*GetNodePlace(node),RESPAWN_IMMEDIATELY);
                SpawnBGObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+GetTeamIndexByTeamId(m_Points_Owner[GetNodePlace(node)])+3*GetNodePlace(node),RESPAWN_ONE_DAY);
            }
            DePopulateNode(GetNodePlace(node));//also moves ressurecting players to next graveyard
        }
        m_Points_Timer[GetNodePlace(node)] = BG_AV_CAPTIME;
    }
    m_Points_PrevOwner[GetNodePlace(node)] = m_Points_Owner[GetNodePlace(node)];
    m_Points_PrevState[GetNodePlace(node)] = m_Points_State[GetNodePlace(node)];
    m_Points_Owner[GetNodePlace(node)] = player->GetTeam();
    m_Points_State[GetNodePlace(node)] = POINT_ASSAULTED;
    if(IsTower(GetNodePlace(node)))
    { //spawn assaulted big flag+aura on top of tower
        SpawnBGObject(BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==ALLIANCE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==HORDE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==ALLIANCE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        SpawnBGObject(BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH+(2*(GetNodePlace(node)-BG_AV_NODES_DUNBALDAR_SOUTH)),(m_Points_Owner[GetNodePlace(node)]==HORDE)? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
    } else {
        // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
        std::vector<uint64> ghost_list = m_ReviveQueue[m_BgCreatures[node]];
        if( !ghost_list.empty() )
        {
            Player *plr;
            WorldSafeLocsEntry const *ClosestGrave = NULL;
            plr= objmgr.GetPlayer(*ghost_list.begin());
            for (std::vector<uint64>::iterator itr = ghost_list.begin(); itr != ghost_list.end(); ++itr)
            {
                plr = objmgr.GetPlayer(*ghost_list.begin());
                if( !plr )
                    continue;
                if(!ClosestGrave)
                    ClosestGrave = GetClosestGraveYard(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetMapId(), plr->GetTeam());
                plr->TeleportTo(GetMapId(), ClosestGrave->x, ClosestGrave->y, ClosestGrave->z, plr->GetOrientation());
            }
            m_ReviveQueue[m_BgCreatures[node]].empty();
        }
        if(GetNodePlace(node) == BG_AV_NODES_SNOWFALL_GRAVE) //snowfall eyecandy
        {
            for(uint8 i = 0; i < 4; i++)
            {
                if(m_Snowfall_Capped)
                {
                    if( m_Points_PrevState[GetNodePlace(node)] == POINT_ASSAULTED )
                        SpawnBGObject(((m_Points_PrevOwner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_PA : BG_AV_OBJECT_SNOW_EYECANDY_PH)+i,RESPAWN_ONE_DAY);
                    else
                        SpawnBGObject(((m_Points_PrevOwner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_A : BG_AV_OBJECT_SNOW_EYECANDY_H)+i,RESPAWN_ONE_DAY);
                }
                SpawnBGObject(((m_Points_Owner[GetNodePlace(node)]==ALLIANCE)?BG_AV_OBJECT_SNOW_EYECANDY_PA : BG_AV_OBJECT_SNOW_EYECANDY_PH)+i,RESPAWN_IMMEDIATELY);
            }
        }
    }
    UpdatePointsIcons(GetNodePlace(node));

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

const uint8 BattleGroundAV::GetWorldStateType(uint8 state, uint16 team)
{
//a_c a_a h_c h_a the positions in worldstate-array
    if(team == ALLIANCE)
    {
        if(state==POINT_CONTROLED || state==POINT_DESTROYED)
            return 0;
        if(state==POINT_ASSAULTED)
            return 1;
    }
    if(team == HORDE)
    {
        if(state==POINT_DESTROYED || state==POINT_CONTROLED)
            return 2;
        if(state==POINT_ASSAULTED)
            return 3;
    }
    //neutral stuff cant get handled
    sLog.outError("BG_AV: should update a strange worldstate state:%i team:%i",state,team);
    return 255;
}


void BattleGroundAV::UpdatePointsIcons(uint8 node)
{
    UpdateWorldState(BG_AV_WorldStates[node][GetWorldStateType(m_Points_State[node],m_Points_Owner[node])],1);
    if(m_Points_PrevState[node] == POINT_NEUTRAL)
        UpdateWorldState(AV_SNOWFALL_N,0);
    else
        UpdateWorldState(BG_AV_WorldStates[node][GetWorldStateType(m_Points_PrevState[node],m_Points_PrevOwner[node])],0);
}

void BattleGroundAV::FillInitialWorldStates(WorldPacket& data)
{
    bool stateok;
    //graveyards
    for (uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; i++)
        for (uint8 j =1; j <= 3; j+=2)
        {//j=1=assaulted j=3=controled
            stateok = (m_Points_State[i] == j);
            data << uint32(BG_AV_WorldStates[i][GetWorldStateType(j,ALLIANCE)]) << uint32((m_Points_Owner[i] == ALLIANCE && stateok)?1:0);
            data << uint32(BG_AV_WorldStates[i][GetWorldStateType(j,HORDE)]) << uint32((m_Points_Owner[i] == HORDE && stateok)?1:0);
        }
    //towers
    for (uint8 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_MAX; i++)
        for (uint8 j =1; j <= 3; j+=2)
        {//j=1=assaulted j=3=controled //i dont have j=2=destroyed cause destroyed is the same like enemy-team controll
            stateok = (m_Points_State[i] == j || (m_Points_State[i] == POINT_DESTROYED && j==3));
            data << uint32(BG_AV_WorldStates[i][GetWorldStateType(j,ALLIANCE)]) << uint32((m_Points_Owner[i] == ALLIANCE && stateok)?1:0);
            data << uint32(BG_AV_WorldStates[i][GetWorldStateType(j,HORDE)]) << uint32((m_Points_Owner[i] == HORDE && stateok)?1:0);
        }
    if(!m_Snowfall_Capped)
        data << uint32(AV_SNOWFALL_N) << uint32(1);
    data << uint32(AV_Alliance_Score)  << uint32(m_Team_Scores[0]);
    data << uint32(AV_Horde_Score) << uint32(m_Team_Scores[1]);
    if(GetStatus() == STATUS_IN_PROGRESS){
        data << uint32(AV_SHOW_A_SCORE) << uint32(1);
        data << uint32(AV_SHOW_H_SCORE) << uint32(1);
    }
    else
    {
        data << uint32(AV_SHOW_A_SCORE) << uint32(0);
        data << uint32(AV_SHOW_H_SCORE) << uint32(0);
    }
    SendMineWorldStates(AV_NORTH_MINE);
    SendMineWorldStates(AV_SOUTH_MINE);
}

void BattleGroundAV::SendMineWorldStates(uint32 mine)
{
    if(mine == AV_NORTH_MINE)
    {
        if(m_Mine_Owner[AV_NORTH_MINE] == ALLIANCE)
        {
            UpdateWorldState(AV_N_MINE_A,1);
            UpdateWorldState(AV_N_MINE_N,0);
            UpdateWorldState(AV_N_MINE_H,0);
        }
        else if(m_Mine_Owner[AV_NORTH_MINE] == HORDE)
        {
            UpdateWorldState(AV_N_MINE_A,0);
            UpdateWorldState(AV_N_MINE_N,0);
            UpdateWorldState(AV_N_MINE_H,1);
        }
        else
        {
            UpdateWorldState(AV_N_MINE_A,0);
            UpdateWorldState(AV_N_MINE_N,1);
            UpdateWorldState(AV_N_MINE_H,0);
        }
    }
    else
    {
        if(m_Mine_Owner[AV_SOUTH_MINE] == ALLIANCE)
        {
            UpdateWorldState(AV_S_MINE_A,1);
            UpdateWorldState(AV_S_MINE_N,0);
            UpdateWorldState(AV_S_MINE_H,0);
        }
        else if(m_Mine_Owner[AV_SOUTH_MINE] == HORDE)
        {
            UpdateWorldState(AV_S_MINE_A,0);
            UpdateWorldState(AV_S_MINE_N,0);
            UpdateWorldState(AV_S_MINE_H,1);
        }
        else
        {
            UpdateWorldState(AV_S_MINE_A,0);
            UpdateWorldState(AV_S_MINE_N,1);
            UpdateWorldState(AV_S_MINE_H,0);
        }
    }
}




const bool BattleGroundAV::IsTower(uint8 node)
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
    if( GetStatus() == STATUS_IN_PROGRESS) //TODO: get out, if this is right (if a player dies before game starts and gets ressurected in main graveyard)
    {
        // Is there any occupied node for this team?
        float mindist = 9999999.0f;
        for (uint8 i = BG_AV_NODES_FIRSTAID_STATION; i <= BG_AV_NODES_FROSTWOLF_HUT; ++i)
        {
            if (m_Points_Owner[i] == team && m_Points_State[i] == POINT_CONTROLED)
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
    m_Mine_Owner[AV_NORTH_MINE] = BG_AV_CreatureInfo[AV_NPC_N_MINE_N_4][1];
    m_Mine_Owner[AV_SOUTH_MINE] = BG_AV_CreatureInfo[AV_NPC_S_MINE_N_4][1];
    m_IsInformedNearVictory=false;
    // Create starting objects
    if(
       // alliance gates
        !AddObject(BG_AV_OBJECT_DOOR_A, BG_AV_OBJECTID_GATE_A, BG_AV_DoorPositons[0][0],BG_AV_DoorPositons[0][1],BG_AV_DoorPositons[0][2],BG_AV_DoorPositons[0][3],0,0,sin(BG_AV_DoorPositons[0][3]/2),cos(BG_AV_DoorPositons[0][3]/2),RESPAWN_IMMEDIATELY)
        // horde gates
        || !AddObject(BG_AV_OBJECT_DOOR_H, BG_AV_OBJECTID_GATE_H, BG_AV_DoorPositons[1][0],BG_AV_DoorPositons[1][1],BG_AV_DoorPositons[1][2],BG_AV_DoorPositons[1][3],0,0,sin(BG_AV_DoorPositons[1][3]/2),cos(BG_AV_DoorPositons[1][3]/2),RESPAWN_IMMEDIATELY))
    {
        sLog.outErrorDb("BatteGroundAV: Failed to spawn some object BattleGround not created!1");
        return false;
    }

//spawn node-objects
    for (uint8 i = BG_AV_NODES_FIRSTAID_STATION ; i < BG_AV_NODES_MAX; ++i)
    {
        if( i <= BG_AV_NODES_FROSTWOLF_HUT )
        {
            if(    !AddObject(i,BG_AV_OBJECTID_BANNER_A_B,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+11,BG_AV_OBJECTID_BANNER_CONT_A_B,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+33,BG_AV_OBJECTID_BANNER_H_B,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(i+22,BG_AV_OBJECTID_BANNER_CONT_H_B,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                //aura
                || !AddObject(BG_AV_OBJECT_AURA_N_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_N,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(BG_AV_OBJECT_AURA_A_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_A,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                || !AddObject(BG_AV_OBJECT_AURA_H_FIRSTAID_STATION+i*3,BG_AV_OBJECTID_AURA_H,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY))
            {
                sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!2");
                return false;
            }
        }
        else //towers
        {
            if( i <= BG_AV_NODES_STONEHEART_BUNKER ) //alliance towers
            {
                if(   !AddObject(i,BG_AV_OBJECTID_BANNER_A,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(i+22,BG_AV_OBJECTID_BANNER_CONT_H,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_AURA_A,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_AURA_N,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_TOWER_BANNER_A,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_TOWER_BANNER_PH,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!3");
                    return false;
                }
            }
            else //horde towers
            {
                if(     !AddObject(i+7,BG_AV_OBJECTID_BANNER_CONT_A,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(i+29,BG_AV_OBJECTID_BANNER_H,BG_AV_ObjectPos[i][0],BG_AV_ObjectPos[i][1],BG_AV_ObjectPos[i][2],BG_AV_ObjectPos[i][3], 0, 0, sin(BG_AV_ObjectPos[i][3]/2), cos(BG_AV_ObjectPos[i][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_AURA_N,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_AURA_H,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_TOWER_BANNER_PA,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY)
                    || !AddObject(BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH+(2*(i-BG_AV_NODES_DUNBALDAR_SOUTH)),BG_AV_OBJECTID_TOWER_BANNER_H,BG_AV_ObjectPos[i+8][0],BG_AV_ObjectPos[i+8][1],BG_AV_ObjectPos[i+8][2],BG_AV_ObjectPos[i+8][3], 0, 0, sin(BG_AV_ObjectPos[i+8][3]/2), cos(BG_AV_ObjectPos[i+8][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!4");
                    return false;
                }
            }
            for(uint8 j=0; j<=9; j++) //burning aura
            {
                if(!AddObject(BG_AV_OBJECT_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j,BG_AV_OBJECTID_FIRE,BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][0],BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][1],BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][2],BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][3], 0, 0, sin(BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_BURN_DUNBALDAR_SOUTH+((i-BG_AV_NODES_DUNBALDAR_SOUTH)*10)+j][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!5.%i",i);
                    return false;
                }
            }
        }
    }
    for(uint8 i=0;i<2;i++) //burning aura for buildings
    {
        for(uint8 j=0; j<=9; j++)
        {
            if(j<5)
            {
                if(!AddObject(BG_AV_OBJECT_BURN_BUILDING_ALLIANCE+(i*10)+j,BG_AV_OBJECTID_SMOKE,BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][0],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][1],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][2],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3], 0, 0, sin(BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!6.%i",i);
                    return false;
                }
            }
            else
            {
                if(!AddObject(BG_AV_OBJECT_BURN_BUILDING_ALLIANCE+(i*10)+j,BG_AV_OBJECTID_FIRE,BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][0],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][1],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][2],BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3], 0, 0, sin(BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_BURN_BUILDING_A+(i*10)+j][3]/2),RESPAWN_ONE_DAY))
                {
                    sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!7.%i",i);
                    return false;
                }
            }
        }
    }
    for(uint16 i= 0; i<=1+(BG_AV_OBJECT_MINE_SUPPLY_N_MAX-BG_AV_OBJECT_MINE_SUPPLY_N_MIN);i++)
    {
        if(!AddObject(BG_AV_OBJECT_MINE_SUPPLY_N_MIN+i,BG_AV_OBJECTID_MINE_N,BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][0],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][1],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][2],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][3], 0, 0, sin(BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_N_MIN+i][3]/2),RESPAWN_ONE_DAY))
        {
            sLog.outError("BatteGroundAV: Failed to spawn some mine supplies BattleGround not created!7.5.%i",i);
            return false;
        }
    }
    for(uint16 i= 0 ; i<=1+(BG_AV_OBJECT_MINE_SUPPLY_S_MAX-BG_AV_OBJECT_MINE_SUPPLY_S_MIN);i++)
    {
        if(!AddObject(BG_AV_OBJECT_MINE_SUPPLY_S_MIN+i,BG_AV_OBJECTID_MINE_S,BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][0],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][1],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][2],BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][3], 0, 0, sin(BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_MINE_SUPPLY_S_MIN+i][3]/2),RESPAWN_ONE_DAY))
        {
            sLog.outError("BatteGroundAV: Failed to spawn some mine supplies BattleGround not created!7.6.%i",i);
            return false;
        }
    }

    if(!AddObject(BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE, BG_AV_OBJECTID_BANNER_SNOWFALL_N ,BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][0],BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][1],BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][2],BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][3],0,0,sin(BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][3]/2), cos(BG_AV_ObjectPos[BG_AV_NODES_SNOWFALL_GRAVE][3]/2), RESPAWN_ONE_DAY))
    {
        sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!8");
        return false;
    }
    for(uint8 i = 0; i < 4; i++)
    {
        if(!AddObject(BG_AV_OBJECT_SNOW_EYECANDY_A+i, BG_AV_OBJECTID_SNOWFALL_CANDY_A ,BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][0],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][1],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][2],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3],0,0,sin(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(BG_AV_OBJECT_SNOW_EYECANDY_PA+i, BG_AV_OBJECTID_SNOWFALL_CANDY_PA ,BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][0],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][1],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][2],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3],0,0,sin(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(BG_AV_OBJECT_SNOW_EYECANDY_H+i, BG_AV_OBJECTID_SNOWFALL_CANDY_H ,BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][0],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][1],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][2],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3],0,0,sin(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), RESPAWN_ONE_DAY)
            || !AddObject(BG_AV_OBJECT_SNOW_EYECANDY_PH+i, BG_AV_OBJECTID_SNOWFALL_CANDY_PH ,BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][0],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][1],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][2],BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3],0,0,sin(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), cos(BG_AV_ObjectPos[AV_OPLACE_SNOW_1+i][3]/2), RESPAWN_ONE_DAY))
        {
            sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!9.%i",i);
            return false;
        }
    }
    for(uint8 i = 0; i< AV_STATICOPLACE_MAX; i++)
    {
        if(!AddObject(BG_AV_OBJECT_MAX+i,int(BG_AV_StaticObjectPos[i][4]) ,BG_AV_StaticObjectPos[i][0],BG_AV_StaticObjectPos[i][1],BG_AV_StaticObjectPos[i][2],BG_AV_StaticObjectPos[i][3],0,0,sin(BG_AV_StaticObjectPos[i][3]/2), cos(BG_AV_StaticObjectPos[i][3]/2), RESPAWN_ONE_DAY))
        {
            sLog.outError("BatteGroundAV: Failed to spawn some object BattleGround not created!10.%i",i);
            return false;
        }
    }
    return true;
}

const char* BattleGroundAV::GetNodeName(uint8 node)
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
}

void BattleGroundAV::ResetBGSubclass()
{
    m_MaxLevel=0;
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
        m_Points_PrevState[i] = m_Points_State[i];
    }
	for(uint32 i = BG_AV_NODES_DUNBALDAR_SOUTH; i <= BG_AV_NODES_STONEHEART_BUNKER; i++)
    {
        m_Points_Owner[i] = ALLIANCE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
        m_Points_PrevState[i] = m_Points_State[i];
    }

    for(uint32 i = BG_AV_NODES_ICEBLOOD_GRAVE; i <= BG_AV_NODES_FROSTWOLF_HUT; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
        m_Points_PrevState[i] = m_Points_State[i];
    }
    for(uint32 i = BG_AV_NODES_ICEBLOOD_TOWER; i <= BG_AV_NODES_FROSTWOLF_WTOWER; i++)
    {
        m_Points_Owner[i] = HORDE;
        m_Points_PrevOwner[i] = m_Points_Owner[i];
        m_Points_State[i] = POINT_CONTROLED;
        m_Points_PrevState[i] = m_Points_State[i];
    }

    m_Points_Owner[BG_AV_NODES_SNOWFALL_GRAVE] = 0;
    m_Points_PrevOwner[BG_AV_NODES_SNOWFALL_GRAVE] = 0;
    m_Points_State[BG_AV_NODES_SNOWFALL_GRAVE] = POINT_NEUTRAL;
    m_Points_PrevState[BG_AV_NODES_SNOWFALL_GRAVE] = m_Points_State[BG_AV_NODES_SNOWFALL_GRAVE];

    for(uint16 i = 0; i < AV_CPLACE_MAX+AV_STATICCPLACE_MAX; i++)
        if(m_BgCreatures[i])
            DelCreature(i);

}

