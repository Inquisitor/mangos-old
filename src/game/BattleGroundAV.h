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
#define LANG_BG_AV_A_NEAR_LOSE           "The Alliance has only few Points left!" //TODO: look for the right sentence
#define LANG_BG_AV_H_NEAR_LOSE           "The Horde has only few Points left!"      //TODO: here also..

#define SEND_MSG_NEAR_LOSE               120
#define BG_AV_SPIRITGUIDE_ENTRY_A        13116
#define BG_AV_CAPTIME                    240000  //4:00
#define BG_AV_SNOWFALL_FIRSTCAP          300000  //5:00 but i also have seen 4:05
#define BG_AV_SCORE_INITIAL_POINTS       600
#define BG_AV_BANNER_RESPAWN_TIMER       3000

#define BG_AV_REP_BOSS                  100 //not sure
#define BG_AV_HONOR_BOSS                80 //not sure (4kills)
#define BG_AV_REP_CAPTAIN               125 //not sure
#define BG_AV_KILL_CAPTAIN              3
#define BG_AV_RES_CAPTAIN               100
#define BG_AV_REP_TOWER                 12
#define BG_AV_KILL_TOWER                 3
#define BG_AV_RES_TOWER                 75
#define BG_AV_KILL_COMMANDER            1 //for a safely returned wingcommander
//bonushonor at the end
#define BG_AV_SURVIVING_TOWER           2
#define BG_AV_SURVIVING_CAPTAIN         2

enum BG_AV_OTHER_VALUES
{
    AV_STATICCPLACE_MAX        = 128,
    AV_STATICOPLACE_MAX        = 83,
    AV_NORTH_MINE              = 0,
    AV_SOUTH_MINE              = 1
};
enum BG_AV_ObjectIds
{
    //cause the mangos-system is a bit different, we don't use the right go-ids for every node.. if we want to be 100% like another big server, we must take one object for every node
    //snowfall 4flags as eyecandy 179424 (alliance neutral)
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


    //snowfall eyecandy banner:
    BG_AV_OBJECTID_SNOWFALL_CANDY_A     = 179044,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PA    = 179424,
    BG_AV_OBJECTID_SNOWFALL_CANDY_H     = 179064,
    BG_AV_OBJECTID_SNOWFALL_CANDY_PH    = 179425,

    //banners on top of towers:
    BG_AV_OBJECTID_TOWER_BANNER_A  = 178927, //[PH] Alliance A1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_H  = 178955, //[PH] Horde H1 Tower Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PA = 179446, //[PH] Alliance H1 Tower Pre-Banner BIG
    BG_AV_OBJECTID_TOWER_BANNER_PH = 179436, //[PH] Horde A1 Tower Pre-Banner BIG

    //Auras
    BG_AV_OBJECTID_AURA_A               = 180421,
    BG_AV_OBJECTID_AURA_H               = 180422,
    BG_AV_OBJECTID_AURA_N               = 180423,
    BG_AV_OBJECTID_AURA_A_S             = 180100,
    BG_AV_OBJECTID_AURA_H_S             = 180101,
    BG_AV_OBJECTID_AURA_N_S             = 180102,

    BG_AV_OBJECTID_GATE_A               = 180424,
    BG_AV_OBJECTID_GATE_H               = 180424,

    //mine supplies
    BG_AV_OBJECTID_MINE_IRONDEEP        = 178785,
    BG_AV_OBJECTID_MINE_COLDTOOTH       = 178784,

    BG_AV_OBJECTID_FIRE                 = 179065,
    BG_AV_OBJECTID_SMOKE                 = 179066
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
//auras for graveyards (3auras per graveyard neutral,alliance,horde)
    BG_AV_OBJECT_AURA_N_FIRSTAID_STATION    = 47,
    BG_AV_OBJECT_AURA_A_FIRSTAID_STATION    = 48,
    BG_AV_OBJECT_AURA_H_FIRSTAID_STATION    = 49,
    BG_AV_OBJECT_AURA_N_STORMPIKE_GRAVE     = 50,
    BG_AV_OBJECT_AURA_A_STORMPIKE_GRAVE     = 51,
    BG_AV_OBJECT_AURA_H_STORMPIKE_GRAVE     = 52,
    BG_AV_OBJECT_AURA_N_STONEHEART_GRAVE    = 53,
    BG_AV_OBJECT_AURA_A_STONEHEART_GRAVE    = 54,
    BG_AV_OBJECT_AURA_H_STONEHEART_GRAVE    = 55,
    BG_AV_OBJECT_AURA_N_SNOWFALL_GRAVE      = 56,
    BG_AV_OBJECT_AURA_A_SNOWFALL_GRAVE      = 57,
    BG_AV_OBJECT_AURA_H_SNOWFALL_GRAVE      = 58,
    BG_AV_OBJECT_AURA_N_ICEBLOOD_GRAVE      = 59,
    BG_AV_OBJECT_AURA_A_ICEBLOOD_GRAVE      = 60,
    BG_AV_OBJECT_AURA_H_ICEBLOOD_GRAVE      = 61,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_GRAVE     = 62,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_GRAVE     = 63,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_GRAVE     = 64,
    BG_AV_OBJECT_AURA_N_FROSTWOLF_HUT       = 65,
    BG_AV_OBJECT_AURA_A_FROSTWOLF_HUT       = 66,
    BG_AV_OBJECT_AURA_H_FROSTWOLF_HUT       = 67,

    //big flags on top of towers 2 flags on each (contested,(alliance | horde)) + 2 auras
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_SOUTH     = 67,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_SOUTH     = 68,
    BG_AV_OBJECT_TFLAG_A_DUNBALDAR_NORTH     = 69,
    BG_AV_OBJECT_TFLAG_H_DUNBALDAR_NORTH     = 70,
    BG_AV_OBJECT_TFLAG_A_ICEWING_BUNKER      = 71,
    BG_AV_OBJECT_TFLAG_H_ICEWING_BUNKER      = 72,
    BG_AV_OBJECT_TFLAG_A_STONEHEART_BUNKER   = 73,
    BG_AV_OBJECT_TFLAG_H_STONEHEART_BUNKER   = 74,
    BG_AV_OBJECT_TFLAG_A_ICEBLOOD_TOWER      = 75,
    BG_AV_OBJECT_TFLAG_H_ICEBLOOD_TOWER      = 76,
    BG_AV_OBJECT_TFLAG_A_TOWER_POINT         = 77,
    BG_AV_OBJECT_TFLAG_H_TOWER_POINT         = 78,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_ETOWER    = 79,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_ETOWER    = 80,
    BG_AV_OBJECT_TFLAG_A_FROSTWOLF_WTOWER    = 81,
    BG_AV_OBJECT_TFLAG_H_FROSTWOLF_WTOWER    = 82,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_SOUTH     = 83,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_SOUTH     = 84,
    BG_AV_OBJECT_TAURA_A_DUNBALDAR_NORTH     = 85,
    BG_AV_OBJECT_TAURA_H_DUNBALDAR_NORTH     = 86,
    BG_AV_OBJECT_TAURA_A_ICEWING_BUNKER      = 87,
    BG_AV_OBJECT_TAURA_H_ICEWING_BUNKER      = 88,
    BG_AV_OBJECT_TAURA_A_STONEHEART_BUNKER   = 89,
    BG_AV_OBJECT_TAURA_H_STONEHEART_BUNKER   = 90,
    BG_AV_OBJECT_TAURA_A_ICEBLOOD_TOWER      = 91,
    BG_AV_OBJECT_TAURA_H_ICEBLOOD_TOWER      = 92,
    BG_AV_OBJECT_TAURA_A_TOWER_POINT         = 93,
    BG_AV_OBJECT_TAURA_H_TOWER_POINT         = 94,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_ETOWER    = 95,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_ETOWER    = 96,
    BG_AV_OBJECT_TAURA_A_FROSTWOLF_WTOWER    = 97,
    BG_AV_OBJECT_TAURA_H_FROSTWOLF_WTOWER    = 98,

    BG_AV_OBJECT_BURN_DUNBALDAR_SOUTH        = 99,
    BG_AV_OBJECT_BURN_DUNBALDAR_NORTH        = 109,
    BG_AV_OBJECT_BURN_ICEWING_BUNKER         = 119,
    BG_AV_OBJECT_BURN_STONEHEART_BUNKER      = 129,
    BG_AV_OBJECT_BURN_ICEBLOOD_TOWER         = 139,
    BG_AV_OBJECT_BURN_TOWER_POINT            = 149,
    BG_AV_OBJECT_BURN_FROSTWOLF_ETWOER       = 159,
    BG_AV_OBJECT_BURN_FROSTWOLF_WTOWER       = 169,
    BG_AV_OBJECT_BURN_BUILDING_ALLIANCE      = 179,
    BG_AV_OBJECT_BURN_BUILDING_HORDE         = 189,
    BG_AV_OBJECT_SNOW_EYECANDY_A             = 199,
    BG_AV_OBJECT_SNOW_EYECANDY_PA            = 203,
    BG_AV_OBJECT_SNOW_EYECANDY_H             = 207,
    BG_AV_OBJECT_SNOW_EYECANDY_PH            = 211,


    BG_AV_OBJECT_MAX                          = 215
};



