#ifndef RAVEN_TMT_SENSORY_MEMORY
#define RAVEN_TMT_SENSORY_MEMORY
//-----------------------------------------------------------------------------
//
//  Name:   Raven_TeammateSensoryMemory.h
//
//  Author: Loic Mouton
//
//  Desc:   this class is the Raven_SensoryMemory derived class adapted to
//          teammate bots
//-----------------------------------------------------------------------------

#include "Raven_SensoryMemory.h"

class Raven_Teammate;

class Raven_TeammateSensoryMemory : public Raven_SensoryMemory
{
public:
	Raven_TeammateSensoryMemory(Raven_Teammate* owner, double MemorySpan);
	~Raven_TeammateSensoryMemory();

	//this method iterates through all the opponents in the game world and 
	//updates the records of those that are in the owner's FOV
	virtual void     UpdateVision() override;
};
#endif