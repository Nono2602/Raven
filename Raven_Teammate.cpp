#include "Raven_Teammate.h"
#include "misc/Cgdi.h"
#include "misc/utils.h"
#include "2D/Transformations.h"
#include "2D/Geometry.h"
#include "lua/Raven_Scriptor.h"
#include "Raven_Game.h"
#include "navigation/Raven_PathPlanner.h"
#include "Raven_SteeringBehaviors.h"
#include "Raven_UserOptions.h"
#include "time/Regulator.h"
#include "Raven_WeaponSystem.h"
#include "Raven_SensoryMemory.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "goals/Raven_Goal_Types.h"
#include "goals/Goal_Think.h"

#include "Debug\DebugConsole.h"



Raven_Teammate::Raven_Teammate(Raven_Game* world, Vector2D pos, Raven_TeamManager* teammanager)
	: Raven_Bot(world, pos), m_pTeamManager(teammanager)
{
}


Raven_Teammate::~Raven_Teammate()
{
}


void Raven_Teammate::Update()
{
	if(GetTargetBot())
		debug_con << "Teammate " << ID() << " targeting bot " << GetTargetBot()->ID() << "";
	else
		debug_con << "Teammate " << ID() << " has no target" << "";

	//process the currently active goal. Note this is required even if the bot
	//is under user control. This is because a goal is created whenever a user 
	//clicks on an area of the map that necessitates a path planning request.
	m_pBrain->Process();

	//Calculate the steering force and update the bot's velocity and position
	UpdateMovement();

	//if the bot is under AI control but not scripted
	if (!isPossessed())
	{
		//appraise and arbitrate between all possible high level goals
		if (m_pGoalArbitrationRegulator->isReady())
		{
			m_pBrain->Arbitrate();
		}

		//update the sensory memory with any visual stimulus
		if (m_pVisionUpdateRegulator->isReady())
		{
			m_pSensoryMem->UpdateVision();
		}

		//select the appropriate weapon to use from the weapons currently in
		//the inventory
		if (m_pWeaponSelectionRegulator->isReady())
		{
			m_pWeaponSys->SelectWeapon();
		}

		//this method aims the bot's current weapon at the current target
		//and takes a shot if a shot is possible
		m_pWeaponSys->TakeAimAndShoot();
	}
}


bool Raven_Teammate::HandleMessage(const Telegram& msg)
{
	//first see if the current goal accepts the message
	if (GetBrain()->HandleMessage(msg)) return true;

	//handle any messages not handles by the goals
	switch (msg.Msg)
	{
	case Msg_TakeThatMF:

		//just return if already dead or spawning
		if (isDead() || isSpawning()) return true;

		//the extra info field of the telegram carries the amount of damage
		ReduceHealth(DereferenceToType<int>(msg.ExtraInfo));

		//if this bot is now dead let the shooter know
		if (isDead())
		{
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				ID(),
				msg.Sender,
				Msg_YouGotMeYouSOB,
				NO_ADDITIONAL_INFO);
		}

		return true;

	case Msg_YouGotMeYouSOB:

		IncrementScore();

		//the bot this bot has just killed should be removed as the target
		m_pTargSys->ClearTarget();
		m_pTeamManager->ClearTarget();

		return true;

	case Msg_GunshotSound:

		//add the source of this sound to the bot's percepts
		GetSensoryMem()->UpdateWithSoundSource((Raven_Bot*)msg.ExtraInfo);

		return true;

	case Msg_UserHasRemovedBot:
	{

		Raven_Bot* pRemovedBot = (Raven_Bot*)msg.ExtraInfo;

		GetSensoryMem()->RemoveBotFromMemory(pRemovedBot);

		//if the removed bot is the target, make sure the target is cleared
		if (pRemovedBot == GetTargetSys()->GetTarget())
		{
			GetTargetSys()->ClearTarget();
			m_pTeamManager->ClearTarget();
		}

		return true;
	}


	default: return false;
	}
}


Raven_Bot* Raven_Teammate::SearchNewTarget()
{
	if (m_pTargetSelectionRegulator->isReady())
	{
		m_pTargSys->Update();
	}

	return m_pTargSys->GetTarget();
}