enum BG_AV_OBJECTS
{
    AV_OPLACE_FIRSTAID_STATION          = 0,
    AV_OPLACE_STORMPIKE_GRAVE         = 1,
    AV_OPLACE_STONEHEART_GRAVE        = 2,
    AV_OPLACE_SNOWFALL_GRAVE          = 3,
    AV_OPLACE_ICEBLOOD_GRAVE          = 4,
    AV_OPLACE_FROSTWOLF_GRAVE         = 5,
    AV_OPLACE_FROSTWOLF_HUT           = 6,
    AV_OPLACE_DUNBALDAR_SOUTH         = 7,
    AV_OPLACE_DUNBALDAR_NORTH         = 8,
    AV_OPLACE_ICEWING_BUNKER          = 9,
    AV_OPLACE_STONEHEART_BUNKER       = 10,
    AV_OPLACE_ICEBLOOD_TOWER          = 11,
    AV_OPLACE_TOWER_POINT             = 12,
    AV_OPLACE_FROSTWOLF_ETOWER        = 13,
    AV_OPLACE_FROSTWOLF_WTOWER        = 14,
    AV_OPLACE_BIGBANNER_DUNBALDAR_SOUTH         = 15,
    AV_OPLACE_BIGBANNER_DUNBALDAR_NORTH         = 16,
    AV_OPLACE_BIGBANNER_ICEWING_BUNKER          = 17,
    AV_OPLACE_BIGBANNER_STONEHEART_BUNKER       = 18,
    AV_OPLACE_BIGBANNER_ICEBLOOD_TOWER          = 19,
    AV_OPLACE_BIGBANNER_TOWER_POINT             = 20,
    AV_OPLACE_BIGBANNER_FROSTWOLF_ETOWER        = 21,
    AV_OPLACE_BIGBANNER_FROSTWOLF_WTOWER        = 22,

    AV_OPLACE_BURN_DUNBALDAR_SOUTH         = 23,
    AV_OPLACE_BURN_DUNBALDAR_NORTH         = 33,
    AV_OPLACE_BURN_ICEWING_BUNKER          = 43,
    AV_OPLACE_BURN_STONEHEART_BUNKER       = 53,
    AV_OPLACE_BURN_ICEBLOOD_TOWER          = 63,
    AV_OPLACE_BURN_TOWER_POINT             = 73,
    AV_OPLACE_BURN_FROSTWOLF_ETOWER        = 83,
    AV_OPLACE_BURN_FROSTWOLF_WTOWER        = 93,
    AV_OPLACE_BURN_BUILDING_A        = 103,
    AV_OPLACE_BURN_BUILDING_H        = 113,
    AV_OPLACE_SNOW_1                      = 123,
    AV_OPLACE_SNOW_2                      = 124,
    AV_OPLACE_SNOW_3                      = 125,
    AV_OPLACE_SNOW_4                      = 126,
    AV_OPLACE_MAX                         = 127
};
const float BG_AV_ObjectPos[AV_OPLACE_MAX][4] = {
    {638.592f,-32.422f,46.0608f,-1.62316f },//firstaid station - OK
    {669.007f,-294.078f,30.2909f,2.77507f },//stormpike -OK
    {77.8013f,-404.7f,46.7549f,-0.872665f },//stone grave - OK
    {-202.581f,-112.73f,78.4876f,-0.715585f },//snowfall - OK
    {-611.962f,-396.17f,60.8351f,2.53682f},  //ice grave
    {-1082.45f,-346.823f,54.9219f,-1.53589f },//frostwolf grave - OK
    {-1402.21f,-307.431f,89.4424f,0.191986f },//frostwolf hut - OK
    {553.779f,-78.6566f,51.9378f,-1.22173f }, //dunnbaldar south - OK
    {674.001f,-143.125f,63.6615f,0.994838f }, //dunbaldar north - OK
    {203.281f,-360.366f,56.3869f,-0.925024f }, //icew - OK
    {-152.437f,-441.758f,40.3982f,-1.95477f }, //stone - OK
    {-571.88f,-262.777f,75.0087f,-0.802851f }, //ice tower - OK
    {-768.907f,-363.71f,90.8949f,1.07991f},  //tower point
    {-1302.9f,-316.981f,113.867f,2.00713f }, //frostwolf etower - OK
    {-1297.5f,-266.767f,114.15f,3.31044f},   //frostwolf wtower
    //bigbanner: all ok
    {555.848f,-84.4151f,64.4397f,3.12414f }, //duns
    {679.339f,-136.468f,73.9626f,-2.16421f }, //dunn
    {208.973f,-365.971f,66.7409f,-0.244346f }, //icew
    {-155.832f,-449.401f,52.7306f,0.610865f }, //stone
    {-572.329f,-262.476f,88.6496f,-0.575959f }, //icetower
    {-768.199f,-363.105f,104.537f,0.10472f }, //towerp
    {-1302.84f,-316.582f,127.516f,0.122173f }, //etower
    {-1297.87f,-266.762f,127.796f,0.0698132f }, //wtower
    //burning auras towers have 9*179065 captain-buildings have 5*179066+5*179065
    //dunns is missing :-/
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    //dunn
    {664.797f,-143.65f,64.1784f,-0.453786f},
    {664.505f,-139.452f,49.6696f,-0.0349067f},
    {676.067f,-124.319f,49.6726f,-1.01229f},
    {693.004f,-144.025f,64.1755f,2.44346f},
    {661.175f,-117.691f,49.645f,1.91986f},
    {684.423f,-146.582f,63.6662f,0.994838f},
    {682.791f,-127.769f,62.4155f,1.09956f},
    {674.576f,-147.101f,56.5425f,-1.6057f},
    {655.719f,-126.673f,49.8138f,2.80998f},
    {0,0,0,0},
    //icew
    {231.503f,-356.688f,42.3704f,0.296706f},
    {224.989f,-348.175f,42.5607f,1.50098f},
    {205.782f,-351.335f,56.8998f,1.01229f},
    {196.605f,-369.187f,56.3914f,2.46091f},
    {210.619f,-376.938f,49.2677f,2.86234f},
    {209.647f,-352.632f,42.3959f,-0.698132f},
    {220.65f,-368.132f,42.3978f,-0.2618f},
    {224.682f,-374.031f,57.0679f,0.541052f},
    {200.26f,-359.968f,49.2677f,-2.89725f},
    {196.619f,-378.016f,56.9131f,1.01229f},
    //stone
    {-155.488f,-437.356f,33.2796f,2.60054f},
    {-163.441f,-454.188f,33.2796f,1.93732f},
    {-143.977f,-445.148f,26.4097f,-1.8675f},
    {-135.764f,-464.708f,26.3823f,2.25147f},
    {-154.076f,-466.929f,41.0636f,-1.8675f},
    {-149.908f,-460.332f,26.4083f,-2.09439f},
    {-151.638f,-439.521f,40.3797f,0.436332f},
    {-131.301f,-454.905f,26.5771f,2.93215f},
    {-171.291f,-444.684f,40.9211f,2.30383f},
    {-143.591f,-439.75f,40.9275f,-1.72788f},
    //iceblood
    {-572.667f,-267.923f,56.8542f,2.35619f},
    {-561.021f,-262.689f,68.4589f,1.37881f},
    {-572.538f,-262.649f,88.6197f,1.8326f},
    {-574.77f,-251.45f,74.9422f,-1.18682f},
    {-578.625f,-267.571f,68.4696f,0.506145f},
    {-571.476f,-257.234f,63.3223f,3.10669f},
    {-566.035f,-273.907f,52.9582f,-0.890118f},
    {-580.948f,-259.77f,68.4696f,1.46608f},
    {-568.318f,-267.1f,75.0008f,1.01229f},
    {-559.621f,-268.597f,52.8986f,0.0523599f},
    //towerp
    {-776.072f,-368.046f,84.3558f,2.63545f},
    {-777.564f,-368.521f,90.6701f,1.72788f},
    {-765.461f,-357.711f,90.888f,0.314159f},
    {-768.763f,-362.735f,104.612f,1.81514f},
    {-760.356f,-358.896f,84.3558f,2.1293f},
    {-771.967f,-352.838f,84.3484f,1.74533f},
    {-773.333f,-364.653f,79.2351f,-1.64061f},
    {-764.109f,-366.069f,70.0934f,0.383972f},
    {-767.103f,-350.737f,68.7933f,2.80998f},
    {-760.115f,-353.845f,68.8633f,1.79769f},
    //froste
    {-1304.87f,-304.525f,91.8366f,-0.680679f},
    {-1301.77f,-310.974f,95.8252f,0.907571f},
    {-1305.58f,-320.625f,102.166f,-0.558505f},
    {-1294.27f,-323.468f,113.893f,-1.67552f},
    {-1302.65f,-317.192f,127.487f,2.30383f},
    {-1293.89f,-313.478f,107.328f,1.6057f},
    {-1312.41f,-312.999f,107.328f,1.5708f},
    {-1311.57f,-308.08f,91.7666f,-1.85005f},
    {-1314.7f,-322.131f,107.36f,0.645772f},
    {-1304.6f,-310.754f,113.859f,-0.401426f},
    //frostw
    {-1308.24f,-273.26f,92.0514f,-0.139626f},
    {-1302.26f,-262.858f,95.9269f,0.418879f},
    {-1297.28f,-267.773f,126.756f,2.23402f},
    {-1299.08f,-256.89f,114.108f,-2.44346f},
    {-1303.41f,-268.237f,114.151f,-1.23918f},
    {-1304.43f,-273.682f,107.612f,0.244346f},
    {-1309.53f,-265.951f,92.1418f,-2.49582f},
    {-1295.55f,-263.865f,105.033f,0.925024f},
    {-1294.71f,-281.466f,107.664f,-1.50098f},
    {-1289.69f,-259.521f,107.612f,-2.19912f},

    //the two buildings of the captains
    //alliance
    {-64.4987f,-289.33f,33.4616f,-2.82743f},
    {-5.98025f,-326.144f,38.8538f,0},
    {-2.67893f,-306.998f,33.4165f,0},
    {-60.25f,-309.232f,50.2408f,-1.46608f},
    {-48.7941f,-266.533f,47.7916f,2.44346f},
    {-3.40929f,-306.288f,33.34f,0},
    {-48.619f,-266.917f,47.8168f,0},
    {-62.9474f,-286.212f,66.7288f,0},
    {-5.05132f,-325.323f,38.8536f,0},
    {-64.2677f,-289.412f,33.469f,0},
//horde
    {-524.276f,-199.6f,82.8733f,-1.46608f},
    {-518.196f,-173.085f,102.43f,0},
    {-500.732f,-145.358f,88.5337f,2.44346f},
    {-501.084f,-150.784f,80.8506f,0},
    {-518.309f,-163.963f,102.521f,2.96706f},
    {-517.053f,-200.429f,80.759f,0},
    {-514.361f,-163.864f,104.163f,0},
    {-568.04f,-188.707f,81.55f,0},
    {-501.775f,-151.581f,81.2027f,0},
    {-509.975f,-191.652f,83.2978f,0},

//snowfall eyecandy
    {-191.153f,-129.868f,78.5595f,-1.25664f },
    {-201.282f,-134.319f,78.6753f,-0.942478f },
    {-215.981f,-91.4101f,80.8702f,-1.74533f },
    {-200.465f,-96.418f,79.7587f,1.36136f }
};

