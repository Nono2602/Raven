#ifndef OBEYLEADERGOAL_EVALUATOR
#define OBEYLEADERGOAL_EVALUATOR
//-----------------------------------------------------------------------------
//
//  Name:   ObeyLeader_Evaluator.h
//
//  Author: Loic Mouton
//
//  Desc:   class to calculate how desirable the goal of obeying is
//-----------------------------------------------------------------------------
#include "goals\Goal_Evaluator.h"
class ObeyLeaderGoal_Evaluator :
	public Goal_Evaluator
{
public:
	ObeyLeaderGoal_Evaluator(double bias);
	~ObeyLeaderGoal_Evaluator();

	virtual double CalculateDesirability(Raven_Bot* pBot) override;

	virtual void  SetGoal(Raven_Bot* pEnt) override;

	virtual void RenderInfo(Vector2D Position, Raven_Bot* pBot) override;
};
#endif
