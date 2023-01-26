#include "stdafx.h"
#include "..\Public\IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"
using namespace IceWolf;

CBattle_BackStepState::CBattle_BackStepState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_BackStepState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_BackStepState::Tick(_float fTimeDelta)
{
	Find_BattleTarget();
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	Find_BattleTarget();
	m_pOwner->Check_Navigation();

	m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_BackStepState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
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
			return new CBattle_SomerSaultState(m_pOwner);
			break;

		default:
			break;
		}
		
	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}



	return nullptr;
}

void CBattle_BackStepState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_STEP_BACK);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	
}

void CBattle_BackStepState::Exit()
{
	m_fRedayAttackTimer = 0.f;
	
}



