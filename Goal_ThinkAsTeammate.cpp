#include "Goal_ThinkAsTeammate.h"
#include "goals\Goal_Think.h"
#include "Raven_Teammate.h"
#include "Goal_Regroup.h"
#include "RegroupGoal_Evaluator.h"

Goal_ThinkAsTeammate::Goal_ThinkAsTeammate(Raven_Teammate * pBot):
	Goal_Think(pBot)
{
	//these biases could be loaded in from a script on a per bot basis
	//but for now we'll just give them some random values
	const double LowRangeOfBias = 0.5;
	const double HighRangeOfBias = 1.5;

	double RegroupBias = RandInRange(LowRangeOfBias, HighRangeOfBias);

	//create the evaluator object
	m_Evaluators.push_back(new RegroupGoal_Evaluator(RegroupBias));
}

Goal_ThinkAsTeammate::~Goal_ThinkAsTeammate()
{
}

Raven_Teammate * Goal_ThinkAsTeammate::GetOwner() const
{
	return static_cast<Raven_Teammate *>(m_pOwner);
}

void Goal_ThinkAsTeammate::AddGoal_Regroup()
{
	if (notPresent(goal_regroup))
	{
		RemoveAllSubgoals();
		AddSubgoal(reinterpret_cast<Goal<Raven_Bot> *>(new Goal_Regroup(GetOwner())));
	}
}