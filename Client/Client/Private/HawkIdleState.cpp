#include "stdafx.h"

#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkWalkState.h"
#include "HawkTurnR_State.h"
#include "HawkBraveState.h"
#include "HawkSitOnState.h"


using namespace Hawk;
\
CIdleState::CIdleState(CHawk* pIceWolf, FIELD_STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_pOwner->Check_Navigation();
	
	return nullptr;
}

CHawkState * CIdleState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	else
	{
		if (m_fTimeDeltaAcc > m_fIdleTime)
		{
			switch (rand() % 3)
			{
			case 0:
				return new CWalkState(m_pOwner);
			case 1:
				return new CIdleState(m_pOwner, FIELD_STATE_END);
			case 2:
				return new CTurn_State(m_pOwner);
	/*		case 3:
				return new CTurnR_State(m_pOwner);*/
			default:
				break;
			}
		}
	}












	// ���� �� �ڵ� 
	/*else
	{
		if (m_fIdleMoveTimer > 5.f)
		{
			switch (m_ePreState_Id)
			{
			case CHawkState::FIELD_STATE_ID::FIELD_STATE_IDLE:
				return new CWalkState(m_pOwner);

			case CHawkState::FIELD_STATE_ID::STATE_WALK:
				return new CTurnR_State(m_pOwner);
				break;

			case CHawkState::FIELD_STATE_ID::STATE_TURN_R:
				return new CBraveState(m_pOwner);
				break;

			case CHawkState::FIELD_STATE_ID::STATE_BRAVE:
				return new CWalkState(m_pOwner);
			default:
				break;
			}

		}
			
	}*/



	

	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_IDLE);
}

void CIdleState::Exit()
{

}