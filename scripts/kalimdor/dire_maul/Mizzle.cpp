/* Script for Mizzle the Crafty
 * Cast Spell: King of gordok
 */

/* Script Data
 * Name: Mizzle_The_Crafty
 */

#include "precompiled.h"
#include "dire_maul.h"

enum
{
	SPELL_KING_GORDOK	= 22799,
};

struct MANGOS_DLL_DECL MizzleAI : public ScriptedAI
{
	MizzleAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 uiKingOfGordokTimer;

	void Reset()
	{
		uiKingOfGordokTimer = 1000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		// Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		// Cast King of Gordok 
		if (uiKingOfGordokTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_KING_GORDOK);
		uiKingOfGordokTimer = 500;
		}
		else
			uiKingOfGordokTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_Mizzle(Creature* pCreature)
{
	return new MizzleAI(pCreature);
}

void AddSC_Mizzle()
{
	Script* newscript;
	newscript = new Script;
	newscript->Name = "Mizzle";
	newscript->GetAI = &GetAI_Mizzle;
	newscript->RegisterSelf();
}