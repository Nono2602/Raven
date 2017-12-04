#include "Raven_Teammate.h"
#include "lua/Raven_Scriptor.h"
#include "Raven_SensoryMemory.h"
#include "time/Regulator.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "goals/Raven_Goal_Types.h"
#include "Goal_ThinkAsTeammate.h"

#include "Debug\DebugConsole.h"

#include "Raven_TeammateSteeringBehaviors.h"
#include "Raven_TeamManager.h"
#include "Raven_Leader.h"
#include "Game\EntityManager.h"

#include "Raven_TeammateSensoryMemory.h"

#include <limits>

Raven_Teammate::Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager)
	: Raven_Teammate(world, pos, teammanager, new Goal_ThinkAsTeammate(this))
{
}

Raven_Teammate::Raven_Teammate(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager, Goal_Think * goal)
	: Raven_Bot(world, pos, new Raven_TeammateSteering(world, this), goal, new Raven_TeammateSensoryMemory(this, script->GetDouble("Bot_MemorySpan"))),
	m_pTeamManager(teammanager), 
	m_pPartner(nullptr), 
	m_savedPosition(Vector2D())
{
	m_pPartnerUpdateRegulator = new Regulator(script->GetDouble("Teammate_PartnerUpdateFreq"));
}


Raven_Teammate::~Raven_Teammate()
{
}


void Raven_Teammate::Update()
{
	Raven_Bot::Update();
	if (isPossessed()) {
		m_pPartner = nullptr;
	}
	else {
		if (TeamSize() == 1) {
			m_pPartner = nullptr;
		}
		else if (m_pPartner && m_pPartner->isDead()) {
			m_pPartner = nullptr;
		}
		if (m_pPartnerUpdateRegulator->isReady()) {
			UpdatePartner();
		}
	}
}


bool Raven_Teammate::HandleMessage(const Telegram& msg)
{
	//first see if the current goal accepts the message
	if (GetBrain()->HandleMessage(msg)) return true;

	Raven_Bot * sender = nullptr;
	// Treat Raven_Teammate specific message
	switch (msg.Msg)
	{
	case Msg_TakeThatMF:
	{
#ifdef FRIENDLY_FIRE //Teammates can do damage to each other
		
		//just return if already dead or spawning
		if (isDead() || isSpawning()) return true;

		//the extra info field of the telegram carries the amount of damage
		ReduceHealth(DereferenceToType<int>(msg.ExtraInfo));

		//if this bot is now dead let the shooter know
		if (isDead())
		{
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				ID(),
				msg.Sender,
				Msg_YouGotMeYouSOB,
				NO_ADDITIONAL_INFO);
		}
#else
		sender = static_cast<Raven_Bot*>(EntityMgr->GetEntityFromID(msg.Sender));
		if (!m_pTeamManager->isTeammate(sender)) {

			//just return if already dead or spawning
			if (isDead() || isSpawning()) return true;

			//the extra info field of the telegram carries the amount of damage
			ReduceHealth(DereferenceToType<int>(msg.ExtraInfo));

			//if this bot is now dead let the shooter know
			if (isDead())
			{
				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
					ID(),
					msg.Sender,
					Msg_YouGotMeYouSOB,
					NO_ADDITIONAL_INFO);
			}
		}
#endif

		return true;
	}

	case Msg_YouGotMeYouSOB:
	{
		sender = static_cast<Raven_Bot*>(EntityMgr->GetEntityFromID(msg.Sender));
		if (!sender->HasTag(teammate_bot) || !m_pTeamManager->isTeammate(sender)) {
			IncrementScore();
			if (GetTargetBot() && GetTargetBot()->ID() == msg.Sender) {
				//the bot this bot has just killed should be removed as the target
				m_pTargSys->ClearTarget();
			}
		}
		return true;
	}

	case Msg_GunshotSound: 
	{
		sender = static_cast<Raven_Bot*>(msg.ExtraInfo);
		if (!m_pTeamManager->isTeammate(sender)) {
			//add the source of this sound to the bot's percepts
			GetSensoryMem()->UpdateWithSoundSource((Raven_Bot*)msg.ExtraInfo);
		}

		return true;
	}

	case Msg_UserHasRemovedBot:
	{

		Raven_Bot* pRemovedBot = (Raven_Bot*)msg.ExtraInfo;

		GetSensoryMem()->RemoveBotFromMemory(pRemovedBot);

		//if the removed bot is the target, make sure the target is cleared
		if (pRemovedBot == GetTargetSys()->GetTarget())
		{
			GetTargetSys()->ClearTarget();
		}
		if (pRemovedBot == m_pPartner) {
			m_pPartner = NULL;
		}
		return true;
	}
	case Msg_AskForLocation:
	{
		m_savedPosition = Pos();
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			ID(),
			msg.Sender,
			Msg_TeammateLocation,
			static_cast<void *>(&m_savedPosition)
		);
		return true;
	}

	case Msg_TeammateLocation:
	{
		if (msg.ExtraInfo)
		{
			Raven_Teammate * sender = static_cast<Raven_Teammate*>(EntityMgr->GetEntityFromID(msg.Sender));
			if (!m_pPartner || m_pPartner->isDead()) {
				m_pPartner = sender;
			}
			else {
				Vector2D senderPos = *static_cast<Vector2D *>(msg.ExtraInfo);
				double distToPartner = Pos().Distance(m_pPartner->Pos());
				if (distToPartner > Pos().Distance(senderPos)) {
					m_pPartner = sender;
				}
			}
		}
		return true;
	}
	default:
		return false;
	}
}

void Raven_Teammate::UpdatePartner()
{
	// ask to all teammates their location
	m_pTeamManager->SendMessageToAllTeammates(this, SEND_MSG_IMMEDIATELY, Msg_AskForLocation);
}

bool Raven_Teammate::HasPartner()
{
	return m_pPartner != nullptr && !m_pPartner->isDead();
}

double Raven_Teammate::DistanceToPartner()
{
	if (m_pPartner && !m_pPartner->isDead() && !isDead()) {
		return Pos().Distance(m_pPartner->Pos());
	}
	return DBL_MAX;
}

Goal_ThinkAsTeammate * const Raven_Teammate::GetBrain()
{
	return static_cast<Goal_ThinkAsTeammate *>(m_pBrain);
}

Raven_TeamManager * Raven_Teammate::GetTeam() const
{
	return m_pTeamManager;
}

bool Raven_Teammate::HasLeader() const
{
	return m_pTeamManager->HasLeader();
}

int Raven_Teammate::TeamSize()
{
	return m_pTeamManager->TeamSize();
}

bool Raven_Teammate::HasTag(int tag) const
{
	return Raven_Bot::HasTag(tag) || tag == teammate_bot;
}