const float BG_AV_StaticObjectPos[AV_STATICOPLACE_MAX][5] = {
    {-1423.16f,-318.436f,89.1136f,2.35619f,2061 },//2061 - Campfire
    {-1290.27f,-172.471f,72.1853f,3.06305f,2061 },//2061 - Campfire
    {-1286.25f,-184.48f,71.8334f,-2.05076f,2061 },//2061 - Campfire
    {-1282.21f,-284.083f,87.256f,-0.0610855f,2061 },//2061 - Campfire
    {-1280.17f,-220.537f,72.2686f,1.62316f,2061 },//2061 - Campfire
    {-1256.55f,-280.278f,73.9473f,-0.0610855f,2061 },//2061 - Campfire
    {-1241.25f,-345.115f,59.6867f,0.357794f,2061 },//2061 - Campfire
    {-1215.59f,-371.946f,56.5293f,0.357794f,2061 },//2061 - Campfire
    {-1202.8f,-271.599f,72.5805f,0.357794f,2061 },//2061 - Campfire
    {-1140.82f,-343.392f,50.9077f,-2.7838f,2061 },//2061 - Campfire
    {-1139.68f,-356.288f,51.264f,-0.706858f,2061 },//2061 - Campfire
    {-1099.21f,-266.231f,57.8849f,-2.28638f,2061 },//2061 - Campfire
    {-1082.6f,-266.681f,57.8575f,2.40855f,2061 },//2061 - Campfire
    {30.4168f,-428.853f,41.528f,-2.59181f,2061 },//2061 - Campfire
    {31.2216f,-428.08f,41.528f,0.549779f,2061 },//2061 - Campfire
    {50.6401f,-421.166f,44.7325f,-0.00875192f,2061 },//2061 - Campfire
    {-743.427f,-398.242f,76.4266f,0.872664f,2066 },//2066 - Bonfire Damage
    {-1235.57f,-241.478f,73.4377f,1.48353f,3832 },//3832 - Burning Embers
    {-1248.85f,-254.06f,73.4377f,1.48353f,3833 },//3833 - Burning Embers
    {-1248.15f,-245.599f,73.4377f,-0.0523605f,3834 },//3834 - Burning Embers
    {-1237.21f,-260.168f,73.4377f,1.48353f,3835 },//3835 - Burning Embers
    {-1260.37f,-248.767f,77.9454f,1.48353f,3836 },//3836 - Burning Embers
    {-1249.32f,-244.907f,92.3372f,0.401426f,3837 },//3837 - Burning Embers
    {-1250.09f,-254.604f,92.3015f,0.148352f,3838 },//3838 - Burning Embers
    {50.6401f,-421.166f,44.7325f,-0.00875192f,22205 },//22205 - Dwarven Fire
    {30.4168f,-428.853f,41.528f,-2.59181f,22207 },//22207 - Dwarven Fire
    {31.2216f,-428.08f,41.528f,0.549779f,22208 },//22208 - Dwarven Fire
    {41.1672f,-426.866f,44.6828f,2.7838f,28048 },//28048 - Wooden Chair
    {39.0988f,-425.746f,44.688f,-0.619592f,28049 },//28049 - Wooden Chair
    {25.2482f,-433.104f,47.6369f,2.38237f,28605 },//28605 - Wooden Chair
    {25.758f,-425.837f,47.6369f,-1.98095f,28606 },//28606 - Wooden Chair
    {27.6786f,-427.69f,47.6369f,-2.67908f,28607 },//28607 - Wooden Chair
    {-1338.28f,-297.628f,91.4867f,0.248971f,29784 },//29784 - Basic Campfire
    {-1338.28f,-297.628f,91.4867f,0.248971f,31442 },//31442 - Basic Campfire
    {-1250.39f,-310.191f,61.185f,-1.10828f,50984 },//50984 - Heated Forge
    {-1245.2f,-307.059f,63.3199f,3.07959f,51704 },//51704 - Anvil
    {-1258.23f,-310.977f,63.2015f,0.862906f,51705 },//51705 - Anvil
    {-1290.27f,-172.471f,72.1853f,3.06305f,177261 },//177261 - Brazier
    {-1286.25f,-184.48f,71.8334f,-2.05076f,177262 },//177262 - Campfire
    {-1280.17f,-220.537f,72.2686f,1.62316f,177263 },//177263 - Campfire
    {-1099.21f,-266.231f,57.8849f,-2.28638f,177292 },//177292 - Brazier
    {-1082.6f,-266.681f,57.8575f,2.40855f,177293 },//177293 - Brazier
    {-1140.82f,-343.392f,50.9077f,-2.7838f,177405 },//177405 - Brazier
    {-1139.68f,-356.288f,51.264f,-0.706858f,177406 },//177406 - Brazier
    {-743.427f,-398.242f,76.4266f,0.872664f,177408 },//177408 - Bonfire
    {-1215.59f,-371.946f,56.5293f,0.357794f,177409 },//177409 - Brazier
    {-1241.25f,-345.115f,59.6867f,0.357794f,177410 },//177410 - Brazier
    {-1202.8f,-271.599f,72.5805f,0.357794f,177411 },//177411 - Brazier
    {-1282.21f,-284.083f,87.256f,-0.0610855f,177412 },//177412 - Brazier
    {-1256.55f,-280.278f,73.9473f,-0.0610855f,177413 },//177413 - Brazier
    {649.265f,-59.1102f,41.5476f,-2.68781f,178684 },//178684 - Forge
    {646.207f,-57.2428f,41.6587f,-0.157079f,178685 },//178685 - Anvil
    {-145.341f,-444.846f,26.4163f,-0.0523596f,179384 },//179384 - Chair
    {560.834f,-75.4266f,37.9558f,0.785398f,179384 },//179384 - Chair
    {-155.405f,-440.24f,33.2862f,2.34747f,179385 },//179385 - Chair
    {550.678f,-79.8234f,44.8257f,-3.09796f,179385 },//179385 - Chair
    {-150.787f,-459.829f,26.4163f,0.558507f,179386 },//179386 - Chair
    {568.326f,-89.4992f,37.9558f,1.39626f,179386 },//179386 - Chair
    {-153.748f,-438.639f,33.2862f,-2.88852f,179387 },//179387 - Chair
    {550.597f,-77.5213f,44.8257f,-2.05076f,179387 },//179387 - Chair
    {-149.057f,-461.089f,26.4163f,1.38754f,179388 },//179388 - Chair
    {570.419f,-89.0567f,37.9558f,2.2253f,179388 },//179388 - Chair
    {-168.342f,-458.4f,33.2862f,-0.445059f,179389 },//179389 - Chair
    {555.517f,-101.589f,44.8257f,0.392699f,179389 },//179389 - Chair
    {-142.968f,-444.076f,26.4163f,-2.23402f,179390 },//179390 - Chair
    {561.851f,-73.1481f,37.9558f,-1.39626f,179390 },//179390 - Chair
    {-172.363f,-452.824f,33.2796f,0.62832f,179391 },//179391 - Anvil
    {548.682f,-100.846f,44.8191f,1.46608f,179391 },//179391 - Anvil
    {-171.282f,-456.892f,33.2796f,1.63188f,179392 },//179392 - Anvil
    {552.429f,-102.764f,44.8191f,2.46964f,179392 },//179392 - Anvil
    {-172.356f,-453.88f,33.2796f,0.322886f,179393 },//179393 - Anvil
    {549.472f,-101.547f,44.8191f,1.16064f,179393 },//179393 - Anvil
    {-171.882f,-454.632f,33.2796f,0.759218f,179394 },//179394 - Anvil
    {550.347f,-101.698f,44.8191f,1.59698f,179394 },//179394 - Anvil
    {-171.656f,-455.671f,33.2796f,0.0610861f,179395 },//179395 - Anvil
    {551.271f,-102.226f,44.8191f,0.898843f,179395 },//179395 - Anvil
    {-170.699f,-458.41f,33.2796f,-1.02974f,179396 },//179396 - Anvil
    {553.947f,-103.347f,44.8191f,-0.191986f,179396 },//179396 - Anvil
    {-170.14f,-457.609f,33.2796f,-0.680678f,179397 },//179397 - Anvil
    {553.725f,-102.396f,44.8191f,0.15708f,179397 },//179397 - Anvil
    {-1423.16f,-318.436f,89.1136f,2.35619f,179419 },//179419 - Brazier
    {618.748f,-52.1126f,42.1122f,-0.0698131f,179437 },//179437 - Wanted: ORCS!
    {-1181.1f,-370.747f,53.6246f,2.68781f,179438 }//179438 - Wanted: DWARVES!
};



