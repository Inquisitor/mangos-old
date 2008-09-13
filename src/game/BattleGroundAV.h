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

#ifndef __BATTLEGROUNDAV_H
#define __BATTLEGROUNDAV_H

class BattleGround;

#define LANG_BG_AV_ALLY                  "Alliance"
#define LANG_BG_AV_HORDE                 "Horde"

#define LANG_BG_AV_TOWER_TAKEN           "%s was destroyed."
#define LANG_BG_AV_TOWER_ASSAULTED       "%s is under attack! If left unchecked, the %s will destroy it!"
#define LANG_BG_AV_TOWER_DEFENDED        "$n has defended the %s"

#define LANG_BG_AV_GRAVE_TAKEN           "The %s was taken by the %s"
#define LANG_BG_AV_GRAVE_DEFENDED        "$n has defended the %s"
#define LANG_BG_AV_GRAVE_ASSAULTED       "The %s is under attack! If left unchecked, the %s will control it!"
#define LANG_BG_AV_GRAVE_CLAIMED         "$n claims the %s! If left unchallenged, the %s will control it!"

#define LANG_BG_AV_ONEMINTOSTART         "The Battle for Alterac Valley begins in 1 minute."
#define LANG_BG_AV_HALFMINTOSTART        "The Battle for Alterac Valley begins in 30 seconds. Prepare yourselves!"
#define LANG_BG_AV_STARTED               "The Battle for Alterac Valley has begun!"
#define LANG_BG_AV_A_NEAR_VICTORY        "The Alliance is near victory!"
#define LANG_BG_AV_H_NEAR_VICTORY        "The Horde is near victory!"

#define BG_AV_SPIRITGUIDE_ENTRY_A        13116
#define BG_AV_CAPTIME                    240000  //4minutes is correct
#define BG_AV_SNOWFALL_FIRSTCAP          300000  //should be a bit longer needs confirmation
#define BG_AV_SCORE_TOWER_LOST           75
#define BG_AV_SCORE_INITIAL_POINTS       600
#define BG_AV_BANNER_RESPAWN_TIMER       3000

enum BG_AV_ObjectTypes
{
    //Initial setup
    BG_AV_OBJECT_FLAG_A_FIRSTAID_STATION    = 0,
    BG_AV_OBJECT_FLAG_A_STORMPIKE_GRAVE     = 1,
    BG_AV_OBJECT_FLAG_A_STONEHEART_GRAVE    = 2,
    BG_AV_OBJECT_FLAG_A_SNOWFALL_GRAVE      = 3,
    BG_AV_OBJECT_FLAG_A_ICEBLOOD_GRAVE      = 4,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_GRAVE     = 5,
    BG_AV_OBJECT_FLAG_A_FROSTWOLF_HUT       = 6,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_SOUTH     = 7,
    BG_AV_OBJECT_FLAG_A_DUNBALDAR_NORTH     = 8,
    BG_AV_OBJECT_FLAG_A_ICEWING_BUNKER      = 9,
    BG_AV_OBJECT_FLAG_A_STONEHEART_BUNKER   = 10,

    BG_AV_OBJECT_FLAG_C_A_FIRSTAID_STATION    = 11,
    BG_AV_OBJECT_FLAG_C_A_STORMPIKE_GRAVE     = 12,
    BG_AV_OBJECT_FLAG_C_A_STONEHEART_GRAVE    = 13,
    BG_AV_OBJECT_FLAG_C_A_SNOWFALL_GRAVE      = 14,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_GRAVE      = 15,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_GRAVE     = 16,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_HUT       = 17,
    BG_AV_OBJECT_FLAG_C_A_ICEBLOOD_TOWER      = 18,
    BG_AV_OBJECT_FLAG_C_A_TOWER_POINT         = 19,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_ETOWER    = 20,
    BG_AV_OBJECT_FLAG_C_A_FROSTWOLF_WTOWER    = 21,

    BG_AV_OBJECT_FLAG_C_H_FIRSTAID_STATION    = 22,
    BG_AV_OBJECT_FLAG_C_H_STORMPIKE_GRAVE     = 23,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_GRAVE    = 24,
    BG_AV_OBJECT_FLAG_C_H_SNOWFALL_GRAVE      = 25,
    BG_AV_OBJECT_FLAG_C_H_ICEBLOOD_GRAVE      = 26,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_GRAVE     = 27,
    BG_AV_OBJECT_FLAG_C_H_FROSTWOLF_HUT       = 28,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_SOUTH     = 29,
    BG_AV_OBJECT_FLAG_C_H_DUNBALDAR_NORTH     = 30,
    BG_AV_OBJECT_FLAG_C_H_ICEWING_BUNKER      = 31,
    BG_AV_OBJECT_FLAG_C_H_STONEHEART_BUNKER   = 32,

