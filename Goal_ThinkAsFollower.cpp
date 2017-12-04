#include "Goal_ThinkAsFollower.h"
#include "Goal_ThinkAsTeammate.h"
#include "Raven_Follower.h"
#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"
#include "Goal_ObeyLeader.h"
#include "ObeyLeaderGoal_Evaluator.h"

Goal_ThinkAsFollower::Goal_ThinkAsFollower(Raven_Follower* pBot) :
	Goal_ThinkAsTeammate(pBot)
{
	//these biases could be loaded in from a script on a per bot basis
	//but for now we'll just give them some random values
	const double LowRangeOfBias = 0.5;
	const double HighRangeOfBias = 1.5;

	double ObeyLeaderBias = RandInRange(LowRangeOfBias, HighRangeOfBias);

	//create the evaluator object
	m_Evaluators.push_back(new ObeyLeaderGoal_Evaluator(ObeyLeaderBias));
}


Goal_ThinkAsFollower::~Goal_ThinkAsFollower()
{
}

bool Goal_ThinkAsFollower::HandleMessage(const Telegram & msg)
{
	if (msg.Msg == Msg_LeaderTargetLocation)
	{
		Arbitrate();
		ForwardMessageToFrontMostSubgoal(msg);
		return true;
	}
	return ForwardMessageToFrontMostSubgoal(msg);
}

void Goal_ThinkAsFollower::AddGoal_ObeyLeader()
{
	if (notPresent(goal_obey_leader))
	{
		RemoveAllSubgoals();
		AddSubgoal(reinterpret_cast<Goal<Raven_Bot> *>(new Goal_ObeyLeader(static_cast<Raven_Follower*>(GetOwner()))));
	}
}
