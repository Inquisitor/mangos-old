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

#define BG_AV_REP_BOSS                  375 // i think you get this, if you win the bg, and not only at killing the boss
#define BG_AV_HONOR_BOSS                80
#define BG_AV_REP_CAPTAIN               125
#define BG_AV_HONOR_CAPTAIN             42
#define BG_AV_RES_CAPTAIN               100
#define BG_AV_REP_TOWER                 12
#define BG_AV_HONOR_TOWER               42
#define BG_AV_RES_TOWER                 75

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

    BG_AV_OBJECT_MAX                          = 68
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
const float BG_AV_NodePositions[15][4] = {
    {641.154f,-31.4824f,46.3348f,5.163358f}, //firstaid station
    {668.013f,-292.967f,30.2909f,2.76067f},  //stormpike
    {78.3916f,-405.245f,47.1657f,1.64148f},  //stone grave
    {-202.776f,-112.4f,78.4893f,2.01061f},   //snowfall
    {-611.962f,-396.17f,60.8351f,2.53682f},  //ice grave
    {-1083.37f,-340.922f,55.2714f,4.99119f}, //frostwolf grave
    {-1403.93f,-308.839f,89.3659f,5.24645f}, //frostwolf hut
    {552.585f,-77.9304f,51.9263f,5.10115f},  // dunbaldar south
    {673.021f,-144.669f,63.6473f,0.907122f}, //dunbaldar north
    {202.515f,-359.258f,56.3768f,5.34463f},  //icewing bunker
    {-151.864f,-440.838f,40.3897f,4.1744f},  //stone bunker
    {-572.723f,-262.062f,75.0087f,5.44671f}, //ice tower
    {-768.907f,-363.71f,90.8949f,1.07991f},  //tower point
    {-1297.5f,-266.767f,114.15f,3.31044f},   //frostwolf etower
    {-1302.89f,-316.92f,113.867f,2.01453f}   //frostwolf wtower
};

const float BG_AV_DoorPositons[2][4] = {
    {780.487f, -493.024f, 99.9553f, 3.0976f},   //alliance
    {-1375.193f, -538.981f, 55.2824f, 0.72178f} //horde
};


//creaturestuff starts here

//x, y, z, o
const float BG_AV_CreaturePos[75][4] = {
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
 //firstaid
    {645.532f,-30.2852f,46.8587f,0.226968f},
    {639.823f,-25.7137f,47.0798f,1.45219f},
    {635.435f,-32.0167f,45.9817f,2.95151f},
    {641.079f,-36.9292f,45.6846f,4.66997f},
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
  //dun south
    {566.87f,-73.6021f,52.5224f,1.06106f},
    {574.349f,-81.493f,52.3379f,0.0502529f},
    {574.858f,-90.2643f,52.3968f,5.61637f},
    {568.527f,-99.569f,52.6258f,5.56924f},
    //dun north
    {668.627f,-121.855f,64.0853f,2.51718f},
    {663.357f,-130.285f,64.1588f,2.979f},
    {661.803f,-137.394f,64.2315f,3.19341f},
    {665.178f,-144.785f,64.1426f,3.70392f},
    //icewing
    {192.439f,-364.37f,57.0477f,3.03868f},
    {194.121f,-374.727f,56.9618f,3.37484f},
    {202.65f,-382.997f,56.88f,4.18851f},
    {215.096f,-382.895f,56.8842f,4.88594f},
    //stone
    {-172.851f,-452.366f,40.8725f,3.31829f},
    {-147.147f,-435.053f,40.8022f,0.599238f},
    {-169.456f,-440.325f,40.985f,2.59101f},
    {-163.494f,-434.904f,41.0725f,1.84174f},
    //ice
    {-563.516f,-262.086f,74.9223f,6.14886f},
    {-580.222f,-267.368f,74.9545f,4.34716f},
    {-573.164f,-271.241f,74.9268f,4.65032f},
    {-565.353f,-257.227f,74.9879f,0.43038f},
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
    //drek thar
    {-1370.9f,-219.793f,98.4258f,5.04381f},
    //vanndar stormpike
    {728.3f,-8.99467f,50.6213f,3.40862f},
    //garni
    {-538.972f,-168.116f,57.0112f,5.83707f},
    //balinda
    {-54.2655f,-288.492f,15.5646f,5.98551f},
    //73 till now
    //both smiths
    {642.863f,-57.4503f,41.6571f,4.26077f},
    {-1253.88f,-317.969f,62.6003f,1.20972f}


};
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
    AV_CPLACE_H_BOSS                 = 69,
    AV_CPLACE_A_BOSS                 = 70,
    AV_CPLACE_H_CAPTAIN              = 71,
    AV_CPLACE_A_CAPTAIN              = 72,

    AV_CPLACE_A_SMITH                = 73,
    AV_CPLACE_H_SMITH                = 74,
    AV_CPLACE_MAX = 75
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

    AV_NPC_A_SMITH         = 14,     // murgot
    AV_NPC_H_SMITH         = 15     // regzar
};

