#ifndef RAVEN_LEADER
#define RAVEN_LEADER
//-----------------------------------------------------------------------------
//
//  Name:   Raven_Leader.h
//
//  Author: Loic Mouton
//
//  Desc:   this class encapsulates the team leader behavior.
//-----------------------------------------------------------------------------
#include "Raven_Teammate.h"

class Raven_Leader : public Raven_Teammate
{
public:
	Raven_Leader(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager);
	~Raven_Leader();

	virtual void	Update() override;
};
#endif
