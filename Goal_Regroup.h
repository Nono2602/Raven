#ifndef GOAL_REGROUP
#define GOAL_REGROUP
#pragma warning (disable:4786)

#include "Goals/Goal_Composite.h"
#include "goals/Raven_Goal_Types.h"


class Raven_Teammate;


class Goal_Regroup : public Goal_Composite<Raven_Teammate>
{
private:

	Vector2D  m_CurrentDestination;

	//set to true when the destination for the exploration has been established
	bool      m_bDestinationIsSet;

public:
	Goal_Regroup(Raven_Teammate* pOwner);
	~Goal_Regroup();

	void Activate();

	int Process();

	void Terminate() {}

	bool HandleMessage(const Telegram& msg);
};
#endif