/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Drakkisath
SD%Complete: 100
SDComment:
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"


//#define    SPELL_FIRENOVA        23462
#define    SPELL_CLEAVE			 15284							 //20691
#define    SPELL_CONFLIGURATION  16805
//#define    SPELL_THUNDERCLAP     15548                            //Not sure if right ID. 23931 would be a harder possibility.
#define		SPELL_FLAMESTRIKE	16419
#define		SPELL_PIERCEARMOR	6016
#define		SPELL_RAGE			16789


struct MANGOS_DLL_DECL boss_drakkisathAI : public ScriptedAI
{
    boss_drakkisathAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    //uint32 m_uiFireNovaTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiConfligurationTimer;
   // uint32 m_uiThunderclapTimer;
	uint32 m_uiFlameStrikeTimer;
	uint32 m_uiPierceArmorTimer;
	uint32 m_uiRageTimer;
    void Reset()
    {
       // m_uiFireNovaTimer       = 6000;
        m_uiCleaveTimer         = 8000;
        m_uiConfligurationTimer = 15000;
        //m_uiThunderclapTimer    = 17000;
		m_uiFlameStrikeTimer	= 25000;
		m_uiPierceArmorTimer	= 17000;
		m_uiRageTimer			= 45000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		/*
        // FireNova
        if (m_uiFireNovaTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FIRENOVA);
            m_uiFireNovaTimer = 10000;
        }
        else
            m_uiFireNovaTimer -= uiDiff; */

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 8000;
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Confliguration
        if (m_uiConfligurationTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONFLIGURATION, 0, m_creature->getVictim()->GetGUID());
            m_uiConfligurationTimer = 18000;
        }
        else
            m_uiConfligurationTimer -= uiDiff;
/*
        // Thunderclap
        if (m_uiThunderclapTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_THUNDERCLAP);
            m_uiThunderclapTimer = 20000;
        }
        else
            m_uiThunderclapTimer -= uiDiff; */

		// Flame Strike
		if (m_uiFlameStrikeTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_FLAMESTRIKE);
			m_uiFlameStrikeTimer = 25500;
		}
		else
			m_uiFlameStrikeTimer -= uiDiff;

		// Pierce Armor
		if (m_uiPierceArmorTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_PIERCEARMOR);
			m_uiPierceArmorTimer = 18000;
		}
		else
			m_uiPierceArmorTimer -= uiDiff;

		// Rage
		if (m_uiRageTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_RAGE);
				m_uiRageTimer = 48000;
		}
		else
			m_uiRageTimer -= uiDiff;



        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_drakkisath(Creature* pCreature)
{
    return new boss_drakkisathAI(pCreature);
}

void AddSC_boss_drakkisath()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_drakkisath";
    newscript->GetAI = &GetAI_boss_drakkisath;
    newscript->RegisterSelf();
}
