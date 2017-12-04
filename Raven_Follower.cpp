#include "Raven_Follower.h"
#include "Raven_Teammate.h"
#include "Goal_ThinkAsFollower.h"

Raven_Follower::Raven_Follower(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager):
	Raven_Teammate(world, pos, teammanager, new Goal_ThinkAsFollower(this))
{
}


Raven_Follower::~Raven_Follower()
{
}

bool Raven_Follower::HasTag(int tag) const
{
	return Raven_Teammate::HasTag(tag) || tag == follower_bot;
}
