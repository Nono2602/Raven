#include "Goal_ThinkAsFollower.h"
#include "Goal_ThinkAsTeammate.h"
#include "Raven_Follower.h"
#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"
#include "Goal_ObeyLeader.h"

Goal_ThinkAsFollower::Goal_ThinkAsFollower(Raven_Follower* pBot) :
	Goal_ThinkAsTeammate(pBot)
{
}


Goal_ThinkAsFollower::~Goal_ThinkAsFollower()
{
}

void Goal_ThinkAsFollower::Arbitrate()
{
	/*if (static_cast<Raven_Teammate*>(m_pOwner)->HasLeader()) {
		AddGoal_ObeyLeader();
	}
	else {*/
		Goal_ThinkAsTeammate::Arbitrate();
	/*}*/
}

bool Goal_ThinkAsFollower::HandleMessage(const Telegram & msg)
{
	switch (msg.Msg)
	{
	case Msg_AttackThisTarget:
	{
		static_cast<Raven_Follower*>(m_pOwner)->SetOrder(*static_cast<Leader_Order *>(msg.ExtraInfo));
		AddGoal_ObeyLeader();
		return true;
	}
	default:
	{
		return ForwardMessageToFrontMostSubgoal(msg);
	}
	}
}

void Goal_ThinkAsFollower::AddGoal_ObeyLeader()
{
	if (notPresent(goal_obey_leader))
	{
		RemoveAllSubgoals();
		AddSubgoal(reinterpret_cast<Goal<Raven_Bot> *>(new Goal_ObeyLeader(static_cast<Raven_Follower*>(GetOwner()))));
	}
}
