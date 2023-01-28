#include "stdafx.h"
#include "HawkBattle_TornadeState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_DashState.h"

using namespace Hawk;

CBattle_TornadeState::CBattle_TornadeState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	
}

CHawkState * CBattle_TornadeState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;


}

CHawkState * CBattle_TornadeState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_fTarget_Distance = Find_BattleTarget();
	m_pOwner->Check_Navigation(); // ����
	m_bIsAnimationFinished =  m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()),"ABone");


	return nullptr;
}

CHawkState * CBattle_TornadeState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
			break;
		case 1:
			return new CBattle_DashState(m_pOwner);
			break;

		default:
			break;
		}


	}
	else
	{
	//	_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	/*m_iRand = rand() % 1;

	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	

		if (10.f <= m_fIdleAttackTimer)
		{
			return new CBattle_ChargeState(m_pOwner);
		}

		else
		{
			if (6.f > m_fTarget_Distance)
				return new CBattle_GrabStartState(m_pOwner);
			

			m_fIdleAttackTimer += fTimeDelta;
		}*/
		



	return nullptr;
}

void CBattle_TornadeState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_TORNADE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_TornadeState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}