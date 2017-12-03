#include "Raven_Teammate.h"
#include "misc/Cgdi.h"
#include "misc/utils.h"
#include "2D/Transformations.h"
#include "2D/Geometry.h"
#include "lua/Raven_Scriptor.h"
#include "Raven_Game.h"
#include "navigation/Raven_PathPlanner.h"
#include "Raven_SteeringBehaviors.h"
#include "Raven_UserOptions.h"
#include "time/Regulator.h"
#include "Raven_WeaponSystem.h"
#include "Raven_SensoryMemory.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "goals/Raven_Goal_Types.h"
#include "Goal_ThinkAsTeammate.h"

#include "Debug\DebugConsole.h"

#include "Raven_TeammateSteeringBehaviors.h"
#include "Raven_TeamManager.h"
#include "Raven_Leader.h"

Raven_Teammate::Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager)
	: Raven_Bot(world, pos, new Raven_TeammateSteering(world, this), new Goal_ThinkAsTeammate(this)), m_pTeamManager(teammanager)
{
}

Raven_Teammate::Raven_Teammate(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager, Goal_Think * goal)
	: Raven_Bot(world, pos, new Raven_TeammateSteering(world, this), goal), m_pTeamManager(teammanager)
{
}


Raven_Teammate::~Raven_Teammate()
{
}


void Raven_Teammate::Update()
{
	Raven_Bot::Update();
	if (!isPossessed() && TeamSize() > 1) {
		UpdateRegroupLocation();
	}
}


bool Raven_Teammate::HandleMessage(const Telegram& msg)
{
	bool ret = Raven_Bot::HandleMessage(msg);
	if (ret) 
	{
		return true;
	}
	// Treat Raven_Teammate specific message

	return false;
}


Raven_Bot* Raven_Teammate::SearchNewTarget()
{
	if (m_pTargetSelectionRegulator->isReady())
	{
		m_pTargSys->Update();
	}

	return m_pTargSys->GetTarget();
}


void Raven_Teammate::UpdateTeamTarget(Raven_Bot* newtarget)
{
	if (GetSensoryMem()->isInMemory(newtarget))
	{
		m_pTargSys->SetTarget(newtarget);
	}
	else
	{
		m_pTargSys->ClearTarget();
	}
}

Goal_ThinkAsTeammate * const Raven_Teammate::GetBrain()
{
	return static_cast<Goal_ThinkAsTeammate *>(m_pBrain);
}

void Raven_Teammate::UpdateRegroupLocation()
{
	Vector2D center;
	if (m_pTeamManager->GetLeader() == nullptr) {
		// Get team center of mass (TeamSize() > 1)
		center = m_pTeamManager->CenterOfMass(this);

		// Get the closest teammate
		Raven_Teammate* closest = m_pTeamManager->ClosestToLocation(center, this);

		// update location
		m_regroupLocation = closest->Pos();
	}
	else {
		// Set regroup position to team leader position
		m_regroupLocation = m_pTeamManager->GetLeader()->Pos();
	}
}

Vector2D Raven_Teammate::GetRegroupLocation()
{
	return m_regroupLocation;
}

int Raven_Teammate::TeamSize()
{
	return m_pTeamManager->TeamSize();
}