//entry, team, minlevel, maxlevel
const uint32 BG_AV_CreatureInfo[16][4] = {
	{ 12050,469,58,58 },
	{ 13326,469,59,59},
	{ 13331,469,60,60},
    { 13422,469,61,61},
    { 13358,469,60,61},
    { 11949,469,0,0},
    { 11948,469,0,0},

    { 12053,67,58,58},
	{ 13328,67,59,59},
	{ 13332,67,60,60},
    { 13421,67,61,61},
    { 13359,67,60,61},
    { 11947,67,0,0},
    { 11946,67,0,0},
    { 13257,469,60,60},
    { 13176,67,60,60}
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

enum BG_AV_QuestIds
{
    //TODO search and add the questids (and also search and add the first for example 6781 has 7223 first)
    AV_QUEST_A_SCRAPS1      = 7223,
    AV_QUEST_A_SCRAPS2      = 6781,
    AV_QUEST_H_SCRAPS1       = 7224,
    AV_QUEST_H_SCRAPS2       = 6741,
    AV_QUEST_A_COMMANDER1   = 3, //soldier
    AV_QUEST_H_COMMANDER1   = 4,
    AV_QUEST_A_COMMANDER2   = 5, //leutnant
    AV_QUEST_H_COMMANDER2   = 6,
    AV_QUEST_A_COMMANDER3   = 7, //commander
    AV_QUEST_H_COMMANDER3   = 8,
    AV_QUEST_A_BOSS1        = 9, // 10 cristal/blood
    AV_QUEST_H_BOSS1        = 10,
    AV_QUEST_A_BOSS2        = 11, // 1
    AV_QUEST_H_BOSS2        = 12,
    AV_QUEST_A_NEAR_MINE    = 123, //the mine near start location of team
    AV_QUEST_H_NEAR_MINE    = 456,
    AV_QUEST_A_OTHER_MINE   = 987, //the other mine ;)
    AV_QUEST_H_OTHER_MINE   = 654,
    AV_QUEST_A_RIDER_HIDE   = 345,
    AV_QUEST_H_RIDER_HIDE   = 543,
    AV_QUEST_A_RIDER_TAME   = 101010,
    AV_QUEST_H_RIDER_TAME   = 42
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
        void UpdateScore(uint8 team, int16 points);
        void InitWorldStates();
        void UpdateNode(uint32 type, uint32 state);
        void PopulateNode(uint32 node);
        void DePopulateNode(uint32 node);
        int32 GetNode(uint64 guid);
        uint32 GetNodePlace(uint32 guid);
        uint32 GetPlaceNode(uint32 node);
        const char* GetNodeName(uint32 node);
        bool IsTower(uint32 node);

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);
        void HandleKillPlayer(Player* player, Player *killer);
        void HandleKillUnit(Creature *unit, Player *killer);
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team);
        void UpdateQuest(uint32 questid, Player *player);

    private:
        Creature* AddAVCreature(uint8 cinfoid, uint16 type);
        int32 m_Team_Scores[2];
        uint32 m_Team_QuestStatus[2][9]; //[x][y] x=team y=questcounter
        uint32 m_Points_Owner[BG_AV_NODES_MAX];
        uint32 m_Points_PrevOwner[BG_AV_NODES_MAX];
        uint32 m_Points_State[BG_AV_NODES_MAX];
        int32  m_Points_Timer[BG_AV_NODES_MAX];
        uint8 m_MaxLevel; //TODO remove this when battlegroundmgr provides a function for this..
        bool m_Snowfall_Capped;
        bool m_IsInformedNearVictory;

};
#endif
