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

        void RemovePlayer(Player *plr,uint64 guid);
        void HandleAreaTrigger(Player *Source, uint32 Trigger);
        //bool SetupBattleGround();

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value);

    private:
};
#endif
