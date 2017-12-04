#include "Goal_ObeyLeader.h"
#include "Raven_Follower.h"
#include "Goals/Goal_AttackTarget.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"
#include "Raven_SensoryMemory.h"

#include "Goals/Goal_Explore.h"
#include "goals/Goal_MoveToPosition.h"

#include "Debug/DebugConsole.h"

Goal_ObeyLeader::Goal_ObeyLeader(Raven_Follower * pOwner) :
	Goal_Composite<Raven_Follower>(pOwner, goal_obey_leader),
	m_bOrderIsSet(false),
	m_bOrderHasChanged(false),
	m_bLeaderPosIsSet(false)
{
}

Goal_ObeyLeader::~Goal_ObeyLeader()
{
}

void Goal_ObeyLeader::Activate()
{
	//if status is inactive, call Activate()
	//ActivateIfInactive();

	Raven_Follower* owner = static_cast<Raven_Follower*>(m_pOwner);
	if (owner->HasLeader() && m_bOrderIsSet)
	{
		if (m_bOrderHasChanged)
		{
			switch (m_order.order) {
			case order_attack:
			{
				debug_con << "Follower " << m_pOwner->ID() << " received order to attack bot " << m_order.target->ID() << "";
				m_pOwner->GetTargetSys()->SetTarget(m_order.target);
				AddGoal_AttackTarget();
			}
			break;
			default:
				break;
			}
			m_bOrderHasChanged = false;
		}
	}
	else {
		m_iStatus = completed;

		return;
	}
}

int Goal_ObeyLeader::Process()
{
	//if status is inactive, call Activate()
	ActivateIfInactive();

	//process the subgoals
	m_iStatus = ProcessSubgoals();

	ReactivateIfFailed();

	return m_iStatus;
}

void Goal_ObeyLeader::Terminate()
{
}

bool Goal_ObeyLeader::HandleMessage(const Telegram & msg)
{
	switch (msg.Msg)
	{
	case Msg_LeaderTargetLocation:
	{
		m_order = *static_cast<Leader_Order*>(msg.ExtraInfo);

		// update memory
		m_pOwner->GetSensoryMem()->AddRavenToMemory(m_order.target);

		m_bOrderIsSet = true;
		m_bOrderHasChanged = true;
	}
	case Msg_LeaderLocation:
	{
		//update known leader position
		m_bLeaderPosIsSet = true;
		m_knownLeaderPos = *static_cast<Vector2D*>(msg.ExtraInfo);
		return true;
	}
	default:
	{
		return ForwardMessageToFrontMostSubgoal(msg);
	}
	}
}

void Goal_ObeyLeader::AddGoal_AttackTarget()
{
	if (notPresent(goal_attack_target))
	{
		RemoveAllSubgoals();
		AddSubgoal(reinterpret_cast<Goal<Raven_Follower>*>(new Goal_AttackTarget(m_pOwner)));
	}
}

bool Goal_ObeyLeader::notPresent(unsigned int GoalType) const
{
	if (!m_SubGoals.empty())
	{
		return m_SubGoals.front()->GetType() != GoalType;
	}

	return true;
}
