#include "Raven_TeamManager.h"



Raven_TeamManager::Raven_TeamManager(Vector2D weaponspawn)
	: m_pTarget(NULL), m_WeaponSpawn(weaponspawn), m_WeaponAvailable(false)
{
}


Raven_TeamManager::~Raven_TeamManager()
{
}


void Raven_TeamManager::AddTeammate(Raven_Teammate* pBot)
{
	if (pBot)
	{
		m_Teammates.push_back(pBot);
	}
}


void Raven_TeamManager::RemoveTeammate(Raven_Teammate* pBot)
{
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	while (curBot != m_Teammates.end())
	{
		if (*curBot == pBot)
		{
			m_Teammates.erase(curBot);
			return;
		}

		curBot++;
	}
}


bool Raven_TeamManager::isTeammate(Raven_Bot* pTarget)
{
	if (!pTarget)
		return false;

	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		if ((*curBot)->ID() == pTarget->ID())
			return true;
	}

	return false;
}


void Raven_TeamManager::UpdateTeam()
{
	if (!m_pTarget)
	{
		if (SearchNewTeamTarget())
		{
			std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
			for (curBot; curBot != m_Teammates.end(); ++curBot)
			{
				(*curBot)->GetTargetSys()->SetTarget(m_pTarget);
			}
		}
	}
}


bool Raven_TeamManager::SearchNewTeamTarget()
{
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	Raven_Bot* curTarget = NULL;
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		//we update each teammate's targeting system to find a new target
		(*curBot)->GetTargetSys()->Update();
		curTarget = (*curBot)->GetTargetBot();

		//if a new target that does not belong to the team is found
		if (!isTeammate(curTarget))
		{
			m_pTarget = (*curBot)->GetTargetBot();
			return true;
		}
	}

	return false;
}


void Raven_TeamManager::ClearTarget()
{
	m_pTarget = NULL;

	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		(*curBot)->GetTargetSys()->ClearTarget();
	}
}