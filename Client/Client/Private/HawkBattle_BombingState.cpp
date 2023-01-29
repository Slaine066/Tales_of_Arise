#include "stdafx.h"

#include "HawkBattle_BombingState.h"
#include "GameInstance.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_TornadeState.h"
#include "HawkBattle_ChargeState.h"
using namespace Hawk;

CBattle_BombingState::CBattle_BombingState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_BombingState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CBattle_BombingState::Tick(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_float fTranslationLength, fRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &fTranslationLength, &fRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((fTranslationLength * 0.01f), fRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CHawkState * CBattle_BombingState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	



	srand((_uint)time(NULL));
	m_iRand = rand() % 2;


	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_ChargeState(m_pOwner);
			break;

		case 1:
			//return new CBattle_RunState(m_pOwner);
			break;

		default:
			break;
		}
	}



	

	return nullptr;
}

void CBattle_BombingState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_BOMBING);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_BombingState::Exit()
{

}