    BG_AV_OBJECT_FLAG_H_FIRSTAID_STATION    = 33,
    BG_AV_OBJECT_FLAG_H_STORMPIKE_GRAVE     = 34,
    BG_AV_OBJECT_FLAG_H_STONEHEART_GRAVE    = 35,
    BG_AV_OBJECT_FLAG_H_SNOWFALL_GRAVE      = 36,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_GRAVE      = 37,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_GRAVE     = 38,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_HUT       = 39,
    BG_AV_OBJECT_FLAG_H_ICEBLOOD_TOWER      = 40,
    BG_AV_OBJECT_FLAG_H_TOWER_POINT         = 41,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_ETOWER    = 42,
    BG_AV_OBJECT_FLAG_H_FROSTWOLF_WTOWER    = 43,

    BG_AV_OBJECT_FLAG_N_SNOWFALL_GRAVE      = 44,

    BG_AV_OBJECT_DOOR_H                     = 45,
    BG_AV_OBJECT_DOOR_A                     = 46,

    BG_AV_OBJECT_MAX                          = 47
};

enum BG_AV_ObjectIds
{
    //Banners
    BG_AV_OBJECTID_BANNER_A             = 178925, // can only be used by horde
    BG_AV_OBJECTID_BANNER_H             = 178943, // can only be used by alliance
    BG_AV_OBJECTID_BANNER_CONT_A        = 178940, // can only be used by horde
    BG_AV_OBJECTID_BANNER_CONT_H        = 179435, // can only be used by alliance

    BG_AV_OBJECTID_BANNER_A_B           = 178365,
    BG_AV_OBJECTID_BANNER_H_B           = 178364,
    BG_AV_OBJECTID_BANNER_CONT_A_B      = 179286,
    BG_AV_OBJECTID_BANNER_CONT_H_B      = 179287,
    BG_AV_OBJECTID_BANNER_SNOWFALL_N    = 180418,

    //Auras
    BG_AV_OBJECTID_AURA_A               = 180421,
    BG_AV_OBJECTID_AURA_H               = 180422,
    BG_AV_OBJECTID_AURA_N               = 180423,
    BG_AV_OBJECTID_AURA_A_S             = 180100,
    BG_AV_OBJECTID_AURA_H_S             = 180101,
    BG_AV_OBJECTID_AURA_N_S             = 180102,

    BG_AV_OBJECTID_GATE_A               = 180424,
    BG_AV_OBJECTID_GATE_H               = 180424,

    BG_AB_OBJECTID_FIRE                 = 179065
};

enum BG_AV_Nodes
{
    BG_AV_NODES_FIRSTAID_STATION        = 0,
    BG_AV_NODES_STORMPIKE_GRAVE         = 1,
    BG_AV_NODES_STONEHEART_GRAVE        = 2,
    BG_AV_NODES_SNOWFALL_GRAVE          = 3,
    BG_AV_NODES_ICEBLOOD_GRAVE          = 4,
    BG_AV_NODES_FROSTWOLF_GRAVE         = 5,
    BG_AV_NODES_FROSTWOLF_HUT           = 6,
    BG_AV_NODES_DUNBALDAR_SOUTH         = 7,
    BG_AV_NODES_DUNBALDAR_NORTH         = 8,
    BG_AV_NODES_ICEWING_BUNKER          = 9,
    BG_AV_NODES_STONEHEART_BUNKER       = 10,
    BG_AV_NODES_ICEBLOOD_TOWER          = 11,
    BG_AV_NODES_TOWER_POINT             = 12,
    BG_AV_NODES_FROSTWOLF_ETOWER        = 13,
    BG_AV_NODES_FROSTWOLF_WTOWER        = 14,

    BG_AV_NODES_MAX                     = 15
};

