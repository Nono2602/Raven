#include "Weapon_Grenade.h"

#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "Game\BaseGameEntity.h"

GrenadeWeapon::GrenadeWeapon(Raven_Bot* owner) :
	Raven_Weapon(type_grenade,
		script->GetInt("Grenade_DefaultRounds"),
		script->GetInt("Grenade_MaxRoundsCarried"),
		script->GetDouble("Grenade_FiringFreq"),
		script->GetDouble("Grenade_IdealRange"),
		script->GetDouble("Grenade_MaxSpeed"),
		owner)
{
	InitializeFuzzyModule();
}

inline void GrenadeWeapon::ShootAt(Vector2D pos)
{
	if (NumRoundsRemaining() > 0 && isReadyForNextShot())
	{
		//Fire off a grenade !
		m_pOwner->GetWorld()->AddGrenade(m_pOwner, pos);
		m_iNumRoundsLeft--;
		UpdateTimeWeaponIsNextAvailable();

		//Add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("Grenade_SoundRange"));
	}
}

double GrenadeWeapon::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0)
		m_dLastDesirabilityScore = 0;
	else
	{
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
	}

	return m_dLastDesirabilityScore;
}


void GrenadeWeapon::InitializeFuzzyModule()
{
	//All the fuzzy rules and variables are taken from the RocketLauncher
	//since a grenade is used in almost the same cases
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");
	FzSet& Target_VeryClose = DistToTarget.AddLeftShoulderSet("Target_VeryClose", 0, 25, 75);
	FzSet& Target_Close = DistToTarget.AddTriangularSet("Target_Close", 25, 75, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 75, 150, 300);
	FzSet& Target_Far = DistToTarget.AddTriangularSet("Target_Far", 150, 300, 750);
	FzSet& Target_VeryFar = DistToTarget.AddRightShoulderSet("Target_VeryFar", 300, 750, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& ExtremelyDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 75, 85, 100);
	FzSet& VeryDesirable = Desirability.AddTriangularSet("VeryDesirable", 50, 85, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 35, 50, 50);
	FzSet& Undesirable = Desirability.AddTriangularSet("Undesirable", 5, 10, 50);
	FzSet& VeryUndesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 5, 10);

	FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
	FzSet& Ammo_Full = AmmoStatus.AddRightShoulderSet("Ammo_Full", 75, 90, 100);
	FzSet& Ammo_Loads = AmmoStatus.AddTriangularSet("Ammo_Loads", 10, 75, 90);
	FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 5, 10, 75);
	FzSet& Ammo_Low = AmmoStatus.AddLeftShoulderSet("Ammo_Low", 1, 5, 10);
	FzSet& Ammo_Empty = AmmoStatus.AddSingletonSet("Ammo_Empty", 0, 0, 0);

	m_FuzzyModule.AddRule(FzAND(Target_VeryClose, Ammo_Full), VeryUndesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryClose, Ammo_Loads), VeryUndesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryClose, Ammo_Okay), VeryUndesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryClose, Ammo_Low), VeryUndesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryClose, Ammo_Empty), VeryUndesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Full), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Empty), VeryUndesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Full), ExtremelyDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), ExtremelyDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Empty), VeryUndesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Full), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Empty), VeryUndesirable);

	m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Full), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Loads), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Okay), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Low), VeryUndesirable);
	m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Empty), VeryUndesirable);
}

void GrenadeWeapon::Render()
{
	gdi->RedPen();
	gdi->OrangeBrush();
	gdi->Circle(m_pOwner->Pos(), 2);
}
