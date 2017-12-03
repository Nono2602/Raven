#include "Raven_Leader.h"

Raven_Leader::Raven_Leader(Raven_Game * world, Vector2D pos, Raven_TeamManager * teammanager):
	Raven_Teammate(world, pos, teammanager, new Goal_Think(this))
{
}

Raven_Leader::~Raven_Leader()
{
}

void Raven_Leader::Update()
{
	Raven_Bot::Update();
	

}