enum BG_AV_Graveyards
{
    AV_GRAVE_STORM_AID         = 751,
    AV_GRAVE_STORM_GRAVE       = 689,
    AV_GRAVE_STONE_GRAVE       = 729,
    AV_GRAVE_SNOWFALL          = 169,
    AV_GRAVE_FROSTWOLF         = 749,
    AV_GRAVE_ICE_GRAVE         = 690,
    AV_GRAVE_FROST_HUT         = 750,
    AV_GRAVE_MAIN_ALLIANCE     = 611,
    AV_GRAVE_MAIN_HORDE        = 610,
};

enum BG_AV_Creatures
{
    AV_SPIRIT_STORM_AID        = 0,
    AV_SPIRIT_STORM_GRAVE      = 1,
    AV_SPIRIT_STONE_GRAVE      = 2,
    AV_SPIRIT_SNOWFALL         = 3,
    AV_SPIRIT_FROSTWOLF        = 4,
    AV_SPIRIT_ICE_GRAVE        = 5,
    AV_SPIRIT_FROST_HUT        = 6,
    AV_SPIRIT_MAIN_ALLIANCE    = 7,
    AV_SPIRIT_MAIN_HORDE       = 8,

    BG_AV_CREATURES_MAX        = 9
};

const uint32 BG_AV_GraveyardIds[BG_AV_NODES_MAX+2]= {
  AV_GRAVE_STORM_AID,
  AV_GRAVE_STORM_GRAVE,
  AV_GRAVE_STONE_GRAVE,
  AV_GRAVE_SNOWFALL,
  AV_GRAVE_ICE_GRAVE,
  AV_GRAVE_FROSTWOLF,
  AV_GRAVE_FROST_HUT,
  AV_GRAVE_MAIN_ALLIANCE,
  AV_GRAVE_MAIN_HORDE
};

enum BG_AV_States
{
    POINT_NEUTRAL              =  0,
    POINT_ASSAULTED            =  1,
    POINT_DESTROYED            =  2,
    POINT_CONTROLED            =  3
};

enum BG_AV_WorldStates
{
    AV_Alliance_Score               = 3127,
    AV_Horde_Score                  = 3128,
    AV_SHOW_H_SCORE                 = 3133,
    AV_SHOW_A_SCORE                 = 3134,

    // Graves
    // Alliance
    //Stormpike first aid station
    AV_AID_A_C                      = 1327,
    AV_AID_A_A                      = 1328,
    AV_AID_H_C                      = 1325,
    AV_AID_H_A                      = 1326,
    //Stormpike Graveyard
    AV_PIKEGRAVE_A_C                = 1333,
    AV_PIKEGRAVE_A_A                = 1335,
    AV_PIKEGRAVE_H_C                = 1334,
    AV_PIKEGRAVE_H_A                = 1336,
    //Stoneheart Grave
    AV_STONEHEART_A_C               = 1319,
    AV_STONEHEART_A_A               = 1318, //wrong
    AV_STONEHEART_H_C               = 1317, //wrong
    AV_STONEHEART_H_A               = 1320, //wrong
    //Neutral
    //Snowfall Grave
    AV_SNOWFALL_N                   = 1966,
    AV_SNOWFALL_A_C                 = 1341,
    AV_SNOWFALL_A_A                 = 1343,
    AV_SNOWFALL_H_C                 = 1342,
    AV_SNOWFALL_H_A                 = 1344,
    //Horde
    //Iceblood grave
    AV_ICEBLOOD_A_C                 = 1346,
    AV_ICEBLOOD_A_A                 = 1348,
    AV_ICEBLOOD_H_C                 = 1347,
    AV_ICEBLOOD_H_A                 = 1349,
    //Frostwolf Grave
    AV_FROSTWOLF_A_C                = 1337,
    AV_FROSTWOLF_A_A                = 1339,
    AV_FROSTWOLF_H_C                = 1338,
    AV_FROSTWOLF_H_A                = 1340,
    //Frostwolf Hut
    AV_FROSTWOLFHUT_A_C             = 1329,
    AV_FROSTWOLFHUT_A_A             = 1331,
    AV_FROSTWOLFHUT_H_C             = 1330,
    AV_FROSTWOLFHUT_H_A             = 1332,

