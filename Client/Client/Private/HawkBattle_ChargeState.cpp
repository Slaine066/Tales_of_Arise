#include "stdafx.h"
#include "HawkBattle_ChargeState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_RunState.h"

using namespace Hawk;

CBattle_ChargeState::CBattle_ChargeState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();


}

CHawkState * CBattle_ChargeState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	return nullptr;
}

CHawkState * CBattle_ChargeState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	m_iRand = rand() % 2;

	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
		case 1:
			return new CBattle_ChargeState(m_pOwner);
		/*case 2:
			return new CBattle_IdleState(m_pOwner);*/
		}
	}

	else
	{
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CHawkState * CBattle_ChargeState::LateTick(_float fTimeDelta)
{
	Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//1 . �ݶ��̴� ������Ʈ�� ���ش�
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	//2. �浹üũ
	if (pCollider->Collision(m_pTarget->Get_Collider()))
	{

	}


	return nullptr;
}

void CBattle_ChargeState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_CHARGE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

}

void CBattle_ChargeState::Exit()
{
	
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}