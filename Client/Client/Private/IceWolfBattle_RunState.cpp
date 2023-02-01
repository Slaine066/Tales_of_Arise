#include "stdafx.h"
#include "..\Public\IceWolfBattle_RunState.h"
#include "GameInstance.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfBattle_IdleState.h"

using namespace IceWolf;

CBattle_RunState::CBattle_RunState(class CIce_Wolf* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_fTimeDletaAcc = 0;
	m_fRandTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();

	//m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	//if (m_pTarget == nullptr)
	//	return nullptr;

	m_fTimeDletaAcc += fTimeDelta;

	//if (m_fTimeDletaAcc > m_fRandTime)
	//	m_iRand = rand() % 6;
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	
	//if (m_ePreState_Id == CIceWolfState::STATE_ID::STATE_SOMESAULT)
	//{
	//	if (2.f < m_fTarget_Distance)
	//	{
	//		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.1f);

	//	}

	//	else
	//		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
	//}
		if (4.5f < m_fTarget_Distance)
		{
			m_pOwner->Get_Transform()->LookAt(vTargetPosition);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.1f);

		}

	else
	{
		switch (m_ePreState_Id)
		{

		case CIceWolfState::STATE_ID::START_BATTLE:
			return new CAttackNormalState(m_pOwner);
			break;

		case CIceWolfState::STATE_ID::STATE_NORMAL_ATK:
			return new CBattle_BackStepState(m_pOwner);

		case CIceWolfState::STATE_ID::STATE_BACKSTEP:
			return new CAttackBiteState(m_pOwner);
			break;

		case CIceWolfState::STATE_ID::STATE_BITE:
			return new CBattle_SomerSaultState(m_pOwner);

		case CIceWolfState::STATE_ID::STATE_ELEMENTAL_CHARGE:
			return new CAttackNormalState(m_pOwner);

	/*	case CIceWolfState::STATE_ID::STATE_IDLE:
			return new CAttackNormalState(m_pOwner);*/
			
		case CIceWolfState::STATE_ID::STATE_SOMESAULT:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);

		default:
			break;
		}
	}




	//�׽�Ʈ�ڵ�
	//else 
	//{
	//	if (4.7f < m_fTarget_Distance)
	//	{
	//		//m_pOwner->Set_Speed(5.f);
	//		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.1f);

	//	}

	//	else
	//	{
	//		if (m_fTimeDletaAcc > m_fRandTime)
	//		{
	//			switch (rand() % 5)
	//			{
	//			case 0:
	//				return new CAttackNormalState(m_pOwner);

	//			case 1:
	//				return new CBattle_BackStepState(m_pOwner);

	//			case 2:
	//				return new CAttackBiteState(m_pOwner);

	//			case 3:
	//				return new CBattle_SomerSaultState(m_pOwner);

	//			case 4:
	//				return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BATTLEING);

	//			default:
	//				break;
	//			}
	//		}
	//	}

	//	int a = 0;






		//�����ڵ� (���� 3������ ���� ������ �ݺ�)
		/*else
		{
			switch (m_ePreState_Id)
			{

			case CIceWolfState::STATE_ID::START_BATTLE:
				return new CAttackNormalState(m_pOwner);
				break;

			case CIceWolfState::STATE_ID::STATE_NORMAL_ATK:
				return new CBattle_BackStepState(m_pOwner);

			case CIceWolfState::STATE_ID::STATE_BACKSTEP:
				return new CAttackBiteState(m_pOwner);
				break;

			case CIceWolfState::STATE_ID::STATE_BITE:
				return new CBattle_SomerSaultState(m_pOwner);

			case CIceWolfState::STATE_ID::STATE_ELEMENTAL_CHARGE:
				return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BATTLEING);

			case CIceWolfState::STATE_ID::STATE_IDLE:
				return new CAttackNormalState(m_pOwner);

			default:
				break;
			}
		}

	}*/
	return nullptr;
}

void CBattle_RunState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);

}

void CBattle_RunState::Exit()
{
	
	

}