const float BG_AV_DoorPositons[2][4] = {
    {780.487f, -493.024f, 99.9553f, 3.0976f},   //alliance
    {-1375.193f, -538.981f, 55.2824f, 0.72178f} //horde
};


//creaturestuff starts here
//is related to BG_AV_CreaturePos
enum BG_AV_CreaturePlace
{
    AV_CPLACE_SPIRIT_STORM_AID      = 0,
    AV_CPLACE_SPIRIT_STORM_GRAVE    = 1,
    AV_CPLACE_SPIRIT_STONE_GRAVE    = 2,
    AV_CPLACE_SPIRIT_SNOWFALL       = 3,
    AV_CPLACE_SPIRIT_FROSTWOLF      = 4,
    AV_CPLACE_SPIRIT_ICE_GRAVE      = 5,
    AV_CPLACE_SPIRIT_FROST_HUT      = 6,
    AV_CPLACE_SPIRIT_MAIN_ALLIANCE  = 7,
    AV_CPLACE_SPIRIT_MAIN_HORDE     = 8,
//i don't will add for all 4 positions a variable.. i think one is enough to compute the rest
    AV_CPLACE_DEFENSE_STORM_AID      = 9,
    AV_CPLACE_DEFEMSE_STORM_GRAVE    = 13,
    AV_CPLACE_DEFENSE_STONE_GRAVE    = 17,
    AV_CPLACE_DEFENSE_SNOWFALL       = 21,
    AV_CPLACE_DEFENSE_FROSTWOLF      = 25,
    AV_CPLACE_DEFENSE_ICE_GRAVE      = 29,
    AV_CPLACE_DEFENSE_FROST_HUT      = 33,

    AV_CPLACE_DEFENSE_DUN_S          = 37,
    AV_CPLACE_DEFENSE_DUN_N          = 41,
    AV_CPLACE_DEFENSE_ICEWING        = 45,
    AV_CPLACE_DEFENSE_STONE_TOWER    = 49,
    AV_CPLACE_DEFENSE_ICE_TOWER      = 53,
    AV_CPLACE_DEFENSE_TOWERPOINT     = 57,
    AV_CPLACE_DEFENSE_FROST_E        = 61,
    AV_CPLACE_DEFENSE_FROST_t        = 65,

    AV_CPLACE_A_MARSHAL_SOUTH       = 69,
    AV_CPLACE_A_MARSHAL_NORTH       = 70,
    AV_CPLACE_A_MARSHAL_ICE         = 71,
    AV_CPLACE_A_MARSHAL_STONE       = 72,
    AV_CPLACE_H_MARSHAL_ICE         = 73,
    AV_CPLACE_H_MARSHAL_TOWER       = 74,
    AV_CPLACE_H_MARSHAL_ETOWER      = 75,
    AV_CPLACE_H_MARSHAL_WTOWER      = 76,
//following static means, no waypoints
    AV_CPLACE_IRONDEEP_S_1_MIN      = 77, //S=static and 1=type1 (trogg)
    AV_CPLACE_IRONDEEP_S_1_MAX      = 96, //if you are in a for-loop and want all type1-troggs make for(i=...irondeep_s_1;i<=..irondeep_max;i++)
//-150 placeholder
    AV_CPLACE_IRONDEEP_S_2_MIN      = 147, //thats the number of pokemon+mew  (2=shaman)
    AV_CPLACE_IRONDEEP_S_2_MAX      = 155,
//-175 placeholder
    AV_CPLACE_IRONDEEP_S_3_MIN      = 172,
    AV_CPLACE_IRONDEEP_S_3_MAX      = 173,
//-185 placeholder
    AV_CPLACE_IRONDEEP_M_1_1        = 182, //m=moving 1_1 = type1 creature 1
    AV_CPLACE_IRONDEEP_M_1_2        = 183,
    AV_CPLACE_IRONDEEP_M_1_3        = 184,
    AV_CPLACE_IRONDEEP_M_2          = 185,
    AV_CPLACE_IRONDEEP_M_3          = 186,

    AV_CPLACE_IRONDEEP_B_4          = 187,
    AV_CPLACE_IRONDEEP_B_2_1        = 188,
    AV_CPLACE_IRONDEEP_B_2_2        = 189,

    AV_CPLACE_MAX = 190
};

