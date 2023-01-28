#include "stdafx.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_GrabEndState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_GrabState.h"

using namespace Hawk;

CBattle_GrabStartState::CBattle_GrabStartState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CHawkState * CBattle_GrabStartState::AI_Behaviour(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();
	


	return nullptr;
}

CHawkState * CBattle_GrabStartState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_bTargetSetting = true;
	}

	return nullptr;
}

CHawkState * CBattle_GrabStartState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;

	////1 . �ݶ��̴� ������Ʈ�� ���ش�
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());
	if (m_bCollision)
		m_iCollisionCount = 1;
	
	////2. �浹üũ

	if (m_bIsAnimationFinished)
	{
		return new CBattle_GrabState(m_pOwner, STATE_ID::STATE_GRAB_START);
	
	}

	else
	{
//		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
//		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

void CBattle_GrabStartState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_START);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	
}

void CBattle_GrabStartState::Exit()
{

	
}