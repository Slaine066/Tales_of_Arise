#include "stdafx.h"

#include "IceWolfIdleState.h"
#include "GameInstance.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CIdleState::CIdleState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target_InField();
	//m_pCurTarget = m_pOwner->Check_FiledTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	/*if (!m_bIsAnimationFinished)
		m_pOwner->Check_Navigation();*/

	return nullptr;
}

CIceWolfState * CIdleState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);



	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
	{
		if (m_pTarget)
		{
			return new CChaseState(m_pOwner);
		}
		else
		{
			if (m_fTimeDeltaAcc > m_fIdleTime)
			{
				switch (rand() % 4)
				{
				case 0:
					return new CWalkState(m_pOwner, FIELD_STATE_END);
				case 1:
					return new CIdleState(m_pOwner);
				case 2:
					return new CHowLingState(m_pOwner);
				case 3:
					return new CTurnLeftState(m_pOwner);
				default:
					break;
				}
			}
		}
	}

	else
	{
		return new CTurnLeftState(m_pOwner);
	}



	//0310 수정전 코드
	/*if (m_pTarget)
	{
	return new CChaseState(m_pOwner);
	}

	else
	{
	if (m_fTimeDeltaAcc > m_fIdleTime)
	{
	switch (rand() % 4)
	{
	case 0:
	return new CWalkState(m_pOwner, FIELD_STATE_END);
	case 1:
	return new CIdleState(m_pOwner);
	case 2:
	return new CHowLingState(m_pOwner);
	case 3:
	return new CTurnLeftState(m_pOwner);
	default:
	break;
	}
	}
	}*/



	//원본코드
	//else
	//{
	//	if (m_fIdleMoveTimer > 3.5f)
	//	{
	//		
	//		switch (m_ePreState_Id)
	//		{
	//		case Client::CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE:
	//			return new CWalkState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	//			break;
	//		case Client::CIceWolfState::FIELD_STATE_ID::STATE_TURN_L:
	//			return new CIdleState(m_pOwner, FIELD_STATE_ID::STATE_TURN);
	//			break;
	//		case Client::CIceWolfState::STATE_TURN:
	//			return new CWalkState(m_pOwner, STATE_TURN);
	//			break;
	//		case Client::CIceWolfState::FIELD_STATE_ID::STATE_HOWLING:
	//			return new CIdleState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE);
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}


	return nullptr;
}

//void CIdleState::Find_Target_InField(void)
//{
//	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
//	if (!pPlayer)
//		return;
//
//	_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
//	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
//
//	_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
//	m_fTarget_Distance = fDistance;
//	if (fDistance < m_pOwner->Get_AggroRadius())
//	{
//		m_pTarget = pPlayer;
//
//		m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);
//
//		if (5 > fDistance)
//			m_bBattleMode = true;
//	}
//}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CIdleState::Exit()
{

}