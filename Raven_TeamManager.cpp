#include "Raven_TeamManager.h"

#include "Debug\DebugConsole.h"
#include "Raven_Leader.h"

#include <limits>
#include <set>
#include "goals/Raven_Feature.h"

#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

Raven_TeamManager::Raven_TeamManager(Vector2D weaponspawn)
	: m_pTarget(NULL), m_WeaponSpawn(weaponspawn), m_WeaponAvailable(false), m_pLeader(nullptr), m_pWeakTeammate(nullptr), goal(teamgoal_attack)
{
}


Raven_TeamManager::~Raven_TeamManager()
{
}


void Raven_TeamManager::AddTeammate(Raven_Teammate* pBot)
{
	if (pBot)
	{
		//if(m_pTarget)
		//	debug_con << "ADDTEAMMATE : TEAM IS TARGETING " << m_pTarget->ID() << "";
		//else
		//	debug_con << "ADDTEAMMATE : TEAM HAS NO TARGET" << "";

		////set the new teammate's target to the team target
		//pBot->GetTargetSys()->SetTarget(m_pTarget);
		////add it to the team list
		m_Teammates.push_back(pBot);
	}
}


Raven_Bot* Raven_TeamManager::RemoveATeammate()
{
	Raven_Bot* pTeammate = m_Teammates.back();

	m_Teammates.pop_back();

	return pTeammate;
}

bool Raven_TeamManager::isTeammate(Raven_Bot* pTarget)
{
	if (!pTarget)
		return false;
	if (pTarget->HasTag(teammate_bot)) 
	{
		Raven_Teammate* target = static_cast<Raven_Teammate*>(pTarget);
		return this == target->GetTeam();
	}

	return false;
}


void Raven_TeamManager::Update()
{
}


int Raven_TeamManager::TeamSize() const
{
	return m_Teammates.size() + (m_pLeader ? 1 : 0);
}

void Raven_TeamManager::SendMessageToAllTeammates(Raven_Teammate* sender, double delay, int msg, void* AdditionalInfo)
{
	int senderID = sender->ID();
	// send message to all teammates
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		if ((*curBot)->ID() != sender->ID() && !(*curBot)->isDead()) {
			Dispatcher->DispatchMsg(delay,
				senderID,
				(*curBot)->ID(),
				msg,
				AdditionalInfo);
		}
	}
}

void Raven_TeamManager::SendMessageToLeader(Raven_Teammate * sender, double delay, int msg, void * AdditionalInfo)
{
	if (m_pLeader && !m_pLeader->isDead()) {
		Dispatcher->DispatchMsg(delay,
			sender->ID(),
			m_pLeader->ID(),
			msg,
			AdditionalInfo);
	}
}


void Raven_TeamManager::Clear()
{
	m_pTarget = NULL;
	m_Teammates.clear();
}