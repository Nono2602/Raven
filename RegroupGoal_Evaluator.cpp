#include "RegroupGoal_Evaluator.h"
#include "Raven_Teammate.h"
#include "goals/GetHealthGoal_Evaluator.h"
#include "Raven_ObjectEnumerations.h"
#include "goals/Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "goals/Raven_Feature.h"
#include "Goal_ThinkAsTeammate.h"

RegroupGoal_Evaluator::RegroupGoal_Evaluator(double bias):
	Goal_Evaluator(bias)
{
}

RegroupGoal_Evaluator::~RegroupGoal_Evaluator()
{
}

double RegroupGoal_Evaluator::CalculateDesirability(Raven_Bot * pBot)
{
	Raven_Teammate * pTarget = static_cast<Raven_Teammate *>(pBot);
	if (pTarget->TeamSize() <= 1) {
		return 0.0;
	}
	double distance = pTarget->GetRegroupLocation().Distance(pTarget->Pos());

	const double distanceMin = 50.0;
	const double distanceMax = 100.0;

	if (distance <= distanceMin) {
		return 0.0;
	}
	else // distance > distanceMin
	{ 
		//value used to tweak the desirability
		const double Tweaker = 0.7;

		double Desirability = Tweaker * (distance - distanceMin) / (distanceMax - distanceMin);

		//ensure the value is in the range 0 to 1
		Clamp(Desirability, 0, 1);

		//bias the value according to the personality of the bot
		Desirability *= m_dCharacterBias;

		return Desirability;
	}
}

void RegroupGoal_Evaluator::SetGoal(Raven_Bot * pBot)
{
	Raven_Teammate * pTarget = static_cast<Raven_Teammate *>(pBot);
	pTarget->GetBrain()->AddGoal_Regroup();
}

void RegroupGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot * pBot)
{
	gdi->TextAtPos(Position, "R: " + ttos(CalculateDesirability(pBot), 2));
	return;
}
