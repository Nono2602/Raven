#ifndef REGROUPGOAL_EVALUATOR
#define REGROUPGOAL_EVALUATOR
//-----------------------------------------------------------------------------
//
//  Name:   RegroupGoal_Evaluator.h
//
//  Author: Loic Mouton
//
//  Desc:   class to calculate how desirable the goal of regrouping is
//-----------------------------------------------------------------------------
#include "goals\Goal_Evaluator.h"

class Raven_Teammate;

class RegroupGoal_Evaluator :
	public Goal_Evaluator
{
public:
	RegroupGoal_Evaluator(double bias);
	~RegroupGoal_Evaluator();

	virtual double CalculateDesirability(Raven_Bot* pBot) override;

	virtual void  SetGoal(Raven_Bot* pEnt) override;

	virtual void RenderInfo(Vector2D Position, Raven_Bot* pBot) override;
};
#endif