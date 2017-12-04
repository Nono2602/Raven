#include "Raven_Leader.h"

#include "lua/Raven_Scriptor.h"
#include "Raven_SensoryMemory.h"
#include "time/Regulator.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "Debug\DebugConsole.h"

Raven_Leader::Raven_Leader(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager):
	Raven_Leader(world, pos, teammanager, new Goal_Think(this))
{
}

Raven_Leader::Raven_Leader(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager, Goal_Think * goal):
	Raven_Teammate(world, pos, teammanager, goal),
	m_savedTarget(NULL)
{
	m_pLocationSendingRegulator = new Regulator(script->GetDouble("Leader_LocationSendingFreq"));
	m_pLocationTargetRegulator = new Regulator(script->GetDouble("Leader_LocationTargetFreq"));
}

Raven_Leader::~Raven_Leader()
{
}

void Raven_Leader::Update()
{
	if (isPossessed())
	{
		m_savedTarget = NULL;
	}
	else
	{
		bool targetChanged = false;
		Raven_Bot* target = GetTargetBot();
		if (target && target != m_savedTarget && !target->isDead() && TeamSize() > 1) {
			m_savedTarget = target;
			targetChanged = true;
		}
		else if (!target) {
			m_savedTarget = NULL;
		}
		if (m_pLocationSendingRegulator->isReady() && TeamSize() > 1) {
			//send location to teammates
			m_savedPosition = Pos();
			m_pTeamManager->SendMessageToAllTeammates(this, SEND_MSG_IMMEDIATELY, Msg_LeaderLocation, static_cast<void*>(&m_savedPosition));
		}
		if (target && (m_pLocationTargetRegulator->isReady() || targetChanged) && TeamSize() > 1) {
			//send target location to teammates
			//prepare order
			m_order.target = target;
			m_order.order = order_attack;
			m_pTeamManager->SendMessageToAllTeammates(this, SEND_MSG_IMMEDIATELY, Msg_LeaderTargetLocation, static_cast<void*>(&m_order));
		}
	}
	Raven_Bot::Update();
}

bool Raven_Leader::HasTag(int tag) const
{
	return Raven_Teammate::HasTag(tag) || tag == leader_bot;
}
