#include "Raven_TeammateSensoryMemory.h"

#include "Raven_SensoryMemory.h"
#include "Raven_Game.h"
#include "time/crudetimer.h"
#include "misc/cgdi.h"
#include "misc/Stream_Utility_Functions.h"

Raven_TeammateSensoryMemory::Raven_TeammateSensoryMemory(Raven_Teammate * owner, double MemorySpan):
	Raven_SensoryMemory(owner, MemorySpan)
{
}

Raven_TeammateSensoryMemory::~Raven_TeammateSensoryMemory()
{
}

void Raven_TeammateSensoryMemory::UpdateVision()
{
	//for each bot in the world test to see if it is visible to the owner of
	//this class
	const std::list<Raven_Bot*>& bots = m_pOwner->GetWorld()->GetAllBots();
	std::list<Raven_Bot*>::const_iterator curBot;
	for (curBot = bots.begin(); curBot != bots.end(); ++curBot)
	{
		//make sure the bot being examined is not this bot
		if (m_pOwner != *curBot && !static_cast<Raven_Teammate*>(m_pOwner)->GetTeam()->isTeammate(*curBot))
		{
			//make sure it is part of the memory map
			MakeNewRecordIfNotAlreadyPresent(*curBot);

			//get a reference to this bot's data
			MemoryRecord& info = m_MemoryMap[*curBot];

			//test if there is LOS between bots 
			if (m_pOwner->GetWorld()->isLOSOkay(m_pOwner->Pos(), (*curBot)->Pos()))
			{
				info.bShootable = true;

				//test if the bot is within FOV
				if (isSecondInFOVOfFirst(m_pOwner->Pos(),
					m_pOwner->Facing(),
					(*curBot)->Pos(),
					m_pOwner->FieldOfView()))
				{
					info.fTimeLastSensed = Clock->GetCurrentTime();
					info.vLastSensedPosition = (*curBot)->Pos();
					info.fTimeLastVisible = Clock->GetCurrentTime();

					if (info.bWithinFOV == false)
					{
						info.bWithinFOV = true;
						info.fTimeBecameVisible = info.fTimeLastSensed;

					}
				}

				else
				{
					info.bWithinFOV = false;
				}
			}

			else
			{
				info.bShootable = false;
				info.bWithinFOV = false;
			}
		}
	}//next bot
}
