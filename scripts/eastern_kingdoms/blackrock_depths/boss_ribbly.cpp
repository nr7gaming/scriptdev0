

#include "precompiled.h"
#include "blackrock_depths.h"


enum 
{
    FACTION_ATTACK          = 21,

    GOSSIP_TEXT_RIBBLY      = 120000,
    GOSSIP_TEXT_RIBBLY_2    = 120001,

    SPELL_GOUGE             = 12540,
    SPELL_HAMSTRING         = 9080,

    NPC_RIBBLYS_CRONY       = 10043
};

float fHelperX1   = 880.843f;
float fHelperY1   = -167.852f;
float fHelperZ1   = -49.673f;
float fHelperO1   = 2.652f;

float fHelperX2   = 877.041f;
float fHelperY2   = -166.488f;
float fHelperZ2   = -49.674f;
float fHelperO2   = 5.777f;

float fHelperX3   = 875.614f;
float fHelperY3   = -168.167f;
float fHelperZ3   = -49.674f;
float fHelperO3   = 0.418f;

bool Attack;
#define GOSSIP_ITEM_RIBBLY "Was willst du von mir?"
#define GOSSIP_ITEM_RIBBLY_2 "Beleidigen!"
#define GOSSIP_ITEM_RIBBLY_3 "Ich werd es euch zeigen!"
//#define GOSSIP_TEXT_RIBBLY "Lasst mich doch einfach in Ruhe"
//#define GOSSIP_TEXT_RIBBLY_2 "Ihr Beleidigt mich hier! Jungs auf sie!"
/*
struct MANGOS_DLL_DECL boss_ribblyAI : public ScriptedAI
{
    boss_ribblyAI(Creature* pCreature) : ScriptedAI(pCreature)
    { 
        Reset();
    }

    uint32 uiGouge;
    uint32 uiHamstring;

    void Reset()
    {
        uiGouge = 15000;
        uiHamstring = 30000;
    }

    void UpdateAI(const uint32 diff)
    {

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiGouge <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(pTarget, SPELL_GOUGE);
                uiGouge = 15000;
            }
        } else
            uiGouge -= diff;

        if (uiHamstring <= diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_HAMSTRING);
            uiHamstring = 30000;
        } else
            uiHamstring -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ribbly(Creature* pCreature)
{
	return new boss_ribblyAI(pCreature);
} */




bool GossipHello_boss_ribbly(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RIBBLY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_RIBBLY, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_boss_ribbly(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RIBBLY_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_RIBBLY_2, pCreature->GetObjectGuid());
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RIBBLY_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pCreature->setFaction(FACTION_ATTACK);
        pCreature->AI()->AttackStart(pPlayer);
       /* pHelper->setFaction(FACTION_ATTACK);
        pHelper->AI()->AttackStart(pPlayer); */
        pCreature->SummonCreature(NPC_RIBBLYS_CRONY, fHelperX1, fHelperY1, fHelperZ1, fHelperO1, TEMPSUMMON_TIMED_DESPAWN, 720*MINUTE*IN_MILLISECONDS);
        pCreature->SummonCreature(NPC_RIBBLYS_CRONY, fHelperX2, fHelperY2, fHelperZ2, fHelperO2, TEMPSUMMON_TIMED_DESPAWN, 720*MINUTE*IN_MILLISECONDS);
        pCreature->SummonCreature(NPC_RIBBLYS_CRONY, fHelperX3, fHelperY3, fHelperZ3, fHelperO3, TEMPSUMMON_TIMED_DESPAWN, 720*MINUTE*IN_MILLISECONDS);

         
        Attack = true;
        break;
    }
    return true;
}

void AddSC_boss_ribbly()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_ribbly";
    //pNewScript->GetAI = &GetAI_boss_ribbly; 
    pNewScript->pGossipHello = &GossipHello_boss_ribbly;
    pNewScript->pGossipSelect = &GossipSelect_boss_ribbly;
    pNewScript->RegisterSelf();
}