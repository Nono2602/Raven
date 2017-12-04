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
class Regulator;

class Raven_Teammate : public Raven_Bot
{
protected:
	Raven_TeamManager*	m_pTeamManager;

	Raven_Teammate* m_pPartner;

	Vector2D m_savedPosition;

	//A regulator object limits the update frequency of a specific AI component
	Regulator*                         m_pPartnerUpdateRegulator;

public:
	Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager);
	Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager, Goal_Think* goal);
	~Raven_Teammate();

	//bots shouldn't be copied, only created or respawned
	Raven_Teammate(const Raven_Teammate&);
	Raven_Teammate& operator=(const Raven_Teammate&);

	virtual void	Update() override;
	virtual bool	HandleMessage(const Telegram& msg) override;

	void			UpdatePartner();
	bool			HasPartner();
	double			DistanceToPartner();
	Raven_Teammate* GetPartner() { return m_pPartner; }

	virtual Goal_ThinkAsTeammate* const       GetBrain() override;
	Raven_TeamManager*                        GetTeam() const;
	bool                                      HasLeader() const;

	int TeamSize();

	virtual bool                       HasTag(int tag) const override;
};

#endif