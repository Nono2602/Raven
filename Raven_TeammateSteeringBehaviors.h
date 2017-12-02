#ifndef RAVEN_TEAMMATESTREERING
#define RAVEN_TEAMMATESTREERING
//-----------------------------------------------------------------------------
//
//  Name:   Raven_TeammateSteeringBehaviors.h
//
//  Author: Loic Mouton
//
//  Desc:   this class holds the methods used by teammate bots to move around.
//-----------------------------------------------------------------------------
#include "Raven_SteeringBehaviors.h"
#include "Raven_Teammate.h"

class Raven_TeammateSteering : public Raven_Steering
{
public:
	Raven_TeammateSteering(Raven_Game* world, Raven_Teammate* agent);
	virtual ~Raven_TeammateSteering();
};

#endif