//x, y, z, o
const float BG_AV_CreaturePos[AV_CPLACE_MAX][4] = {
    //spiritguides
    {643.000000f,44.000000f,69.740196f,-0.001854f},
    {676.000000f,-374.000000f,30.000000f,-0.001854f},
    {73.417755f,-496.433105f,48.731918f,-0.001854f},
    {-157.409195f,31.206272f,77.050598f,-0.001854f},
    {-1090.476807f,-253.308670f,57.672371f,-0.001854f},
    {-531.217834f,-405.231384f,49.551376f,-0.001854f},
    {-1496.065063f,-333.338409f,101.134804f,-0.001854f},
    {873.001770f,-491.283630f,96.541931f,-0.001854f},
    {-1437.670044f,-610.088989f,51.161900f,-0.001854f},
//alliance
 //grave
 //firstaid - OK
    {635.17f,-29.5594f,46.5056f,4.81711f},
    {642.488f,-32.9437f,46.365f,4.67748f},
    {642.326f,-27.9442f,46.9211f,4.59022f},
    {635.945f,-33.6171f,45.7164f,4.97419f},
   //stormpike
    {669.272f,-297.304f,30.291f,4.66604f},
    {674.08f,-292.328f,30.4817f,0.0918785f},
    {667.01f,-288.532f,29.8809f,1.81583f},
    {664.153f,-294.042f,30.2851f,3.28531f},
  //stone
    {81.7027f,-406.135f,47.7843f,0.598464f},
    {78.1431f,-409.215f,48.0401f,5.05953f},
    {73.4135f,-407.035f,46.7527f,3.34736f},
    {78.2258f,-401.859f,46.4202f,2.05852f},
  //snowfall
    {-207.412f,-110.616f,78.7959f,2.43251f},
    {-197.95f,-112.205f,78.5686f,6.22441f},
    {-202.709f,-116.829f,78.4358f,5.13742f},
    {-202.059f,-108.314f,78.5783f,5.91968f},
  //ice
    {-615.501f,-393.802f,60.4299f,3.06147f},
    {-608.513f,-392.717f,62.5724f,2.06323f},
    {-609.769f,-400.072f,60.7174f,5.22367f},
    {-616.093f,-398.293f,60.5628f,3.73613f},
  //frost
    {-1077.7f,-340.21f,55.4682f,6.25569f},
    {-1082.74f,-333.821f,54.7962f,2.05459f},
    {-1090.66f,-341.267f,54.6768f,3.27746f},
    {-1081.58f,-344.63f,55.256f,4.75636f},
  //frost hut
    {-1408.95f,-311.69f,89.2536f,4.49954f},
    {-1407.15f,-305.323f,89.1993f,2.86827f},
    {-1400.64f,-304.3f,89.7008f,1.0595f},
    {-1400.4f,-311.35f,89.3028f,4.99434f},
  //towers
  //dun south - OK
    {569.395f,-101.064f,52.8296f,2.34974f},
    {574.85f,-92.9842f,52.5869f,3.09325f},
    {575.411f,-83.597f,52.3626f,6.26573f},
    {571.352f,-75.6582f,52.479f,0.523599f},
    //dun north - OK
    {668.60f,-122.53f,64.12f,2.34f}, //not 100% ok
    {662.253f,-129.105f,64.1794f,2.77507f},
    {661.209f,-138.877f,64.2251f,3.38594f},
    {665.481f,-146.857f,64.1271f,3.75246f},
    //icewing - OK
    {225.228f,-368.909f,56.9983f,6.23806f},
    {191.36f,-369.899f,57.1524f,3.24631f},
    {215.518f,-384.019f,56.9889f,5.09636f},
    {199.625f,-382.177f,56.8691f,4.08407f},
    //stone
    {-172.851f,-452.366f,40.8725f,3.31829f},
    {-147.147f,-435.053f,40.8022f,0.599238f},
    {-169.456f,-440.325f,40.985f,2.59101f},
    {-163.494f,-434.904f,41.0725f,1.84174f},
    //ice - OK
    {-573.522f,-271.854f,75.0078f,3.9619f},
    {-565.616f,-269.051f,74.9952f,5.02655f},
    {-562.825f,-261.087f,74.9898f,5.95157f},
    {-569.176f,-254.446f,74.8771f,0.820305f},
    //towerpoint
    {-763.04f,-371.032f,90.7933f,5.25979f},
    {-759.764f,-358.264f,90.8681f,0.289795f},
    {-768.808f,-353.056f,90.8811f,1.52601f},
    {-775.944f,-362.639f,90.8949f,2.59573f},
    //frost etower
    {-1294.13f,-313.045f,107.328f,0.270162f},
    {-1306.5f,-308.105f,113.767f,1.78755f},
    {-1294.78f,-319.966f,113.79f,5.94545f},
    {-1294.83f,-312.241f,113.799f,0.295293f},
    //frost wtower
    {-1300.96f,-275.111f,114.058f,4.12804f},
    {-1302.41f,-259.256f,114.065f,1.67602f},
    {-1287.97f,-262.087f,114.165f,6.18264f},
    {-1291.59f,-271.166f,114.151f,5.28257f},

    //alliance marshall
    {721.104f,-7.64155f,50.7046f,3.45575f},// south
    {723.058f,-14.1548f,50.7046f,3.40339f},// north
    {715.691f,-4.72233f,50.2187f,3.47321f},// icewing
    {720.046f,-19.9413f,50.2187f,3.36849f},// stone
//horde  (coords not 100% ok)
    {-1363.99f,-221.99f,98.4053f,4.93012f},
    {-1370.96f,-223.532f,98.4266f,4.93012f},
    {-1378.37f,-228.614f,99.3546f,5.38565f},
    {-1358.02f,-228.998f,98.868f,3.87768f},

 //irondeep mine
    //troggs:
    {801.405f,-328.055f,53.0195f,4.31096f},
    {805.399f,-320.146f,52.8546f,0.296706f},
    {831.711f,-346.785f,47.2975f,0.226893f},
    {834.354f,-355.526f,48.1491f,6.07375f},
    {859.03f,-367.231f,47.4655f,0.0174533f},
    {852.632f,-372.416f,48.1657f,3.66519f},
    {835.077f,-379.418f,48.2755f,5.93412f},
    {821.006f,-387.635f,49.0728f,3.15905f},
    {817.26f,-447.432f,49.4308f,2.18166f},
    {821.892f,-464.723f,48.9451f,4.66003f},
    {834.87f,-453.304f,47.9075f,0.226893f},
    {847.601f,-423.072f,50.0852f,4.57276f},
    {847.135f,-411.307f,50.2106f,1.5708f},
    {874.577f,-414.786f,52.7817f,1.67552f},
    {881.618f,-419.948f,53.6411f,0.593412f},
    {900.962f,-427.44f,59.0842f,1.50098f},
    {907.209f,-428.267f,59.8869f,1.8675f},
    {901.809f,-457.709f,59.0657f,3.52557f},
    {897.929f,-471.742f,59.7729f,2.54818f},
    {909.246f,-474.576f,58.2901f,0.226893f},
    {0,0,0,0}, //placeholder
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    //shaman
    {808.953f,-325.964f,52.5075f,3.01942f},//452
    {826.402f,-349.454f,47.2722f,1.51844f},//488
    {851.471f,-362.52f,47.314f,4.06662f},//456
    {817.83f,-455.715f,48.4207f,0.925025f},//149
    {842.08f,-421.775f,48.2659f,1.0821f},//480
    {877.989f,-418.051f,52.9753f,4.46804f},//492
    {898.801f,-437.105f,58.5266f,0.959931f},//489
    {902.195f,-475.891f,58.3954f,1.39626f},//487
    {909.99f,-462.154f,59.1644f,3.7001f},//142
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    //skullthumper
    {832.035f,-389.301f,47.5567f,2.11185f},//409
    {827.012f,-457.397f,48.9331f,2.35619f},//534
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    //moving group trogg,trogg,trogg,skull,sham
    {826.92f,-425.763f,48.2884f,1.49238f},//609
    {827.251f,-421.546f,48.3029f,1.49237f},//610
    {827.089f,-423.614f,48.2707f,1.49237f},//608
    {826.762f,-427.763f,48.3039f,1.49237f},//428
    {826.616f,-429.625f,48.2699f,1.49239f},//490
    //boss morloch,sham,sham
    {867.107f,-437.199f,50.7527f,1.69662f},//299
    {858.593f,-439.614f,50.2184f,0.872665f},//530
    {868.182f,-444.315f,51.5415f,1.64061f},//532
};


enum BG_AV_CreatureIds
{

	AV_NPC_A_GRAVEDEFENSE0 = 0,		// stormpike Defender
	AV_NPC_A_GRAVEDEFENSE1 = 1,		// seasoned defender
	AV_NPC_A_GRAVEDEFENSE2 = 2,		// veteran defender
    AV_NPC_A_GRAVEDEFENSE3 = 3,      // champion defender
    AV_NPC_A_TOWERDEFENSE  = 4,      // stormpike bowman
    AV_NPC_A_CAPTAIN       = 5,      // balinda
    AV_NPC_A_BOSS          = 6,      // vanndar

    AV_NPC_H_GRAVEDEFENSE0 = 7,      // frostwolf guardian
	AV_NPC_H_GRAVEDEFENSE1 = 8,      // seasoned guardian
	AV_NPC_H_GRAVEDEFENSE2 = 9,      // veteran guardian
    AV_NPC_H_GRAVEDEFENSE3 = 10,     // champion guardian
    AV_NPC_H_TOWERDEFENSE  = 11,     // frostwolf bowman
    AV_NPC_H_CAPTAIN       = 12,     // galvangar
    AV_NPC_H_BOSS          = 13,      // drek thar

    AV_NPC_A_MARSHAL_SOUTH       = 14,
    AV_NPC_MARSHAL_NORTH       = 15,
    AV_NPC_A_MARSHAL_ICE         = 16,
    AV_NPC_A_MARSHAL_STONE       = 17,
    AV_NPC_H_MARSHAL_ICE         = 18,
    AV_NPC_H_MARSHAL_TOWER       = 19,
    AV_NPC_MARSHAL_ETOWER      = 20,
    AV_NPC_H_MARSHAL_WTOWER      = 21,
    AV_NPC_IRONDEEP_N_1           = 22,
    AV_NPC_IRONDEEP_N_2           = 23,
    AV_NPC_IRONDEEP_N_3           = 24,
    AV_NPC_IRONDEEP_N_4           = 25,
    AV_NPC_IRONDEEP_A_1           = 26,
    AV_NPC_IRONDEEP_A_2           = 27,
    AV_NPC_IRONDEEP_A_3           = 28,
    AV_NPC_IRONDEEP_A_4           = 29,
    AV_NPC_IRONDEEP_H_1           = 30,
    AV_NPC_IRONDEEP_H_2           = 31,
    AV_NPC_IRONDEEP_H_3           = 32,
    AV_NPC_IRONDEEP_H_4           = 33,
    AV_NPC_INFO_MAX              = 34

};

//entry, team, minlevel, maxlevel
const uint32 BG_AV_CreatureInfo[AV_NPC_INFO_MAX][4] = {
    { 12050, 1216, 58, 58 }, //Stormpike Defender
    { 13326, 1216, 59, 59 }, //Seasoned Defender
    { 13331, 1216, 60, 60 }, //Veteran Defender
    { 13422, 1216, 61, 61 }, //Champion Defender
    { 13358, 1216, 59, 60 }, //Stormpike Bowman //i think its 60,61 and 69,70.. but this is until now not possible TODO look if this is ok
    { 11949,469,0,0},//not spawned with this data, but used for handlekillunit
    { 11948,469,0,0},//not spawned with this data, but used for handlekillunit
    { 12053, 1214, 58, 58 }, //Frostwolf Guardian
    { 13328, 1214, 59, 59 }, //Seasoned Guardian
    { 13332, 1214, 60, 60 }, //Veteran Guardian
    { 13421, 1214, 61, 61 }, //Champion Guardian
    { 13359, 1214, 59, 60 }, //Frostwolf Bowman
    { 11947,67,0,0}, //not spawned with this data, but used for handlekillunit
    { 11946,67,0,0}, //not spawned with this data, but used for handlekillunit
    { 14763, 1534, 60, 60 }, //Dun Baldar South Marshal
    { 14762, 1534, 60, 60 }, //Dun Baldar North Marshal
    { 14764, 1534, 60, 60 }, //Icewing Marshal
    { 14765, 1534, 60, 60 }, //Stonehearth Marshal

    { 14773, 1214, 70, 70 }, //Iceblood Warmaster
    { 14776, 1214, 70, 70 }, //Tower Point Warmaster
    { 14772, 1214, 70, 70 }, //East Frostwolf Warmaster
    { 14777, 1214, 70, 70 }, //West Frostwolf Warmaster

    { 10987, 59, 52, 53 }, //Irondeep Trogg
    { 11600, 59, 53, 54 }, //Irondeep Shaman
    { 11602, 59, 54, 55 }, //Irondeep Skullthumper
    { 11657, 59, 58, 58 }, //Morloch

    {13396,469,52,53}, //irondeep alliance TODO: get the right ids - currently i'm not sure if all of them (mine-npcs) are right
    {13098,469,53,54},
    {13080,469,54,55},
    {13078,469,58,58},

    {13397,67,52,53}, //irondeep horde
    {13099,67,53,54},
    {13081,67,54,55},
    {13079,67,58,58}
//south:ally-boss: 13086 hordeboss: 13088 neutralboss:11677

};