    //Towers
    //Alliance
    //Dunbaldar South Bunker
    AV_DUNS_CONTROLLED              = 1361,
    AV_DUNS_DESTROYED               = 1370,
    AV_DUNS_ASSAULTED               = 1378,
    //Dunbaldar North Bunker
    AV_DUNN_CONTROLLED              = 1362,
    AV_DUNN_DESTROYED               = 1371,
    AV_DUNN_ASSAULTED               = 1379,
    //Icewing Bunker
    AV_ICEWING_CONTROLLED           = 1363,
    AV_ICEWING_DESTROYED            = 1372,
    AV_ICEWING_ASSAULTED            = 1380,
    //Stoneheart Bunker
    AV_STONEH_CONTROLLED            = 1364,
    AV_STONEH_DESTROYED             = 1373,
    AV_STONEH_ASSAULTED             = 1381,
    //Horde
    //Iceblood Tower
    AV_ICEBLOOD_CONTROLLED          = 1385, //wrong
    AV_ICEBLOOD_DESTROYED           = 1368, //wrong
    AV_ICEBLOOD_ASSAULTED           = 1386, //wrong
    //Tower Point
    AV_TOWERPOINT_CONTROLLED        = 1384,
    AV_TOWERPOINT_DESTROYED         = 1367,
    AV_TOWERPOINT_ASSAULTED         = 1389,
    //Frostwolf East
    AV_FROSTWOLFE_CONTROLLED        = 1382,
    AV_FROSTWOLFE_DESTROYED         = 1365,
    AV_FROSTWOLFE_ASSAULTED         = 1387,
    //Frostwolf West
    AV_FROSTWOLFW_CONTROLLED        = 1383,
    AV_FROSTWOLFW_DESTROYED         = 1366,
    AV_FROSTWOLFW_ASSAULTED         = 1388
};

const uint32 BG_AV_State_Auras[5] = { BG_AV_OBJECTID_AURA_N, BG_AV_OBJECTID_AURA_A, BG_AV_OBJECTID_AURA_H, BG_AV_OBJECTID_AURA_N, BG_AV_OBJECTID_AURA_N };
const uint32 BG_AV_State_Auras_Small[5] = { BG_AV_OBJECTID_AURA_N,BG_AV_OBJECTID_AURA_A_S,BG_AV_OBJECTID_AURA_H_S,BG_AV_OBJECTID_AURA_N_S,BG_AV_OBJECTID_AURA_N_S };

class BattleGroundAVScore : public BattleGroundScore
{
    public:
        BattleGroundAVScore() : GraveyardsAssaulted(0), GraveyardsDefended(0), TowersAssaulted(0), TowersDefended(0), MinesCaptured(0), LeadersKilled(0), SecondaryObjectives(0) {};
        virtual ~BattleGroundAVScore() {};
        uint32 GraveyardsAssaulted;
        uint32 GraveyardsDefended;
        uint32 TowersAssaulted;
        uint32 TowersDefended;
        uint32 MinesCaptured;
        uint32 LeadersKilled;
        uint32 SecondaryObjectives;
};

class BattleGroundAV : public BattleGround
{
    friend class BattleGroundMgr;
    public:
        BattleGroundAV();
        ~BattleGroundAV();
        void Update(time_t diff);

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player *plr);

        void RemovePlayer(Player *plr,uint64 guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        bool SetupBattleGround();
        virtual void ResetBGSubclass();

        /* Nodes occupying */
        void EventPlayerClaimsPoint(Player *player, uint64 guid, uint32 entry);
        void EventPlayerAssaultsPoint(Player* player, uint32 type);
        void EventPlayerDefendsPoint(Player* player, uint32 type);
        void EventPlayerDestroyedPoint(uint32 node);
        void UpdatePointsIcons(uint32 node);
        void UpdateScore();
        void InitWorldStates();
        void UpdateNode(uint32 type, uint32 state);
        int32 GetNode(uint64 guid);
        uint32 GetNodePlace(uint32 guid);
        uint32 GetPlaceNode(uint32 node);
        const char* GetNodeName(uint32 node);
        bool IsTower(uint32 node);

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);
        void HandleKillPlayer(Player* player, Player *killer);
        void HandleKillUnit(Creature *unit, Player *killer);
        WorldSafeLocsEntry const* SelectGraveYard(Player* player);

    private:
        int32 m_Team_Scores[2];
        uint32 m_Points_Owner[BG_AV_NODES_MAX];
        uint32 m_Points_State[BG_AV_NODES_MAX];
        int32 m_Points_Timer[BG_AV_NODES_MAX];
        bool m_Snowfall_Capped;

};
#endif
