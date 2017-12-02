#ifndef RAVEN_TEAMMATE
#define RAVEN_TEAMMATE
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_Teammate.h
//
//  Author: Elise Combette
//
//  Desc:   this class creates and stores all the entities that handle the
//          Raven bots team.
//
//          this class has methods for updating the game entities.
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>

#include "misc/utils.h"
#include "Raven_Bot.h"
#include "Raven_TeamManager.h"
#include "Raven_TargetingSystem.h"
#include "Goal_ThinkAsTeammate.h"

class Raven_TeamManager;

class Raven_Teammate : public Raven_Bot
{
protected:
	Raven_TeamManager*	m_pTeamManager;

	Vector2D m_regroupLocation;
	void UpdateRegroupLocation();

public:
	Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager);
	Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager, Goal_Think* goal);
	~Raven_Teammate();

	//bots shouldn't be copied, only created or respawned
	Raven_Teammate(const Raven_Teammate&);
	Raven_Teammate& operator=(const Raven_Teammate&);

	virtual void	Update() override;
	virtual bool	HandleMessage(const Telegram& msg) override;

	Raven_Bot*		SearchNewTarget();
	void			UpdateTeamTarget(Raven_Bot* newtarget);

	virtual Goal_ThinkAsTeammate* const       GetBrain() override;

	Vector2D GetRegroupLocation();
	int TeamSize();
};

#endif