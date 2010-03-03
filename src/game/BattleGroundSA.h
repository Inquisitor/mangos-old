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

#ifndef __BATTLEGROUNDSA_H
#define __BATTLEGROUNDSA_H

class BattleGround;

class BattleGroundSAScore : public BattleGroundScore
{
    public:
        BattleGroundSAScore() {};
        virtual ~BattleGroundSAScore() {};
    uint8 demolishers_destroyed;
    uint8 gates_destroyed;
};


enum BG_SA_Honor
{
    // Both teams get 248 honor, winning team gets an extra 124.
    BG_SA_HONOR_WIN  = 124,
    BG_SA_HONOR_END  = 248
};


enum BG_SA_Status
  {
    BG_SA_NOTSTARTED = 0,
    BG_SA_WARMUP,
    BG_SA_ROUND_ONE,
    BG_SA_SECOND_WARMUP,
    BG_SA_ROUND_TWO,
    BG_SA_BONUS_ROUND
  };

enum BG_SA_GateState
{
    BG_SA_GATE_OK               = 1,
    BG_SA_GATE_DAMAGED          = 2,
    BG_SA_GATE_DESTROYED        = 3
};

enum BG_SA_Timers
{
    BG_SA_BOAT_START            = 60000,
    BG_SA_WARMUPLENGTH          = 120000,
    BG_SA_ROUNDLENGTH           = 600000
};

enum BG_SA_WorldStates
{
    BG_SA_TIMER_MINS            = 3559,
    BG_SA_TIMER_SEC_TENS        = 3560,
    BG_SA_TIMER_SEC_DECS        = 3561,
    BG_SA_ALLY_ATTACKS          = 4352,
    BG_SA_HORDE_ATTACKS         = 4353,
    
    BG_SA_PURPLE_GATEWS         = 3614,
    BG_SA_RED_GATEWS            = 3617,
    BG_SA_BLUE_GATEWS           = 3620,
    BG_SA_GREEN_GATEWS          = 3623,
    BG_SA_YELLOW_GATEWS         = 3638,
    BG_SA_ANCIENT_GATEWS        = 3849,
    

    BG_SA_LEFT_GY_ALLIANCE      = 3635,
    BG_SA_RIGHT_GY_ALLIANCE     = 3636,
    BG_SA_CENTER_GY_ALLIANCE    = 3637,

    BG_SA_RIGHT_ATT_TOKEN_ALL   = 3627,
    BG_SA_LEFT_ATT_TOKEN_ALL    = 3626,

    BG_SA_LEFT_ATT_TOKEN_HRD    = 3629,
    BG_SA_RIGHT_ATT_TOKEN_HRD   = 3628,

    BG_SA_HORDE_DEFENCE_TOKEN   = 3631,
    BG_SA_ALLIANCE_DEFENCE_TOKEN = 3630,

    BG_SA_RIGHT_GY_HORDE        = 3632,
    BG_SA_LEFT_GY_HORDE         = 3633,
    BG_SA_CENTER_GY_HORDE       = 3634,

    BG_SA_BONUS_TIMER           = 0xdf3,
    BG_SA_ENABLE_TIMER          = 3564,
};

enum BG_SA_NPCs
{
    BG_SA_GUN_1                 = 0,
    BG_SA_GUN_2,
    BG_SA_GUN_3,
    BG_SA_GUN_4,
    BG_SA_GUN_5,
    BG_SA_GUN_6,
    BG_SA_GUN_7,
    BG_SA_GUN_8,
    BG_SA_GUN_9,
    BG_SA_GUN_10,
    BG_SA_DEMOLISHER_1,
    BG_SA_DEMOLISHER_2,
    BG_SA_DEMOLISHER_3,
    BG_SA_DEMOLISHER_4,
    BG_SA_NPC_SPARKLIGHT,
    BG_SA_NPC_RIGSPARK,
    BG_SA_NPC_MAX
};

const uint32 BG_SA_NpcEntries[BG_SA_NPC_MAX] = 
{
    27894,
    27894,
    27894,
    27894,
    27894,
    27894,
    27894,
    27894,
    27894,
    27894,
    //4 beach demolishers
    28781,
    28781,
    28781,
    28781,
    //Fizzle Sparklight, or whatever his name was
    29260,
    29262,
};

const float BG_SA_NpcSpawnlocs[BG_SA_NPC_MAX][4] = 
{
    //   X           Y            Z          O
    //Cannons
    { 1436.429f,   110.05f,    41.407f,    5.4f  }, 
    { 1404.9023f,  84.758f,    41.183f,    5.46f },
    { 1068.693f,   -86.951f,   93.81f,     0.02f }, 
    { 1068.83f,    -127.56f,   96.45f,     0.0912f },
    { 1422.115f,   -196.433f,  42.1825f,   1.0222f }, 
    { 1454.887f,   -220.454f,  41.956f,    0.9627f },
    { 1232.345f,   -187.517f,  66.945f,    0.45f  }, 
    { 1249.634f,   -224.189f,  66.72f,     0.635f },
    { 1236.213f,   92.287f,    64.965f,    5.751f }, 
    { 1215.11f,    57.772f,    64.739f,    5.78f  },
    //Demolishers
    { 1611.597656f, -117.270073f, 8.719355f, 2.513274f },
    { 1575.562500f, -158.421875f, 5.024450f, 2.129302f },
    { 1618.047729f, 61.424641f,   7.248210f, 3.979351f },
    { 1575.103149f, 98.873344f,   2.830360f, 3.752458f },
    //Npcs
    { 1348.644165f, -298.786469f, 31.080130f, 1.710423f },
    { 1358.191040f, 195.527786f,  31.018187f, 4.171337f },
};