//x,y,z,o,static_creature_info-id
const float BG_AV_StaticCreaturePos[AV_STATICCPLACE_MAX][5] = { //static creatures
    {-1235.31f,-340.777f,60.5088f,3.31613f,0 },//2225 - Zora Guthrek
    {-1244.02f,-323.795f,61.0485f,5.21853f,1 },//3343 - Grelkor
    {-1235.16f,-332.302f,60.2985f,2.96706f,2 },//3625 - Rarck
    {587.303f,-42.8257f,37.5615f,5.23599f,3 },//4255 - Brogus Thunderbrew
    {643.635f,-58.3987f,41.7405f,4.72984f,4 },//4257 - Lana Thunderbrew
    {591.464f,-44.452f,37.6166f,5.65487f,5 },//5134 - Jonivera Farmountain
    {608.515f,-33.3935f,42.0003f,5.41052f,6 },//5135 - Svalbrad Farmountain
    {617.656f,-32.0701f,42.7168f,4.06662f,7 },//5139 - Kurdrum Barleybeard
    {-1183.76f,-268.295f,72.8233f,3.28122f,8 },//10364 - Yaelika Farclaw
    {-1187.86f,-275.31f,73.0481f,3.63028f,9 },//10367 - Shrye Ragefist
    {-1008.42f,-368.006f,55.3426f,5.95647f,10 },//10981 - Frostwolf
    {-1091.92f,-424.28f,53.0139f,2.93958f,10 },//10981 - Frostwolf
    {-558.455f,-198.768f,58.1755f,4.97946f,10 },//10981 - Frostwolf
    {-861.247f,-312.51f,55.1427f,3.35382f,10 },//10981 - Frostwolf
    {-1003.81f,-395.913f,50.4736f,2.85631f,10 },//10981 - Frostwolf
    {-904.5f,-289.815f,65.1222f,5.7847f,10 },//10981 - Frostwolf
    {-1064.41f,-438.839f,51.3614f,1.88857f,10 },//10981 - Frostwolf
    {258.814f,76.2017f,18.6468f,6.19052f,11 },//10986 - Snowblind Harpy
    {265.838f,-315.846f,-16.5429f,3.15917f,11 },//10986 - Snowblind Harpy
    {426.485f,-51.1927f,-5.66286f,1.60347f,11 },//10986 - Snowblind Harpy
    {452.044f,-33.9594f,-0.044651f,2.72815f,11 },//10986 - Snowblind Harpy
    {266.032f,-315.639f,-16.5429f,4.67962f,11 },//10986 - Snowblind Harpy
    {532.64f,-54.5863f,20.7024f,2.93215f,11 },//10986 - Snowblind Harpy
    {295.183f,-299.908f,-34.6123f,0.135851f,12 },//10990 - Alterac Ram
    {421.08f,-225.006f,-23.73f,0.166754f,12 },//10990 - Alterac Ram
    {-55.7766f,-192.498f,20.4352f,6.12221f,12 },//10990 - Alterac Ram
    {527.887f,-477.223f,62.3559f,0.170935f,12 },//10990 - Alterac Ram
    {389.144f,-346.508f,-30.334f,4.14117f,12 },//10990 - Alterac Ram
    {108.121f,-322.248f,37.5655f,4.46788f,12 },//10990 - Alterac Ram
    {507.479f,-67.9403f,10.3571f,3.26304f,12 },//10990 - Alterac Ram
    {329.071f,-185.016f,-29.1542f,0.356943f,12 },//10990 - Alterac Ram
    {252.449f,-422.313f,35.1404f,4.53771f,12 },//10990 - Alterac Ram
    {358.882f,-118.061f,-24.9119f,2.29257f,12 },//10990 - Alterac Ram
    {487.151f,-174.229f,14.7558f,4.73192f,12 },//10990 - Alterac Ram
    {449.652f,-123.561f,6.14273f,6.12029f,12 },//10990 - Alterac Ram
    {272.419f,-261.802f,-41.8835f,3.66559f,12 },//10990 - Alterac Ram
    {359.021f,-210.954f,-29.3483f,4.31339f,12 },//10990 - Alterac Ram
    {450.598f,-318.048f,-37.7548f,0.655219f,12 },//10990 - Alterac Ram
    {509.333f,-218.2f,3.05439f,3.66292f,12 },//10990 - Alterac Ram
    {485.771f,-223.613f,-1.53f,2.04862f,12 },//10990 - Alterac Ram
    {486.636f,-452.172f,39.6592f,2.3341f,12 },//10990 - Alterac Ram
    {702.783f,-257.494f,25.9777f,1.68329f,12 },//10990 - Alterac Ram
    {460.942f,-199.263f,-6.0149f,0.380506f,12 },//10990 - Alterac Ram
    {483.108f,-115.307f,10.1056f,3.69701f,12 },//10990 - Alterac Ram
    {471.601f,-154.174f,14.0702f,5.5807f,12 },//10990 - Alterac Ram
    {213.938f,-420.793f,41.2549f,5.71394f,12 },//10990 - Alterac Ram
    {289.387f,-294.685f,-33.9073f,0.555494f,12 },//10990 - Alterac Ram
    {155.649f,-402.891f,43.3915f,5.94838f,12 },//10990 - Alterac Ram
    {517.184f,-295.105f,-9.78195f,6.05668f,12 },//10990 - Alterac Ram
    {102.334f,-332.165f,38.9812f,3.31445f,12 },//10990 - Alterac Ram
    {320.244f,-107.793f,-42.6357f,-1.00311f,12 },//10990 - Alterac Ram
    {217.976f,110.774f,15.7603f,4.56793f,13 },//11675 - Snowblind Windcaller
    {269.872f,6.66684f,20.7592f,0.381212f,13 },//11675 - Snowblind Windcaller
    {313.528f,-319.041f,-27.2373f,0.554098f,13 },//11675 - Snowblind Windcaller
    {435.441f,-39.9289f,-0.169651f,0.549454f,13 },//11675 - Snowblind Windcaller
    {315.115f,-317.62f,-29.1123f,0.90111f,13 },//11675 - Snowblind Windcaller
    {428.091f,-122.731f,3.40332f,6.05901f,14 },//11678 - Snowblind Ambusher
    {235.05f,85.5705f,18.3079f,-0.914255f,14 },//11678 - Snowblind Ambusher
    {-1553.04f,-344.342f,64.4163f,6.09933f,15 },//11839 - Wildpaw Brute
    {-545.23f,-165.35f,57.7886f,3.01145f,16 },//11947 - Captain Galvangar
    {722.43f,-10.9982f,50.7046f,3.42085f,17 },//11948 - Vanndar Stormpike
    {-57.7891f,-286.597f,15.6479f,6.02139f,18 },//11949 - Captain Balinda Stonehearth
    {930.498f,-520.755f,93.7334f,1.8326f,19 },//11997 - Stormpike Herald
    {-776.092f,-345.161f,67.4092f,1.89257f,20 },//12051 - Frostwolf Legionnaire
    {-1224.63f,-308.144f,65.0087f,4.01139f,20 },//12051 - Frostwolf Legionnaire
    {-713.039f,-442.515f,82.8638f,0.68724f,20 },//12051 - Frostwolf Legionnaire
    {-711.783f,-444.061f,82.7039f,0.683494f,20 },//12051 - Frostwolf Legionnaire
    {587.633f,-45.9816f,37.5438f,5.81195f,21 },//12096 - Stormpike Quartermaster
    {-1293.79f,-194.407f,72.4398f,5.84685f,22 },//12097 - Frostwolf Quartermaster
    {446.163f,-377.119f,-1.12725f,0.209526f,23 },//12127 - Stormpike Guardsman
    {549.348f,-399.254f,53.3537f,3.24729f,23 },//12127 - Stormpike Guardsman
    {549.801f,-401.217f,53.8305f,3.24729f,23 },//12127 - Stormpike Guardsman
    {192.704f,-406.874f,42.9183f,6.10696f,23 },//12127 - Stormpike Guardsman
    {441.305f,-435.765f,28.2385f,2.14472f,23 },//12127 - Stormpike Guardsman
    {192.982f,-404.891f,43.0132f,6.1061f,23 },//12127 - Stormpike Guardsman
    {355.342f,-391.989f,-0.486707f,3.00643f,23 },//12127 - Stormpike Guardsman
    {446.035f,-375.104f,-1.12725f,0.21033f,23 },//12127 - Stormpike Guardsman
    {697.864f,-433.238f,62.7914f,1.65776f,23 },//12127 - Stormpike Guardsman
    {610.74f,-331.585f,30.8021f,5.14253f,23 },//12127 - Stormpike Guardsman
    {609.815f,-329.775f,30.9271f,-2.38829f,23 },//12127 - Stormpike Guardsman
    {695.874f,-433.434f,62.8543f,1.65776f,23 },//12127 - Stormpike Guardsman
    {443.337f,-435.283f,28.6842f,2.13768f,23 },//12127 - Stormpike Guardsman
    {-1251.5f,-316.327f,62.6565f,5.02655f,24 },//13176 - Smith Regzar
    {-1332.0f,-331.243f,91.2631f,1.50098f,25 },//13179 - Wing Commander Guse
    {569.983f,-94.9992f,38.0325f,1.39626f,26 },//13216 - Gaelden Hammersmith
    {-1244.92f,-308.916f,63.2525f,1.62316f,27 },//13218 - Grunnda Wolfheart
    {-1319.56f,-342.675f,60.3404f,1.20428f,28 },//13236 - Primalist Thurloga
    {647.61f,-61.1548f,41.7405f,4.24115f,29 },//13257 - Murgot Deepforge
    {-1321.64f,-343.73f,60.4833f,1.01229f,30 },//13284 - Frostwolf Shaman
    {-1317.61f,-342.853f,60.3726f,2.47837f,30 },//13284 - Frostwolf Shaman
    {-1319.31f,-344.475f,60.3825f,1.72788f,30 },//13284 - Frostwolf Shaman
    {569.963f,-42.0218f,37.7581f,4.27606f,31 },//13438 - Wing Commander Slidore
    {729.2f,-78.812f,51.6335f,3.97935f,32 },//13442 - Arch Druid Renferal
    {729.118f,-82.8713f,51.6335f,2.53073f,33 },//13443 - Druid of the Grove
    {725.554f,-79.4973f,51.6335f,5.27089f,33 },//13443 - Druid of the Grove
    {724.768f,-84.1642f,51.6335f,0.733038f,33 },//13443 - Druid of the Grove
    {596.68f,-83.0633f,39.0051f,6.24828f,34 },//13447 - Corporal Noreg Stormpike
    {600.032f,-2.92475f,42.0788f,5.00909f,35 },//13577 - Stormpike Ram Rider Commander
    {610.239f,-21.8454f,43.272f,4.90438f,36 },//13617 - Stormpike Stable Master
    {613.422f,-150.764f,33.4517f,5.55015f,37 },//13797 - Mountaineer Boombellow
    {-1213.91f,-370.619f,56.4455f,0.837758f,38 },//13798 - Jotek
    {704.35f,-22.9071f,50.2187f,0.785398f,39 },//13816 - Prospector Stonehewer
    {-1271.24f,-335.766f,62.3971f,5.75959f,40 },//14185 - Najak Hexxen
    {-1268.64f,-332.688f,62.6171f,5.28835f,41 },//14186 - Ravak Grimtotem
    {648.363f,-65.2233f,41.7405f,3.12414f,42 },//14187 - Athramanis
    {648.238f,-67.8931f,41.7405f,2.60054f,43 },//14188 - Dirk Swindle
    {-1223.44f,-309.833f,64.9331f,4.0131f,44 },//14282 - Frostwolf Bloodhound
    {-1226.4f,-307.136f,64.9706f,4.0145f,44 },//14282 - Frostwolf Bloodhound
    {356.001f,-389.969f,-0.438796f,3.0334f,45 },//14283 - Stormpike Owl
    {355.835f,-394.005f,-0.60149f,3.02498f,45 },//14283 - Stormpike Owl
    {882.266f,-496.378f,96.7707f,4.83248f,45 },//14283 - Stormpike Owl
    {878.649f,-495.917f,96.6171f,4.67693f,45 },//14283 - Stormpike Owl
    {932.851f,-511.017f,93.6748f,3.61004f,45 },//14283 - Stormpike Owl
    {935.806f,-513.983f,93.7436f,3.61788f,45 },//14283 - Stormpike Owl
    {947.412f,-509.982f,95.1098f,2.82743f,46 },//14284 - Stormpike Battleguard
    {934.557f,-512.395f,93.662f,3.61004f,46 },//14284 - Stormpike Battleguard
    {939.42f,-502.777f,94.5887f,5.14872f,46 },//14284 - Stormpike Battleguard
    {854.276f,-494.241f,96.8017f,5.44543f,46 },//14284 - Stormpike Battleguard
    {776.621f,-487.775f,99.4049f,3.50811f,46 },//14284 - Stormpike Battleguard
    {880.169f,-495.699f,96.6204f,4.8325f,46 },//14284 - Stormpike Battleguard
    {773.651f,-497.482f,99.0408f,2.11185f,46 },//14284 - Stormpike Battleguard
    {949.1f,-506.913f,95.4237f,3.31613f,46 },//14284 - Stormpike Battleguard
    {-1370.9f,-219.793f,98.4258f,5.04381f,47}, //drek thar

};

