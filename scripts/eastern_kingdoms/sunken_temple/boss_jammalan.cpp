#include "precompiled.h"
#include "sunken_temple.h"

#define SPELL_EARTHRABTOTEM		8376
#define SPELL_FLAMESTRIKE		12468
#define SPELL_HEALINGWAVE		12492
#define SPELL_HEXOFJAMMALAN		12480

struct MANGOS_DLL_DECL boss_jammalanAI : public ScriptedAI
{
	boss_jammalanAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
	/*{
		m_pInstance = (instance_sunken_temple*) pCreature->GetInstanceData();
		Reset();
	} */
	uint32 m_uiEarthrabTotemTimer;
	uint32 m_uiFlameStrikeTimer;
	uint32 m_uiHealingWaveTimer;
	uint32 m_uiHexOfJammalanTimer;
	uint32 m_uiHealingWave2Timer;
	uint32 m_uiHealingWave3Timer;
	void Reset()
	{
		m_uiEarthrabTotemTimer		= 13000;
		m_uiFlameStrikeTimer		= 6000;
		m_uiHealingWaveTimer		= 20000;
		m_uiHexOfJammalanTimer		= 25000;
		m_uiHealingWave2Timer		= 20000;
		m_uiHealingWave3Timer		= 15000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		// Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		// Eearthrab Totem
		if (m_uiEarthrabTotemTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_EARTHRABTOTEM);
			m_uiEarthrabTotemTimer = 14000;
		}
		else 
			m_uiEarthrabTotemTimer -= uiDiff;

		// FlameStrike
		if (m_uiFlameStrikeTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_FLAMESTRIKE);
			m_uiFlameStrikeTimer = 9000;
		}
		else
			m_uiFlameStrikeTimer -= uiDiff;

		// Healing Wave wenn we have 50% life
		if (m_uiHealingWaveTimer < uiDiff && m_creature->GetHealthPercent() < 50.0f)
		{
			DoCastSpellIfCan(m_creature, SPELL_HEALINGWAVE);
			m_uiHealingWaveTimer = 15000;
		}
		else
			m_uiHealingWaveTimer -= uiDiff;

		
		// Healing Wave wenn we have 15% life
		if (m_uiHealingWave2Timer < uiDiff && m_creature->GetHealthPercent() < 15.0f)
		{
			DoCastSpellIfCan(m_creature, SPELL_HEALINGWAVE);
			m_uiHealingWave2Timer = 15000;
		}
		else
			m_uiHealingWave2Timer -= uiDiff;

		
		// Healing Wave
		if (m_uiHealingWave3Timer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_HEALINGWAVE);
			m_uiHealingWave3Timer = 15000;
		}
		else
			m_uiHealingWave3Timer -= uiDiff;

		/// HexOfJammalan
		if (m_uiHexOfJammalanTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			DoCastSpellIfCan(pTarget, SPELL_HEXOFJAMMALAN);
			m_uiHexOfJammalanTimer = 35000;
		}
		else
			m_uiHexOfJammalanTimer -= uiDiff;


		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_jammalan(Creature* pCreature)
{
	return new boss_jammalanAI(pCreature);
}

void AddSC_boss_jammalan()
{
	Script* newscript;
	newscript = new Script;
	newscript->Name = "boss_jammalan";
	newscript->GetAI = &GetAI_boss_jammalan;
	newscript->RegisterSelf();
}