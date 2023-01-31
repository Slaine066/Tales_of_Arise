#include "stdafx.h"

#include "BerserkerBattle_Damage_LargeB_State.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_WalkState.h"
using namespace Berserker;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(CBerserker* pBerserker, _bool bOnAngry)
{
	m_pOwner = pBerserker;
	m_bAngry = bOnAngry;
	if (m_bAngry)
	{
		m_eEmotion = EMO_ANGRY;
	}
}

CBerserkerState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{

	//if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}


	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	CMonster::STATS pBerserker_Stats;

	memcpy(&pBerserker_Stats, &m_pOwner->Get_Stats(), sizeof(CMonster::STATS));
	
	if(m_bIsAnimationFinished)
	{
		if (pBerserker_Stats.m_fCurrentHp <= 3 && false == m_bAngry)
		{
			m_bAngry = true;
			return new CBattle_HowLingState(m_pOwner);
		}

		else if (pBerserker_Stats.m_fCurrentHp <= 2 && true == m_bAngry)
		{
			switch (m_eEmotion)
			{
			case EMO_CALM:
				return new CBattle_WalkState(m_pOwner);
				break;

			case EMO_ANGRY:
				return new CBattle_BackStepState(m_pOwner);
				break;

			default:
				break;
			}
		}
	}




	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_B);

	
}

void CBattle_Damage_LargeB_State::Exit()
{
	
}