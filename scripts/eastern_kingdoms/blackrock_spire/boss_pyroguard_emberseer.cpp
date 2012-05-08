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
SDName: Boss_Pyroguard_Emberseer
SD%Complete: 100
SDComment: Event to activate Emberseer NYI - 'aggro'-text missing
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"


#define   SPELL_FIRENOVA		  16079					  //23462, 16079 is the spell id from old.wowhead.com
#define   SPELL_FLAMEBUFFET		  16536						  //23341
#define   SPELL_PYROBLAST         20228                         // guesswork, but best fitting in spells-area, was 17274 (has mana cost)

// BLACKHAND_INCANCERATOR spells
//#define   SPELL_ENCAGE            16045
#define   SPELL_STRIKE            15580

//#define   NPC_EMBERSEER           9816
/*
#define   SPELL_EMBERSEER_GROW    16048 */

#define EMOTE_1 "Ich werde Staerker 1"
#define EMOTE_2 "Ich werde Staerker 2"
#define EMOTE_3 "Ich werde Staerker 3"




struct MANGOS_DLL_DECL boss_pyroguard_emberseerAI : public ScriptedAI
{
    boss_pyroguard_emberseerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_blackrock_spire*) pCreature->GetInstanceData();
        Reset();
    }

    instance_blackrock_spire* m_pInstance;
    uint32 m_uiFireNovaTimer;
    uint32 m_uiFlameBuffetTimer;
    uint32 m_uiPyroBlastTimer;

    uint32 uiSay1Timer;
    uint32 uiSay2Timer;
    uint32 uiSay3Timer;

    void Reset()
    {
        m_uiFireNovaTimer = 6000;
        m_uiFlameBuffetTimer = 3000;
        m_uiPyroBlastTimer = 14000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

        uiSay1Timer = 10000;
        uiSay2Timer = 20000;
        uiSay3Timer = 30000;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMBERSEER, IN_PROGRESS); 
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMBERSEER, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMBERSEER, FAIL);
    }

    void MoveInLineOfSight(Unit* target, const uint32 diff)
    {
        if (target && m_creature->GetDistance2d(target) <= 30)
        {
            if (!m_creature->HasAura(SPELL_ENCAGE))
            {
                if (uiSay1Timer <= diff)
                {
                    m_creature->MonsterTextEmote(EMOTE_1, m_creature, true);
                    uiSay2Timer = 10000;
                } else
                    uiSay1Timer -= diff;

                if (uiSay2Timer <= diff)
                {
                    m_creature->MonsterTextEmote(EMOTE_2, m_creature, true);
                    uiSay3Timer = 10000;
                } else
                    uiSay2Timer -= diff;

                if (uiSay3Timer <= diff)
                {
                    m_creature->MonsterTextEmote(EMOTE_3, m_creature, true);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // FireNova Timer
        if (m_uiFireNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FIRENOVA);
            m_uiFireNovaTimer = 6000;
        }
        else
            m_uiFireNovaTimer -= uiDiff;

        // FlameBuffet Timer
        if (m_uiFlameBuffetTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            DoCastSpellIfCan(pTarget, SPELL_FLAMEBUFFET);
            m_uiFlameBuffetTimer = 14000;
        }
        else
            m_uiFlameBuffetTimer -= uiDiff;

        // PyroBlast Timer
        if (m_uiPyroBlastTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_PYROBLAST);
            m_uiPyroBlastTimer = 15000;
        }
        else
            m_uiPyroBlastTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_pyroguard_emberseer(Creature* pCreature)
{
    return new boss_pyroguard_emberseerAI(pCreature);
}

struct MANGOS_DLL_DECL npc_blackhandAI : public ScriptedAI
{
    npc_blackhandAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (instance_blackrock_spire*) pCreature->GetInstanceData();
        Reset();
    }

    instance_blackrock_spire* pInstance;

    uint32 uiStrikeTimer;
    uint32 uiEncageTimer;
    uint32 uiFightTimer;

    bool AggroFight;
    bool x;
    bool IsInCombat;


    void Reset()
    {
        uiEncageTimer = 10000;
        uiStrikeTimer = 15000;
        uiFightTimer = 20000;  

        AggroFight = false;
        IsInCombat = false;
    }

    void Aggro(Unit* pWho, Creature* pCreature)
    {
        AggroFight = true;
    }

    void JustDied(Unit* pKiller)
    {
        if (Creature* Emberseer = GetClosestCreatureWithEntry(m_creature, NPC_EMBERSEER , 150.00f))
        {
            DoCastSpellIfCan(Emberseer, SPELL_EMBERSEER_GROW);
        }
    }

    void JustReachedHome()
    {
        Reset();
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_creature->isInCombat())
        {
            IsInCombat = true;
        }
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (!IsInCombat)
            {
                if (uiEncageTimer <= diff)
                {
                    if (Creature* Emberseer = GetClosestCreatureWithEntry(m_creature, NPC_EMBERSEER , 150.00f))
                    {
                        DoCastSpellIfCan(Emberseer, SPELL_ENCAGE);
                        uiEncageTimer = 10000;
                    }
                } else
                    uiEncageTimer -= diff;
            } 
        }

        if (AggroFight)
        {
            if (uiFightTimer <= diff)
            {
                //SetData(TYPE_EMBERSEER, IN_PROGRESS);
                    AggroFight = false;
            } else
                uiFightTimer -= diff;
        }

        if (uiStrikeTimer <= diff)
        {
            if (m_creature->isInCombat())
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_STRIKE);
                uiStrikeTimer = 20000;
            }
        } else
            uiStrikeTimer -= diff;

        DoMeleeAttackIfReady();
    }


};

CreatureAI* GetAI_npc_blackhand(Creature* pCreature)
{
    return new npc_blackhandAI(pCreature);
}



void AddSC_boss_pyroguard_emberseer()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "boss_pyroguard_emberseer";
    pNewScript->GetAI = &GetAI_boss_pyroguard_emberseer;
    pNewScript->RegisterSelf();

    Script* pNewScript2;
    pNewScript2 = new Script;
    pNewScript2->Name = "npc_blackhand";
    pNewScript2->GetAI = &GetAI_npc_blackhand;
    pNewScript2->RegisterSelf();
}

