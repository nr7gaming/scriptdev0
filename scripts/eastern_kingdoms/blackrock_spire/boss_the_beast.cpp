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
SDName: Boss_The_Best
SD%Complete: 100
SDComment:
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"
#include "blackrock_spire.h"


#define    SPELL_FLAMEBREAK      16785
#define    SPELL_IMMOLATE		 15570							//20294
#define    SPELL_TERRIFYINGROAR  14100
#define	   SPELL_BERSERKERCHARGE 16636
#define	   SPELL_FIREBLAST		 16144
#define	   SPELL_FIREBALL		 16788



struct MANGOS_DLL_DECL boss_thebeastAI : public ScriptedAI
{
    boss_thebeastAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiFlamebreakTimer;
    uint32 m_uiImmolateTimer;
    uint32 m_uiTerrifyingRoarTimer;
	uint32 m_uiBerserkerChargeTimer;
	uint32 m_uiFireBlastTimer;
	uint32 m_uiFireBallTimer;

    void Reset()
    {
        m_uiFlamebreakTimer     = 12000;
        m_uiImmolateTimer       = 3000;
        m_uiTerrifyingRoarTimer = 23000;
		m_uiBerserkerChargeTimer = 48000;
		m_uiFireBlastTimer		= 15000;
		m_uiFireBallTimer		= 25000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Flamebreak
        if (m_uiFlamebreakTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FLAMEBREAK);
            m_uiFlamebreakTimer = 10000;
        }
        else
            m_uiFlamebreakTimer -= uiDiff;

        // Immolate
        if (m_uiImmolateTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_IMMOLATE);

            m_uiImmolateTimer = 8000;
        }
        else
            m_uiImmolateTimer -= uiDiff;

        // Terrifying Roar
        if (m_uiTerrifyingRoarTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_TERRIFYINGROAR);
            m_uiTerrifyingRoarTimer = 20000;
        }
        else
            m_uiTerrifyingRoarTimer -= uiDiff;

		// BerserkerCharge
		if (m_uiBerserkerChargeTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_BERSERKERCHARGE);
			m_uiBerserkerChargeTimer = 45000;
		}
		else 
			m_uiBerserkerChargeTimer -= uiDiff;

		// FireBlast
		if (m_uiFireBlastTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_FIREBLAST);
				m_uiFireBlastTimer = 12000;
		}
		else m_uiFireBlastTimer -= uiDiff;

		// Fireball
		if (m_uiFireBallTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_FIREBALL);
				m_uiFireBallTimer = 23000;
		}
		else m_uiFireBallTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thebeast(Creature* pCreature)
{
    return new boss_thebeastAI(pCreature);
}

void AddSC_boss_thebeast()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_the_beast";
    newscript->GetAI = &GetAI_boss_thebeast;
    newscript->RegisterSelf();
}