const uint32 BG_AV_StaticCreatureInfo[51][4] = {
    { 2225, 1215, 65, 65 }, //Zora Guthrek
    { 3343, 1215, 65, 65 }, //Grelkor
    { 3625, 1215, 65, 65 }, //Rarck
    { 4255, 1217, 65, 65 }, //Brogus Thunderbrew
    { 4257, 1217, 65, 65 }, //Lana Thunderbrew
    { 5134, 1217, 65, 65 }, //Jonivera Farmountain
    { 5135, 1217, 65, 65 }, //Svalbrad Farmountain
    { 5139, 1217, 65, 65 }, //Kurdrum Barleybeard
    { 10364, 1215, 65, 65 }, //Yaelika Farclaw
    { 10367, 1215, 65, 65 }, //Shrye Ragefist
    { 10981, 38, 50, 51 }, //Frostwolf
    { 10986, 514, 62, 63 }, //Snowblind Harpy
    { 10990, 1274, 60, 61 }, //Alterac Ram
    { 11675, 514, 63, 63 }, //Snowblind Windcaller
    { 11678, 14, 52, 53 }, //Snowblind Ambusher
    { 11839, 39, 66, 66 }, //Wildpaw Brute
    { 11947, 1214, 61, 61 }, //Captain Galvangar
    { 11948, 1216, 63, 63 }, //Vanndar Stormpike
    { 11949, 1216, 61, 61 }, //Captain Balinda Stonehearth
    { 11997, 1334, 60, 60 }, //Stormpike Herald
    { 12051, 1214, 67, 67 }, //Frostwolf Legionnaire
    { 12096, 1217, 65, 65 }, //Stormpike Quartermaster
    { 12097, 1215, 65, 65 }, //Frostwolf Quartermaster
    { 12127, 1216, 67, 67 }, //Stormpike Guardsman
    { 13176, 1215, 60, 60 }, //Smith Regzar
    { 13179, 1215, 69, 69 }, //Wing Commander Guse
    { 13216, 1217, 68, 68 }, //Gaelden Hammersmith
    { 13218, 1215, 68, 68 }, //Grunnda Wolfheart
    { 13236, 1214, 60, 60 }, //Primalist Thurloga
    { 13257, 1216, 60, 60 }, //Murgot Deepforge
    { 13284, 1214, 68, 68 }, //Frostwolf Shaman
    { 13438, 1217, 68, 68 }, //Wing Commander Slidore
    { 13442, 1216, 60, 60 }, //Arch Druid Renferal
    { 13443, 1216, 60, 60 }, //Druid of the Grove
    { 13447, 1216, 68, 68 }, //Corporal Noreg Stormpike
    { 13577, 1216, 60, 60 }, //Stormpike Ram Rider Commander
    { 13617, 1216, 60, 60 }, //Stormpike Stable Master
    { 13797, 32, 60, 61 }, //Mountaineer Boombellow
    { 13798, 1214, 60, 61 }, //Jotek
    { 13816, 1216, 61, 61 }, //Prospector Stonehewer
    { 14185, 877, 69, 69 }, //Najak Hexxen
    { 14186, 105, 60, 60 }, //Ravak Grimtotem
    { 14187, 1594, 60, 60 }, //Athramanis
    { 14188, 57, 69, 69 }, //Dirk Swindle
    { 14282, 1214, 53, 54 }, //Frostwolf Bloodhound
    { 14283, 1216, 53, 54 }, //Stormpike Owl
    { 14284, 1216, 61, 61 }, //Stormpike Battleguard
    { 11946, 1214, 63, 63 }, //Drek'Thar //TODO: make the levels right (boss=0 maybe)
    { 11948, 1216, 63, 63 }, //Vanndar Stormpike
    { 11947, 1214, 61, 61 }, //Captain Galvangar
    { 11949, 1216, 61, 61 } //Captain Balinda Stonehearth
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
    AV_GRAVE_MAIN_HORDE        = 610
};



