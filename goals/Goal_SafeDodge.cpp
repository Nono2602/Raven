#include "Goal_SafeDodge.h"
#include "Goal_SeekToPosition.h"
#include "../Raven_Bot.h"
#include "../Raven_SteeringBehaviors.h"
#include "../Raven_Game.h"

#include "Messaging/Telegram.h"
#include "../Raven_Messages.h"

#include "debug/DebugConsole.h"
#include "misc/cgdi.H"


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void Goal_SafeDodge::Activate()
{
  static const double StepDistance = m_pOwner->BRadius()*3;
  m_iStatus = active;

  m_pOwner->GetSteering()->SeekOn();

  
    if (m_bClockwise)
    {
		m_vStrafeTarget = m_pOwner->Pos() + m_pOwner->Facing().Perp() * StepDistance + m_pOwner->Facing() * m_pOwner->BRadius()*2;
		if (m_pOwner->canWalkTo (m_vStrafeTarget))
      {
        m_pOwner->GetSteering()->SetTarget(m_vStrafeTarget);
      }
      else
      {
        //debug_con << "changing" << "";
        m_bClockwise = !m_bClockwise;
        m_iStatus = inactive;
      }
    }

    else
    {
		m_vStrafeTarget = m_pOwner->Pos() - m_pOwner->Facing().Perp() * StepDistance + m_pOwner->Facing() * m_pOwner->BRadius()*2;
      if (m_pOwner->canWalkTo(m_vStrafeTarget))
      {
        m_pOwner->GetSteering()->SetTarget(m_vStrafeTarget);
      }
      else
      {
       // debug_con << "changing" << "";
        m_bClockwise = !m_bClockwise;
        m_iStatus = inactive;
      }
    }   
}



//-------------------------- Process ------------------------------------------
//-----------------------------------------------------------------------------
int Goal_SafeDodge::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive(); 

	if(m_strafe_once)
		 m_iStatus = completed;
  

  //else if bot reaches the target position set status to inactive so the goal 
  //is reactivated on the next update-step
 if (m_pOwner->isAtPosition(m_vStrafeTarget))
  {
    m_iStatus = inactive;
	m_strafe_once = true;
  }

  return m_iStatus;
}

//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_SafeDodge::Terminate()
{
  m_pOwner->GetSteering()->SeekOff();
}

//---------------------------- Render -----------------------------------------

void Goal_SafeDodge::Render()
{
//#define SHOW_TARGET
#ifdef SHOW_TARGET
  gdi->OrangePen();
  gdi->HollowBrush();

  gdi->Line(m_pOwner->Pos(), m_vStrafeTarget);
  gdi->Circle(m_vStrafeTarget, 3);
#endif
  
}



