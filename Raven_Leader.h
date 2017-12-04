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

enum {
	order_attack,
};

typedef struct {
	Raven_Bot* target;
	int order;
} Leader_Order;

class Raven_Leader : public Raven_Teammate
{
private:
	Leader_Order m_order;

public:
	Raven_Leader(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager);
	Raven_Leader(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager, Goal_Think* goal);
	~Raven_Leader();

	virtual void	Update() override;

	virtual bool                       HasTag(int tag) const override;
};
#endif
