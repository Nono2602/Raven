#include "ObeyLeaderGoal_Evaluator.h"
#include "Raven_Follower.h"
#include "goals/GetHealthGoal_Evaluator.h"
#include "Raven_ObjectEnumerations.h"
#include "goals/Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "goals/Raven_Feature.h"
#include "Goal_ThinkAsFollower.h"

ObeyLeaderGoal_Evaluator::ObeyLeaderGoal_Evaluator(double bias) :
	Goal_Evaluator(bias)
{
}

ObeyLeaderGoal_Evaluator::~ObeyLeaderGoal_Evaluator()
{
}

double ObeyLeaderGoal_Evaluator::CalculateDesirability(Raven_Bot * pBot)
{
	Raven_Follower * pTarget = static_cast<Raven_Follower *>(pBot);
	if (pTarget->TeamSize() <= 1 || !pTarget->HasLeader()) {
		return 0.0;
	}
	const double Tweaker = 0.9;

	return Tweaker * m_dCharacterBias;
}

void ObeyLeaderGoal_Evaluator::SetGoal(Raven_Bot * pBot)
{
	Raven_Follower * pTarget = static_cast<Raven_Follower *>(pBot);
	pTarget->GetBrain()->AddGoal_ObeyLeader();
}

void ObeyLeaderGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot * pBot)
{
	gdi->TextAtPos(Position, "R: " + ttos(CalculateDesirability(pBot), 2));
	return;
}
