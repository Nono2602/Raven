#include "Raven_Leader.h"

#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

Raven_Leader::Raven_Leader(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager):
	Raven_Leader(world, pos, teammanager, new Goal_Think(this))
{
}

Raven_Leader::Raven_Leader(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager, Goal_Think * goal):
	Raven_Teammate(world, pos, teammanager, goal)
{
}

Raven_Leader::~Raven_Leader()
{
}

void Raven_Leader::Update()
{
	Raven_Bot::Update();

	Raven_Bot* target = GetTargetBot();
	if (target && !target->isDead()) {
		m_order.target = target;
		m_order.order = order_attack;
		m_pTeamManager->SendMessageToAllTeammates(this, SEND_MSG_IMMEDIATELY, Msg_AttackThisTarget, static_cast<void*>(&m_order));
	}
}

bool Raven_Leader::HasTag(int tag) const
{
	return Raven_Teammate::HasTag(tag) || tag == leader_bot;
}
