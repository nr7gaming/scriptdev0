/*
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2010-2011 ScriptDev0 <http://github.com/mangos-zero/scriptdev0>
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

/* ScriptData
SDName: Boss_Quartmaster_Zigris
SD%Complete: 100
SDComment: Needs revision
SDCategory: Blackrock Spire
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_SHOOT          = 16496,
    SPELL_STUNBOMB       = 16497,
    SPELL_HEALING_POTION = 15504,
    SPELL_HOOKEDNET      = 15609
};

struct MANGOS_DLL_DECL boss_quatermasterzigrisAI : public ScriptedAI
{
    boss_quatermasterzigrisAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiShootTimer;
    uint32 m_uiStunBombTimer;
    //uint32 HelingPotion_Timer;

    void Reset()
    {
        m_uiShootTimer    = 1000;
        m_uiStunBombTimer = 16000;
        //HelingPotion_Timer = 25000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Shoot
        if (m_uiShootTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOOT);
            m_uiShootTimer = 500;
        }
        else
            m_uiShootTimer -= uiDiff;

        // StunBomb
        if (m_uiStunBombTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_STUNBOMB);
            m_uiStunBombTimer = 14000;
        }
        else
            m_uiStunBombTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_quatermasterzigris(Creature* pCreature)
{
    return new boss_quatermasterzigrisAI(pCreature);
}

void AddSC_boss_quatermasterzigris()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "quartermaster_zigris";
    newscript->GetAI = &GetAI_boss_quatermasterzigris;
    newscript->RegisterSelf();
}
