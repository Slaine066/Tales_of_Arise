#include "stdafx.h"
#include "HawkBattle_GrabEndState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_RunState.h"

using namespace Hawk;

CBattle_GrabEndState::CBattle_GrabEndState(CHawk* pHawk)
{
	
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CHawkState * CBattle_GrabEndState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CHawkState * CBattle_GrabEndState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);
		
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
			

	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner);
	

	else
	{
		/*_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());*/
		m_pOwner->Check_Navigation();

	}

	return nullptr;
}

CHawkState * CBattle_GrabEndState::LateTick(_float fTimeDelta)
{
	Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//1 . �ݶ��̴� ������Ʈ�� ���ش�
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	
	//2. �浹üũ
	m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());
	if (m_bCollision)
	{
		
	}
	
	return nullptr;
}

void CBattle_GrabEndState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_END);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_GrabEndState::Exit()
{


}