enum BG_SA_Objects
{
    BG_SA_GREEN_GATE = 0,
    BG_SA_YELLOW_GATE,
    BG_SA_BLUE_GATE,
    BG_SA_RED_GATE,
    BG_SA_PURPLE_GATE,
    BG_SA_ANCIENT_GATE,
    BG_SA_TITAN_RELIC,
    BG_SA_BOAT_ONE,
    BG_SA_BOAT_TWO,
    BG_SA_SIGIL_1,
    BG_SA_SIGIL_2,
    BG_SA_SIGIL_3,
    BG_SA_SIGIL_4,
    BG_SA_SIGIL_5,
    BG_SA_CENTRAL_FLAGPOLE,
    BG_SA_RIGHT_FLAGPOLE,
    BG_SA_LEFT_FLAGPOLE,
    BG_SA_CENTRAL_FLAG,
    BG_SA_RIGHT_FLAG,
    BG_SA_LEFT_FLAG,
    BG_SA_MAXOBJ
};

const float BG_SA_ObjSpawnlocs[BG_SA_MAXOBJ][4] = 
{
    { 1411.57f,   108.163f,  28.692f,  5.441f  },
    { 1055.452f,  -108.1f,   82.134f,  0.034f  },
    { 1431.3413f, -219.437f, 30.893f,  0.9736f },
    { 1227.667f,  -212.555f, 55.372f,  0.5023f },
    { 1214.681f,  81.210f,   53.413f,  5.745f  },
    { 878.555f,   -108.989f, 119.835f, 0.0565f },
    { 836.5f,     -108.8f,   120.219f, 0.0f    },
    //Ships
    { 2679.696777f, -826.891235f, 3.712860f, 5.78367f  }, //rot2 1 rot3 0.0002
    { 2574.003662f, 981.261475f,  2.603424f, 0.807696f },
    //Sigils
    { 1414.054f, 106.72f,   41.442f, 5.441f  },
    { 1060.63f,  -107.8f,   94.7f,   0.034f  },
    { 1433.383f, -216.4f,   43.642f, 0.9736f },
    { 1230.75f,  -210.724f, 67.611f, 0.5023f },
    { 1217.8f,   79.532f,   66.58f,  5.745f  },
    //Flagpoles
    { 1215.114258f, -65.711861f,  70.084267f, -3.124123f },
    { 1338.863892f, -153.336533f, 30.895121f, -2.530723f },
    { 1309.124268f, 9.410645f,    30.893402f, -1.623156f },
    //Flags
    { 1215.108032f, -65.715767f,  70.084267f, -3.124123f },
    { 1338.859253f, -153.327316f, 30.895077f, -2.530723f },
    { 1309.192017f, 9.416233f,    30.893402f, 1.518436f  },
};

/* Ships:
 * 193182 - ally
 * 193183 - horde
 * 193184 - horde
 * 193185 - ally
 * Banners:
 * 191308 - left one,
 * 191306 - right one,
 * 191310 - central,
 * Ally ones, substract 1
 * to get horde ones.
 */

const uint32 BG_SA_ObjEntries[BG_SA_MAXOBJ] =
{
    190722,
    190727,
    190724,
    190726,
    190723,
    192549,
    192834,
    193182,
    193185,
    192687,
    192685,
    192689,
    192690,
    192691,
    191311,
    191311,
    191311,
    191310,
    191306,
    191308,
};

const uint32 BG_SA_Factions[2] =
{
    1732,
    1735,
};

enum BG_SA_Graveyards
{
    BG_SA_BEACH_GY = 0,
    BG_SA_DEFENDER_LAST_GY,
    BG_SA_RIGHT_CAPTURABLE_GY,
    BG_SA_LEFT_CAPTURABLE_GY,
    BG_SA_CENTRAL_CAPTURABLE_GY,
    BG_SA_GY_MAX
};

const uint32 BG_SA_GYEntries[BG_SA_GY_MAX] =
{
    1350,
    1349,
    1347,
    1346,
    1348,
};

const float BG_SA_GYOrientation[BG_SA_GY_MAX] =
{
    6.202f,
    1.926f, //Right capturable GY
    3.917f, //Left capturable GY
    3.104f, //Center, capturable
    6.148f  //Defender last GY
};

struct BG_SA_RoundScore
{
    BattleGroundTeamId winner;
    uint32 time;
};

class BattleGroundSA : public BattleGround
{
    friend class BattleGroundMgr;

    public:
        BattleGroundSA();
        ~BattleGroundSA();
        void Update(uint32 diff);

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player *plr);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();
        // Send Worldstates
        virtual void FillInitialWorldStates(WorldPacket& data);
        virtual void Reset();

        void RemovePlayer(Player *plr,uint64 guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        void HandleKillUnit(Creature *creature, Player *killer);

        virtual void EventPlayerClickedOnFlag(Player *Source, GameObject* target_obj);
        virtual void EventPlayerDamagedGO(Player* plr, GameObject* go, uint32 event);
        //virtual void EventPlayerUsedGO(Player* Source, GameObject* object);
        //bool SetupBattleGround();

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);

        void EndBattleGround(uint32 winner);

    private:
    
    void ToggleTimer();
    void SendTime();
    void DestroyGate(uint32 i, Player* pl);
    void CaptureGraveyard(BG_SA_Graveyards i);

    BattleGroundTeamId attackers;
    uint32 TotalTime;
    bool ShipsStarted;
    BG_SA_GateState GateStatus[6];
    BG_SA_Status status;
    BattleGroundTeamId GraveyardStatus[BG_SA_GY_MAX];
    BG_SA_RoundScore RoundScores[2]; 
    bool TimerEnabled;
};
#endif
