#include "Raven_Follower.h"
#include "Raven_Teammate.h"


Raven_Follower::Raven_Follower(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager):
	Raven_Teammate(world, pos, teammanager),
	m_bOrderIsSet(false)
{
}


Raven_Follower::~Raven_Follower()
{
}

bool Raven_Follower::HasTag(int tag) const
{
	return Raven_Teammate::HasTag(tag) || tag == follower_bot;
}

void Raven_Follower::SetOrder(const Leader_Order & order)
{
	m_bOrderIsSet = true;
	m_order.target = order.target;
	m_order.order = order.order;
}

const Leader_Order & Raven_Follower::GetOrder() const
{
	return m_order;
}
