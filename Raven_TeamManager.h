#ifndef RAVEN_TEAMMANAGER
#define RAVEN_TEAMMANAGER
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_TeamManager.h
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
#include "Raven_Teammate.h"

class Raven_Teammate;
class Raven_Leader;

enum {
	teamgoal_attack,
	teamgoal_protect
};

class Raven_TeamManager
{
private:
	std::list<Raven_Teammate*>	m_Teammates;

	Raven_Leader*				m_pLeader;

	Raven_Bot*					m_pTarget;
	Raven_Teammate*				m_pWeakTeammate;

	int goal;

	Vector2D					m_WeaponSpawn;
	bool						m_WeaponAvailable;

public:
	Raven_TeamManager(Vector2D weaponspawn);
	~Raven_TeamManager();

	void		AddTeammate(Raven_Teammate* pBot);
	Raven_Bot*	RemoveATeammate();

	void			SetLeader(Raven_Leader* pBot) { m_pLeader = pBot; }
	Raven_Leader*	GetLeader() const { return m_pLeader; }
	bool			HasLeader() const { return m_pLeader != NULL;  }

	bool		isEmpty() { return m_Teammates.empty(); }

	bool		isTeammate(Raven_Bot* pTarget);

	void		Update();
	void		Clear();

	Raven_Bot*	GetTeamTarget()const { return m_pTarget; }

	bool		isWeaponAvailable()const { return m_WeaponAvailable; }
	Vector2D	GetWeaponSpawn()const { return m_WeaponSpawn; }

	int					TeamSize() const;
	
	void	SendMessageToAllTeammates(Raven_Teammate* sender, double delay, int msg, void* AdditionalInfo = NULL);
	void	SendMessageToLeader(Raven_Teammate* sender, double delay, int msg, void* AdditionalInfo = NULL);
};

#endif