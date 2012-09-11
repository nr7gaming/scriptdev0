/* Script for the boss "King Gordok" in Dire Maul
 * Event for the spell "King of the gordok" need to script
 */

/* Script Data
 * Name: Boss_King_Gordok
 */

#include "precompiled.h"
#include "dire_maul.h"



enum
{
	SPELL_BERSERKER_CHARGE			= 22886,
	SPELL_MORTAL_STRIKE				= 15708,
	SPELL_SUNDER_ARMOR				= 15572,
	SPELL_WAR_STOMP					= 16727
	
	//NPC_MIZZLE_THE_CRAFTY			= 14353
};

struct MANGOS_DLL_DECL boss_king_gordokAI : public ScriptedAI
{
	boss_king_gordokAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}
	uint32 uiBerserkerChargeTimer;
	uint32 uiMortalStrikeTimer;
	uint32 uiSunderArmorTimer;
	uint32 uiWarStompTimer;

	bool m_bSummonMizzle;

	void Reset()
	{
		uiBerserkerChargeTimer = 15000;
		uiMortalStrikeTimer = 10000;
		uiSunderArmorTimer = 5000;
		uiWarStompTimer = 7500;
		m_bSummonMizzle = false;
	}

	void SummonCreature(uint32 uiCreatureId)
	{
		float fX, fY, fZ;
		m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 2*INTERACTION_DISTANCE, fX, fY, fZ);
		fX = std::min(m_creature->GetPositionX(), fX);
		if (Creature* pSummoned = m_creature->SummonCreature(uiCreatureId, fX, fY, fZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 240000))
		;
				
	}

	void UpdateAI(const uint32 uiDiff)
	{
		// Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;


	    // Berserker_charge_Timer
	    if (uiBerserkerChargeTimer < uiDiff)
	    {
		    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		    DoCastSpellIfCan(pTarget, SPELL_BERSERKER_CHARGE);
		    uiBerserkerChargeTimer = 20000;
	    }
	    else
		    uiBerserkerChargeTimer -= uiDiff;

	    // MortalStrikeTimer
	    if (uiMortalStrikeTimer < uiDiff)
	    {
		    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		    DoCastSpellIfCan(pTarget, SPELL_MORTAL_STRIKE);
		    uiMortalStrikeTimer = 16500;
	    }
	    else
		    uiMortalStrikeTimer -= uiDiff;

	    // SunderArmorTimer
	    if (uiSunderArmorTimer < uiDiff)
	    {
		    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		    DoCastSpellIfCan(pTarget, SPELL_SUNDER_ARMOR);
		    uiSunderArmorTimer = 6250;
	    }
	    else
		    uiSunderArmorTimer -= uiDiff;

	    // WarStompTimer
	    if (uiWarStompTimer < uiDiff)
	    {
		    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
		    DoCastSpellIfCan(pTarget, SPELL_WAR_STOMP);
		    uiWarStompTimer = 9000;
	    }
	    else
		    uiWarStompTimer -= uiDiff;

	    DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_king_gordok(Creature* pCreature)
{
	return new boss_king_gordokAI(pCreature);
}

void AddSC_boss_king_gordok()
{
	Script* newscript;
	newscript = new Script;
	newscript->Name = "boss_king_gordok";
	newscript->GetAI = &GetAI_boss_king_gordok;
	newscript->RegisterSelf();
}

