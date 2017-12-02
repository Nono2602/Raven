#ifndef GOAL_THINKASTEAMMATE
#define GOAL_THINKASTEAMMATE
//-----------------------------------------------------------------------------
//
//  Name:   Goal_ThinkAsTeammate.h
//
//  Author: Loic Mouton
//
//  Desc:   this class holds the strategy choice methods of teammate bots.
//-----------------------------------------------------------------------------
#include "goals\Goal_Think.h"

class Raven_Teammate;

class Goal_ThinkAsTeammate : public Goal_Think
{
public:
	Goal_ThinkAsTeammate(Raven_Teammate* pBot);
	virtual ~Goal_ThinkAsTeammate();

	//top level goal type
	void AddGoal_Regroup();

	virtual Raven_Teammate * GetOwner() const;
};

#endif