const uint32 BG_AV_GraveyardIds[9]= {
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

enum BG_AV_BUFF
{ //TODO add all other buffs here
    AV_BUFF_ARMOR = 21163
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

/*
    //the comments behind the state shows which icon overlaps the other.. but is, until now, unused
    // Graves

    // Alliance
    //Stormpike first aid station
    AV_AID_A_C                      = 1325,
    AV_AID_A_A                      = 1326,
    AV_AID_H_C                      = 1327,
    AV_AID_H_A                      = 1328,
    //Stormpike Graveyard
    AV_PIKEGRAVE_A_C                = 1333,
    AV_PIKEGRAVE_A_A                = 1335,
    AV_PIKEGRAVE_H_C                = 1334,
    AV_PIKEGRAVE_H_A                = 1336,
    //Stoneheart Grave
    AV_STONEHEART_A_C               = 1302,
    AV_STONEHEART_A_A               = 1304, //over hc
    AV_STONEHEART_H_C               = 1301, //over ac
    AV_STONEHEART_H_A               = 1303, //over aa
    //Neutral
    //Snowfall Grave
*/
    AV_SNOWFALL_N                   = 1966, //over aa
/*
    AV_SNOWFALL_A_C                 = 1341, //over hc
    AV_SNOWFALL_A_A                 = 1343, //over ha
    AV_SNOWFALL_H_C                 = 1342,
    AV_SNOWFALL_H_A                 = 1344, //over ac
    //Horde
    //Iceblood grave
    AV_ICEBLOOD_A_C                 = 1346, //over hc
    AV_ICEBLOOD_A_A                 = 1348, //over ac
    AV_ICEBLOOD_H_C                 = 1347,
    AV_ICEBLOOD_H_A                 = 1349, //over aa
    //Frostwolf Grave
    AV_FROSTWOLF_A_C                = 1337, //over hc
    AV_FROSTWOLF_A_A                = 1339, //over ac
    AV_FROSTWOLF_H_C                = 1338,
    AV_FROSTWOLF_H_A                = 1340, //over aa
    //Frostwolf Hut
    AV_FROSTWOLFHUT_A_C             = 1329, //over hc
    AV_FROSTWOLFHUT_A_A             = 1331, //over ha
    AV_FROSTWOLFHUT_H_C             = 1330,
    AV_FROSTWOLFHUT_H_A             = 1332, //over ac

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
    AV_ICEBLOOD_CONTROLLED          = 1385,
    AV_ICEBLOOD_DESTROYED           = 1368,
    AV_ICEBLOOD_ASSAULTED           = 1390,
    //Tower Point
    AV_TOWERPOINT_CONTROLLED        = 1384,
    AV_TOWERPOINT_DESTROYED         = 1367, //goes over controlled
    AV_TOWERPOINT_ASSAULTED         = 1389, //goes over destroyed
    //Frostwolf West
    AV_FROSTWOLFW_CONTROLLED        = 1382,
    AV_FROSTWOLFW_DESTROYED         = 1365, //over controlled
    AV_FROSTWOLFW_ASSAULTED         = 1387, //over destroyed
    //Frostwolf East
    AV_FROSTWOLFE_CONTROLLED        = 1383,
    AV_FROSTWOLFE_DESTROYED         = 1366,
    AV_FROSTWOLFE_ASSAULTED         = 1388,
*/
    //mines
    AV_IRONDEEP_MINE_N              = 1360,
    AV_IRONDEEP_MINE_A              = 1358,
    AV_IRONDEEP_MINE_H              = 1359,

    AV_S_MINE_N                     = 1357,
    AV_S_MINE_A                     = 1355,
    AV_S_MINE_H                     = 1356,

/*
    //towers assaulted by own team (unused)
    AV_STONEH_UNUSED                = 1377,
    AV_ICEWING_UNUSED               = 1376,
    AV_DUNS_UNUSED                  = 1375,
    AV_DUNN_UNUSED                  = 1374,

    AV_ICEBLOOD_UNUSED              = 1395,
    AV_TOWERPOINT_UNUSED            = 1394,
    AV_FROSTWOLFE_UNUSED            = 1393,
    AV_FROSTWOLFW_UNUSED            = 1392
*/

};

//a_c a_a h_c h_a
const uint32 BG_AV_WorldStates[16][4] = {
    //Stormpike first aid station
    {1325, 1326,1327,1328},
    //Stormpike Graveyard
    {1333,1335,1334,1336},
    //Stoneheart Grave
    {1302,1304,1301,1303},
    //Snowfall Grave
    {1341,1343,1342,1344},
    //Iceblood grave
    {1346,1348,1347,1349},
    //Frostwolf Grave
    {1337,1339,1338,1340},
    //Frostwolf Hut
    {1329,1331,1330,1332},
    //Dunbaldar South Bunker
    {1361,1375,1370,1378},
    //Dunbaldar North Bunker
    {1362,1374,1371,1379},
    //Icewing Bunker
    {1363,1376,1372,1380},
    //Stoneheart Bunker
    {1364,1377,1373,1381},
    //Iceblood Tower
    {1368,1390,1385,1395},
    //Tower Point
    {1367,1389,1384,1394},
    //Frostwolf East
    {1366,1388,1383,1393},
    //Frostwolf West
    {1365,1387,1382,1392},
};



enum BG_AV_ItemIds
{
    AV_ITEM_BLOOD  = 17306,
    AV_ITEM_SCRAPS = 17422,
    AV_ITEM_CRYSTAL= 17423,
    AV_ITEM_A_SOLDIER = 17502,
    AV_ITEM_A_LIEUTNANT = 17503,
    AV_ITEM_A_COMMANDER = 17504,
    AV_ITEM_H_SOLDIER = 17326,
    AV_ITEM_H_LIEUTNANT = 17327,
    AV_ITEM_H_COMMANDER =17328,
    AV_ITEM_A_HIDE      = 17643,
    AV_ITEM_H_HIDE      = 17642,
    AV_ITEM_IRONDEEP = 17522,
    AV_ITEM_COLDTOOTH = 17542

};

enum BG_AV_QuestIds
{
    //TODO search and add the questids (and also search and add the first for example 6781 has 7223 first)
    AV_QUEST_A_SCRAPS1      = 7223,
    AV_QUEST_A_SCRAPS2      = 6781,
    AV_QUEST_H_SCRAPS1      = 7224,
    AV_QUEST_H_SCRAPS2      = 6741,
    AV_QUEST_A_COMMANDER1   = 6942, //soldier
    AV_QUEST_H_COMMANDER1   = 6825,
    AV_QUEST_A_COMMANDER2   = 6941, //leutnant
    AV_QUEST_H_COMMANDER2   = 6826,
    AV_QUEST_A_COMMANDER3   = 6943, //commander
    AV_QUEST_H_COMMANDER3   = 6827,
    AV_QUEST_A_BOSS1        = 7386, // 5 cristal/blood
    AV_QUEST_H_BOSS1        = 7385,
    AV_QUEST_A_BOSS2        = 6881, // 1
    AV_QUEST_H_BOSS2        = 6801,
    AV_QUEST_A_NEAR_MINE    = 5892, //the mine near start location of team
    AV_QUEST_H_NEAR_MINE    = 5893,
    AV_QUEST_A_OTHER_MINE   = 6982, //the other mine ;)
    AV_QUEST_H_OTHER_MINE   = 6985,
    AV_QUEST_A_RIDER_HIDE   = 7026,
    AV_QUEST_H_RIDER_HIDE   = 7002,
    AV_QUEST_A_RIDER_TAME   = 7027,
    AV_QUEST_H_RIDER_TAME   = 7001
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
        void UpdatePointsIcons(uint8 node);
        void UpdateScore(uint8 team, int16 points);
        void UpdateNode(uint32 type, uint32 state);
        void PopulateMine(uint8 mine);
        void PopulateNode(uint32 node);
        void DePopulateNode(uint32 node);
        const uint8 GetNodePlace(uint32 guid);
        const uint32 GetPlaceNode(uint8 node);
        const char* GetNodeName(uint8 node);
        const bool IsTower(uint8 node);
        const uint16 GetBonusHonor(uint8 kills);

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);
        void HandleKillPlayer(Player* player, Player *killer);
        void HandleKillUnit(Creature *unit, Player *killer);
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team);
        void UpdateQuest(uint32 questid, Player *player);

    private:
        void FillInitialWorldStates(WorldPacket& data);
        const uint8 GetWorldStateType(uint8 state, uint16 team);
        bool AddAVCreature(uint8 cinfoid, uint16 type);
        int32 m_Team_Scores[2];
        uint32 m_Team_QuestStatus[2][9]; //[x][y] x=team y=questcounter
        uint32 m_Points_Owner[BG_AV_NODES_MAX];
        uint32 m_Points_PrevOwner[BG_AV_NODES_MAX];
        uint32 m_Points_State[BG_AV_NODES_MAX];
        uint32 m_Points_PrevState[BG_AV_NODES_MAX];
        int32  m_Points_Timer[BG_AV_NODES_MAX];

        uint32 m_Mine_Owner[2];

        uint8 m_MaxLevel; //TODO remove this when battlegroundmgr provides a function for this..
        bool m_Snowfall_Capped;
        bool m_IsInformedNearVictory;

};

#endif
