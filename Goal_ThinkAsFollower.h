#ifndef GOAL_THINKASFOLLOWER
#define GOAL_THINKASFOLLOWER
//-----------------------------------------------------------------------------
//
//  Name:   Goal_ThinkAsFollowers.h
//
//  Author: Loic Mouton
//
//  Desc:   this class holds the strategy choice methods of followers bots.
//-----------------------------------------------------------------------------
#include "Goal_ThinkAsTeammate.h"

class Raven_Follower;

class Goal_ThinkAsFollower : public Goal_ThinkAsTeammate
{
public:
	Goal_ThinkAsFollower(Raven_Follower* pBot);
	~Goal_ThinkAsFollower();

	/*virtual void Arbitrate() override;*/

	virtual bool HandleMessage(const Telegram& msg) override;

	//top level goal type
	void AddGoal_ObeyLeader();
};
#endif

