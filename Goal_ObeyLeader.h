#ifndef GOAL_OBEYLEADER
#define GOAL_OBEYLEADER
//-----------------------------------------------------------------------------
//
//  Name:   Goal_ObeyLeader.h
//
//  Author: Loic Mouton
//
//  Desc:   this class holds the methods used by the main goal of follower bots.
//-----------------------------------------------------------------------------

#include "Goals/Goal_Composite.h"
#include "Goals/Raven_Goal_Types.h"
#include "Raven_Leader.h"

class Raven_Follower;

class Goal_ObeyLeader : public Goal_Composite<Raven_Follower>
{
private:
	bool m_bOrderIsSet;
	bool m_bOrderHasChanged;
	Leader_Order m_order;

	bool m_bLeaderPosIsSet;
	Vector2D m_knownLeaderPos;

public:
	Goal_ObeyLeader(Raven_Follower* pOwner);
	~Goal_ObeyLeader();

	void Activate();

	int Process();

	void Terminate();

	bool HandleMessage(const Telegram& msg);

	//top level goal types
	void AddGoal_AttackTarget();

	//returns true if the given goal is not at the front of the subgoal list
	bool notPresent(unsigned int GoalType)const;
};
#endif
