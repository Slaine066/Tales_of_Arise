#include "stdafx.h"
#include "..\Public\IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_RunState.h"
using namespace IceWolf;

CAttack_Elemental_Charge::CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType)
{
	m_eStateId_Charge = eStateType;
	m_pOwner = pIceWolf;
	
}

CIceWolfState * CAttack_Elemental_Charge::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::Tick(_float fTimeDelta)
{


	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotation);

		//m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::LateTick(_float fTimeDelta)
{



	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_ELEMENTAL_CHARGE);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	
	return nullptr;
}



void CAttack_Elemental_Charge::Enter()
{
	switch (m_eStateId_Charge)
	{
	case STATE_CHARGE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_START);
		break;

	case STATE_CHARGE_LOOP:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_LOOP);
		break;

	case STATE_CHARGE_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_END);
		break;
	}


	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAttack_Elemental_Charge::Exit()
{
	m_pOwner->Get_Model()->Reset();
}



