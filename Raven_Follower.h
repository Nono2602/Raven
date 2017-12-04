#ifndef RAVEN_FOLLOWER
#define RAVEN_FOLLOWER
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_Follower.h
//
//  Author: Loic Mouton
//
//  Desc:   this class creates and stores all the entities that handle the
//          Raven bot followers.
//
//          this class has methods for updating the game entities.
//-----------------------------------------------------------------------------
#include "Raven_Teammate.h"
#include "Raven_Leader.h"
#include "Goal_ThinkAsFollower.h"

class Goal_ThinkAsFollower;

class Raven_Follower : public Raven_Teammate
{
public:
	Raven_Follower(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager);
	~Raven_Follower();

	//bots shouldn't be copied, only created or respawned
	Raven_Follower(const Raven_Follower&);
	Raven_Follower& operator=(const Raven_Follower&);

	virtual Goal_ThinkAsFollower* const       GetBrain() override;

	virtual bool                       HasTag(int tag) const override;
};
#endif