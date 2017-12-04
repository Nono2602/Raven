#include "Goal_ObeyLeader.h"
#include "Raven_Follower.h"
#include "Goals/Goal_AttackTarget.h"

Goal_ObeyLeader::Goal_ObeyLeader(Raven_Follower * pOwner):
	Goal_Composite<Raven_Follower>(pOwner, goal_obey_leader)
{
}

Goal_ObeyLeader::~Goal_ObeyLeader()
{
}

void Goal_ObeyLeader::Activate()
{
}

int Goal_ObeyLeader::Process()
{
	//if status is inactive, call Activate()
	ActivateIfInactive();

	Raven_Follower* owner = static_cast<Raven_Follower*>(m_pOwner);
	if (owner->HasLeader()) {
		//process the subgoals
		ProcessSubgoals();

		m_iStatus = active;
		return m_iStatus;
	}
	else {
		m_iStatus = failed;
		return m_iStatus;
	}
}

void Goal_ObeyLeader::Terminate()
{
}

bool Goal_ObeyLeader::HandleMessage(const Telegram & msg)
{
	return false;
}

void Goal_ObeyLeader::AddGoal_AttackTarget()
{
	if (notPresent(goal_attack_target))
	{
		RemoveAllSubgoals();
		AddSubgoal(reinterpret_cast<Goal<Raven_Follower>*>( new Goal_AttackTarget(m_pOwner)));
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
