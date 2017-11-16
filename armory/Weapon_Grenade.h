#pragma once

#include "Raven_Weapon.h"

class Raven_Bot;

class GrenadeWeapon : public Raven_Weapon
{
private:
	void InitializeFuzzyModule();
public:
	GrenadeWeapon(Raven_Bot* owner);
	
	void Render();
	void ShootAt(Vector2D pos);
	double GetDesirability(double DistToTarget);
};

