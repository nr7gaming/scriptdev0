/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
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

/* ScriptData
SDName: Boss_Gyth
* ToDO:
* 29.04.12
* Over the arena, on the balkon, spawn some mobs when start the fight in the arena. 

*/

#include "precompiled.h"
#include "blackrock_spire.h"


#define    SPELL_CORROSIVEACID		16359						//20667, spell id is from old.wowhead.com
#define    SPELL_FREEZE             16350                       // ID was wrong!
#define    SPELL_FLAMEBREATH		16390						 //20712, spell id is from old.wowhead.com
#define    SPELL_ROOT_SELF          33356
#define	   SPELL_KNOCK_AWAY			10101						// this spell use this boss, information from old.wowhead.com
#define	   SPELL_SUMMONREND			16328						// summon Boss Rend blackhand

#define    MODEL_ID_INVISIBLE		11686
#define    MODEL_ID_GYTH_MOUNTED	9723
#define    MODEL_ID_GYTH			9806

#define    NPC_FIRE_TONGUE          10372
#define    NPC_CHROMATIC_WHELP      10442
#define    NPC_CHROMATIC_DRAGON     10447
#define    NPC_BLACKHAND_ELITE      10317
#define    NPC_BLACKHAND_HANDLER    10742
#define    NPC_REND_BLACKHAND       10429

// Rend and Nefarius for the event

#define NPC_REND_EVENT              170002
#define NPC_NEFARIUS_EVENT          170003
// Visitor from the arena fight
#define NPC_VISITOR                 9819

enum Yells
{
    SAY_NEFARIAN_INTRO_1        = -1533200,
    SAY_NEFARIAN_INTRO_2        = -1533201,
    SAY_REND_W_2                = -1533202,
    SAY_NEFARIAN_W_3            = -1533203,
    SAY_NEFARIAN_W_4            = -1533204,
    SAY_NEFARIAN_W_5            = -1533205,
    SAY_NEFARIAN_W_7            = -1533206,
    SAY_NEFARIAN_GO_REND_1      = -1533207,
    SAY_REND_END_1              = -1533208,
    SAY_NEFARIUS_END            = -1533209
};

//static Position Spawn = { 201.129f, -420.035f, 110.894f };
float fX1 = 200.049f;
float fY1 = -420.301f;
float fZ1 = 110.894f;
float fO = 3.070f;

float fX2 = 200.036f;
float fY2 = -417.669f;
float fZ2 = 110.894f;

float fX3 = 200.022f;
float fY3 = 415.051f;
float fZ3 = 110.890f;

float fX4 = 200.067f;
float fY4 = -423.786f;
float fZ4 = 110.891f;

float fX5 = 205.525f;
float fY5 = -425.451f;
float fZ5 = 110.913f;

float fX6 = 160.501f;
float fY6 = -420.693f;
float fZ6 = 110.472f;

// Teleport Point from nefarius after rend dead
float fNefariusX = 58.985f;
float fNefariusY = -522.634f;
float fNefariusZ = -6.955f;
float fNefariusO = 3.686f;

// Zuschauer positionen
float fOr = 4.680f;
float fZuschZ = 121.975f;
float fZuschauerX1 = 163.871f; float fZuschauerY1 = -396.051f;
float fZuschauerX2 = 163.890f; float fZuschauerY2 = -393.735f;
float fZuschauerX3 = 163.959f; float fZuschauerY3 = -391.558f;
float fZuschauerX4 = 160.370f; float fZuschauerY4 = -396.098f;
float fZuschauerX5 = 160.446f; float fZuschauerY5 = -393.710f;
float fZuschauerX6 = 160.510f; float fZuschauerY6 = -391.677f;
float fZuschauerX7 = 156.925f; float fZuschauerY7 = -396.068f;
float fZuschauerX8 = 157.023f; float fZuschauerY8 = -392.978f;
float fZuschauerX9 = 154.353f; float fZuschauerY9 = -395.867f;
float fZuschauerX10 = 154.453f; float fZuschauerY10 = -392.711f;
float fZuschauerX11 = 148.150f; float fZuschauerY11 = -395.717f;
float fZuschauerX12 = 148.249f; float fZuschauerY12 = -393.194f;
float fZuschauerX13 = 146.252f; float fZuschauerY13 = -396.326f;
float fZuschauerX14 = 146.328f; float fZuschauerY14 = -393.120f;
float fZuschauerX15 = 142.815f; float fZuschauerY15 = -395.987f;
float fZuschauerX16 = 142.875f; float fZuschauerY16 = -393.459f;
float fZuschauerX17 = 139.545f; float fZuschauerY17 = -395.696f;
float fZuschauerX18 = 139.613f; float fZuschauerY18 = -392.817f;


