#include "Raven_TeamManager.h"

#include "Debug\DebugConsole.h"
#include "Raven_Leader.h"

#include <limits>
#include <set>
#include "goals/Raven_Feature.h"

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
		if(m_pTarget)
			debug_con << "ADDTEAMMATE : TEAM IS TARGETING " << m_pTarget->ID() << "";
		else
			debug_con << "ADDTEAMMATE : TEAM HAS NO TARGET" << "";

		//set the new teammate's target to the team target
		pBot->GetTargetSys()->SetTarget(m_pTarget);
		//add it to the team list
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

	if (m_pLeader != nullptr && m_pLeader->ID() == pTarget->ID())
		return true;

	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		if ((*curBot)->ID() == pTarget->ID())
			return true;
	}

	return false;
}


void Raven_TeamManager::Update()
{
	debug_con << "TEAM UPDATE" << "";
	if (!m_pTarget || m_pTarget->isDead()) {//if the team has has no target or if the last target died
		if (m_pLeader) { //if the team has a leader
			debug_con << "TEAM UPDATING WITH LEADER" << "";
			SearchNewTeamTargetByLeader();
		}
		else if(m_Teammates.size() >= 1){ //else
			debug_con << "TEAM UPDATING WITHOUT LEADER" << "";
			//update the targeting system of each teammate to find the new target
			SearchNewTeamTarget();
		}
	}

	UpdateTeammates();
}


void Raven_TeamManager::SearchNewTeamTarget()
{
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	Raven_Bot* curTarget = NULL;
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		//we update each teammate's targeting system to find a new target
		(*curBot)->GetTargetSys()->Update();
		curTarget = (*curBot)->GetTargetBot();

		//if a new target that does not belong to the team is found
		if (curTarget && !isTeammate(curTarget))
		{
			//it becomes the new team target
			m_pTarget = curTarget;
			return;
		}
	}
}

void Raven_TeamManager::SearchNewTeamTargetByLeader()
{
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	std::set<Raven_Bot*> potentialTarget;
	Raven_Bot* curTarget = NULL;
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		//we update each teammate's targeting system to find a new target
		(*curBot)->GetTargetSys()->Update();
		curTarget = (*curBot)->GetTargetBot();

		//if a new target that does not belong to the team is found
		if (curTarget && !isTeammate(curTarget))
		{
			//it is added to the potential target collection
			potentialTarget.insert(curTarget);
		}
	}
	// same with the leader
	m_pLeader->GetTargetSys()->Update();
	curTarget = m_pLeader->GetTargetBot();
	if (curTarget && !isTeammate(curTarget))
	{
		potentialTarget.insert(curTarget);
	}

	// search for most desirable target
	double desirability = 0.0f;
	curTarget = NULL;
	std::set<Raven_Bot*>::iterator it;
	for (it = potentialTarget.begin(); it != potentialTarget.end(); ++it) {
		double tmp = 1.0 - Raven_Feature::Health(*it);
		if (tmp > desirability) {
			desirability = tmp;
			curTarget = *it;
		}
	}
	if (curTarget) {
		m_pTarget = curTarget;
	}
}


void Raven_TeamManager::UpdateTeammates()
{
	if (!m_pTarget)
	{
		ClearTarget();
	}
	else
	{
		debug_con << "TEAM IS TARGETING " << m_pTarget->ID() << "";
		
		std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
		for (curBot; curBot != m_Teammates.end(); ++curBot)
		{
			(*curBot)->UpdateTeamTarget(m_pTarget);

			if((*curBot)->GetTargetBot())
				debug_con << "UPDATE TEAMMATE " << (*curBot)->ID() << " : targeting bot " << (*curBot)->GetTargetBot()->ID() << "";
			else
				debug_con << "UPDATE TEAMMATE " << (*curBot)->ID() << " : no target " << "";
		}
		if (m_pLeader) {
			m_pLeader->UpdateTeamTarget(m_pTarget);

			if (m_pLeader->GetTargetBot())
				debug_con << "UPDATE TEAMMATE " << m_pLeader->ID() << " : targeting bot " << m_pLeader->GetTargetBot()->ID() << "";
			else
				debug_con << "UPDATE TEAMMATE " << m_pLeader->ID() << " : no target " << "";
		}
	}
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

int Raven_TeamManager::TeamSize() const
{
	return m_Teammates.size() + (m_pLeader ? 1 : 0);
}

Vector2D Raven_TeamManager::CenterOfMass(Raven_Teammate * pBot) const
{
	int nb = 0;
	Vector2D pos;
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		if ((*curBot)->ID() != pBot->ID()) {
			pos += (*curBot)->Pos();
			nb++;
		}
	}
	if (m_pLeader) {
		pos += m_pLeader->Pos();
		nb++;
	}
	pos /= nb;

	return pos;
}

Raven_Teammate * Raven_TeamManager::ClosestToLocation(const Vector2D & location, Raven_Teammate* pBot) const
{
	double distance = DBL_MAX;
	Raven_Teammate * closest = nullptr;
	std::list<Raven_Teammate*>::const_iterator curBot = m_Teammates.begin();
	for (curBot; curBot != m_Teammates.end(); ++curBot)
	{
		if (pBot == nullptr || (*curBot)->ID() != pBot->ID()) {
			double tmp = location.Distance((*curBot)->Pos());
			if (tmp < distance) {
				distance = tmp;
				closest = (*curBot);
			}
		}
	}
	if (m_pLeader) {
		double tmp = location.Distance(m_pLeader->Pos());
		if (tmp < distance) {
			closest = m_pLeader;
		}
	}

	return closest;
}


void Raven_TeamManager::Clear()
{
	m_pTarget = NULL;
	m_Teammates.clear();
}