struct MANGOS_DLL_DECL boss_gythAI : public ScriptedAI
{
    boss_gythAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackrock_spire*) pCreature->GetInstanceData();
        Reset();
    }

    instance_blackrock_spire* m_pInstance;
    uint64 m_uiCombatDoorGUID;
    uint32 uiAggroTimer;
    uint32 uiDragonsTimer;
    uint32 uiOrcTimer;
    uint32 uiCorrosiveAcidTimer;
    uint32 uiFreezeTimer;
    uint32 uiFlamebreathTimer;
	uint32 uiKnockAwayTimer;
    uint32 uiLine1Count;
    uint32 uiLine2Count;

    uint32 uiIntroTimer;
    uint32 uiWaveTimer;
    uint32 uiRendSpawnTimer;
    uint32 uiVisitorSpawnTimer;

    bool m_bSummonedRend;
    bool m_bAggro;
    bool m_bRootSelf;

    bool Intro1;
    bool Intro2;
    bool Wave1;
    bool Wave2;
    bool Wave3;
    bool Wave4;
    bool Wave5;
    bool Wave6;
    bool Wave7;
    bool WaveEnd;

    bool IntroEnd;
    bool IntroEnd2;
    bool IntroEnd3;

    bool VisitorSpawn;

    void Reset()
    {
        uiDragonsTimer = 3000;
        uiOrcTimer = 60000;
        uiAggroTimer = 60000;
        uiCorrosiveAcidTimer = 8000;
        uiFreezeTimer = 11000;
        uiFlamebreathTimer = 4000;
		uiKnockAwayTimer = 20000;
        m_bSummonedRend = false;
        m_bAggro = false;
        m_bRootSelf = false;

        uiIntroTimer = 3000;
        uiWaveTimer = 60000;
        uiRendSpawnTimer = 30000;
        uiVisitorSpawnTimer = 30000;

        Intro1 = true;
        Intro2 = false;

        Wave1 = false;
        Wave2 = false;
        Wave3 = false;
        Wave4 = false;
        Wave5 = false;
        Wave6 = false;
        Wave7 = false;
        WaveEnd = false;

        IntroEnd = false;
        IntroEnd2 = false;
        IntroEnd3 = false;

        VisitorSpawn = true;

        // how many times should the two lines of summoned creatures be spawned
        // min 2 x 2, max 7 lines of attack in total
        uiLine1Count = urand(2, 5);
        uiLine2Count = urand(2, 7 - uiLine1Count);

        // Invisible for event start
        m_creature->SetDisplayId(MODEL_ID_INVISIBLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_GYTH, IN_PROGRESS);
            m_uiCombatDoorGUID = m_pInstance->GetData64(GO_GYTH_COMBAT_DOOR);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GYTH, DONE);
			DoCastSpellIfCan(m_creature, SPELL_SUMMONREND);
            Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
            DoScriptText(SAY_NEFARIUS_END, Nefarius);
            Nefarius->NearTeleportTo(fNefariusX, fNefariusY, fNefariusZ, fNefariusO, false);
            Nefarius->SetDisplayId(MODEL_ID_INVISIBLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GYTH, FAIL);
        
    }

    void UnSummon()
    {
        //m_creature->ForcedDespawn(NPC_VISITOR);
    }

 /*   void SummonCreatureWithRandomTarget(uint32 uiCreatureId)
    {
        float fX, fY, fZ;
        m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 2*INTERACTION_DISTANCE, fX, fY, fZ);
        fX = std::min(m_creature->GetPositionX(), fX);      // Halfcircle - suits better the rectangular form
        if (Creature* pSummoned = m_creature->SummonCreature(uiCreatureId, fX, fY, fZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000))
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pSummoned->AI()->AttackStart(pTarget);
    } */

    void JustSummoned(Creature* summon)
    {
        switch (summon->GetEntry())
        {
           /* Unit* Target;
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                pTarget = Target;
            } */
        case NPC_FIRE_TONGUE:
            summon->SetInCombatWithZone();
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summon->AI()->AttackStart(Target);
            break;
        case NPC_CHROMATIC_WHELP:
            summon->SetInCombatWithZone();
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summon->AI()->AttackStart(Target);
            break;
        case NPC_CHROMATIC_DRAGON:
            summon->SetInCombatWithZone();
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summon->AI()->AttackStart(Target);
            break;
        case NPC_BLACKHAND_ELITE:
            summon->SetInCombatWithZone();
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summon->AI()->AttackStart(Target);
            break;
        case NPC_BLACKHAND_HANDLER:
            summon->SetInCombatWithZone();
            if (Unit* Target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                summon->AI()->AttackStart(Target);
            break;
        default:
            break;
        }
    }


    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bRootSelf)
        {
            DoCastSpellIfCan(m_creature, SPELL_ROOT_SELF);
            m_bRootSelf = true;
        }

        if (!m_bAggro && IntroEnd3 /*uiLine1Count == 0 && uiLine2Count == 0*/)
        {
            if (uiAggroTimer < uiDiff)
            {
                m_bAggro = true;
                // Visible now!
                // teleport me behind the combat door. so i don't spawn in group.
                m_creature->NearTeleportTo(fX1, fY1, fZ1, fO, false);
                m_creature->SetDisplayId(MODEL_ID_GYTH_MOUNTED);
                m_creature->setFaction(14);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveAurasDueToSpell(SPELL_ROOT_SELF);
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);

            }
            else
                uiAggroTimer -= uiDiff;
        }

        if (Intro1)
        {
            Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
            DoScriptText(SAY_NEFARIAN_INTRO_1, Nefarius);
            Intro1 = false;
            Intro2 = true;
            uiIntroTimer = 10000;
        }

        if (Intro2)
        {
            if (uiIntroTimer <= uiDiff)
            {
                Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
                DoScriptText(SAY_NEFARIAN_INTRO_2, Nefarius);
                Intro2 = false;
                Wave1 = true;
                uiWaveTimer = 5000;
            } else uiIntroTimer -= uiDiff;
        }

        if (VisitorSpawn)
        {
            if (uiVisitorSpawnTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX1, fZuschauerY1, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX2, fZuschauerY2, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX3, fZuschauerY3, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX4, fZuschauerY4, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX5, fZuschauerY5, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX6, fZuschauerY6, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX7, fZuschauerY7, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX8, fZuschauerY8, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX9, fZuschauerY9, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX10, fZuschauerY10, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX11, fZuschauerY11, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX12, fZuschauerY12, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX13, fZuschauerY13, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX14, fZuschauerY14, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX15, fZuschauerY15, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX16, fZuschauerY16, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX17, fZuschauerY17, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_VISITOR, fZuschauerX18, fZuschauerY18, fZuschZ, fOr, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120*IN_MILLISECONDS);
                uiVisitorSpawnTimer = 30000;
                VisitorSpawn = false;
            } else
                uiVisitorSpawnTimer -= uiDiff;
        }

        if (uiVisitorSpawnTimer <= uiDiff)
        {
            VisitorSpawn = true;
            uiVisitorSpawnTimer = 90*IN_MILLISECONDS;
        } else
        uiVisitorSpawnTimer -= uiDiff;

        // Summon First Wave: 3 whelps, 1 dragon
        if (Wave1)
        {
            if (uiWaveTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave1 = false;
                Wave2 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave 2, 2 whelps, 1 dragon
        if (Wave2)
        {
            if (uiWaveTimer <= uiDiff)
            {
                Creature* Rend = GetClosestCreatureWithEntry(m_creature, NPC_REND_EVENT, 50.00f);
                DoScriptText(SAY_REND_W_2, 0, Rend);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave2 = false;
                Wave3 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave 3, 1 elite handler, 1 dragon, 2 whelps
        if (Wave3)
        {
            if (uiWaveTimer <= uiDiff)
            {
                Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
                DoScriptText(SAY_NEFARIAN_W_3, Nefarius);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_HANDLER, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave3 = false;
                Wave4 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave4, 1 elite handler, 1 dragon, 2 whelps
        if (Wave4)
        {
            if (uiWaveTimer <= uiDiff)
            {
                Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
                DoScriptText(SAY_NEFARIAN_W_4, Nefarius);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_HANDLER, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave4 = false;
                Wave5 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave5, 1 elite handler, 1 dragon, 3 whelps
        if (Wave5)
        {
            if (uiWaveTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX2, fY2, fZ2, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_HANDLER, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave5 = false;
                Wave6 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave6, 1 elite handler, 2 dragon, 3 whelps
        if (Wave6)
        {
            if (uiWaveTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX2, fY2, fZ2, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_HANDLER, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave6 = false;
                Wave7 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        // Summon Wave7, 1 elite handler, 2 dragons, 2 whelps
        if (Wave7)
        {
            if (uiWaveTimer <= uiDiff)
            {
                Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
                DoScriptText(SAY_NEFARIAN_W_7, Nefarius);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX2, fY2, fZ2, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_HANDLER, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                Wave7 = false;
                IntroEnd2 = true;
                uiWaveTimer = 60000;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
            } else
                uiWaveTimer -= uiDiff;
        }

        if (IntroEnd2)
        {
            Creature* Nefarius = GetClosestCreatureWithEntry(m_creature, NPC_NEFARIUS_EVENT, 50.00f);
            DoScriptText(SAY_NEFARIAN_GO_REND_1, Nefarius);
            IntroEnd2 = false;
            IntroEnd3 = true;
        }

        if (IntroEnd3)
        {            
            Creature* Rend = GetClosestCreatureWithEntry(m_creature, NPC_REND_EVENT, 50.00f);
            DoScriptText(SAY_REND_END_1, Rend);
            Rend->SetDisplayId(MODEL_ID_INVISIBLE);
        }



        // Summon Wave3, 2 whelps, 1 dragon, 1 dr


        // Summon Dragon pack. 2 Dragons and 3 Whelps
/*        if (!m_bAggro && !m_bSummonedRend && uiLine1Count > 0)
        {
            if (uiDragonsTimer < uiDiff)
            {
                m_creature->SummonCreature(NPC_FIRE_TONGUE, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_FIRE_TONGUE, fX2, fY2, fZ2, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                --uiLine1Count;
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
                uiDragonsTimer = 60000;
            }
            else
                uiDragonsTimer -= uiDiff;
        }

        //Summon Orc pack. 1 Orc Handler 1 Elite Dragonkin and 3 Whelps
        if (!m_bAggro && !m_bSummonedRend && uiLine1Count == 0 && uiLine2Count > 0)
        {
            if (uiOrcTimer < uiDiff)
            {
                m_creature->SummonCreature(NPC_CHROMATIC_DRAGON, fX1, fY1, fZ1, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_BLACKHAND_ELITE, fX2, fY2, fZ2, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX3, fY3, fZ3, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX4, fY4, fZ4, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                m_creature->SummonCreature(NPC_CHROMATIC_WHELP, fX5, fY5, fZ5, fO, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120*IN_MILLISECONDS);
                if (m_pInstance)
                    m_pInstance->DoUseDoorOrButton(m_uiCombatDoorGUID);
                --uiLine2Count;
                uiOrcTimer = 60000;
            }
            else
                uiOrcTimer -= uiDiff;
        } */

        // we take part in the fight
        if (m_bAggro)
        {
             // CorrosiveAcid_Timer
            if (uiCorrosiveAcidTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_CORROSIVEACID);
                uiCorrosiveAcidTimer = 7000;
            }
            else
                uiCorrosiveAcidTimer -= uiDiff;

            // Freeze_Timer
            if (uiFreezeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FREEZE) == CAST_OK)
                    uiFreezeTimer = 16000;
            }
            else
                uiFreezeTimer -= uiDiff;

            // Flamebreath_Timer
            if (uiFlamebreathTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_FLAMEBREATH);
                uiFlamebreathTimer = 10500;
            }
            else
                uiFlamebreathTimer -= uiDiff;
			
			// KNockAway_Timer
			if (uiKnockAwayTimer < uiDiff)
			{
				DoCastSpellIfCan(m_creature, SPELL_KNOCK_AWAY);
				uiKnockAwayTimer = 25000;
			}
			else
				uiKnockAwayTimer -= uiDiff;


            //Summon Rend
            if (!m_bSummonedRend && m_creature->GetHealthPercent() < 11.0f)
            {
                // summon Rend and Change model to normal Gyth
                // Inturrupt any spell casting
                m_creature->InterruptNonMeleeSpells(false);
                // Gyth model
                m_creature->SetDisplayId(MODEL_ID_GYTH);
                m_creature->SummonCreature(NPC_REND_BLACKHAND, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 900000);
                m_bSummonedRend = true;
            }

            DoMeleeAttackIfReady();
        }                                                   // end if Aggro
    }
};
// test
CreatureAI* GetAI_boss_gyth(Creature* pCreature)
{
    return new boss_gythAI(pCreature);
}

void AddSC_boss_gyth()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "boss_gyth";
    pNewScript->GetAI = &GetAI_boss_gyth;
    pNewScript->RegisterSelf();
}
