#include "stdafx.h"

#include "BerserkerBattle_Damage_LargeB_State.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_WalkState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerJumpState.h"

using namespace Berserker;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(class CBerserker* pBerserker, _bool bAngry, _bool bBerserkerMode, _bool bEvadeChance, HITTYPE eType, _vector vCauserPos, _float fMoveLength, STATE_ID eStateId)
{
	m_pOwner = pBerserker;
	/*m_bAngry = bOnAngry;
	if (m_bAngry)
	{
		m_eEmotion = EMO_ANGRY;
	}*/
	m_bAngry = bAngry;
	m_bBerserkerMode = bBerserkerMode;
	m_bEvadeChance = bEvadeChance;
	m_eStateId = eStateId;
	m_eHitType = eType;
	m_vCauserPos = vCauserPos;
	m_fMoveLength = fMoveLength;
}

CBerserkerState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	if (m_eStateId == Client::CBerserkerState::SMASH_DAMAGE)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	//if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}
	if (m_eStateId == Client::CBerserkerState::STATE_DOWN)
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			m_pOwner->Check_Navigation();
		}
		m_pOwner->Check_Navigation_Jump();
	}
	else if (m_eStateId != Client::CBerserkerState::SMASH_DAMAGE)
	{
		Move(fTimeDelta);
		m_pOwner->Check_Navigation_Jump();
	}
		


	

	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	CMonster::STATS pBerserker_Stats;

	memcpy(&pBerserker_Stats, &m_pOwner->Get_Stats(), sizeof(CMonster::STATS));
	
	if (m_bAngry)
		m_pOwner->Set_AngryOn();

	if(m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CBerserkerState::STATE_TAKE_DAMAGE:
			if (m_pOwner->Get_IsFly())
				return new CBerserkerJumpState(m_pOwner, 1.f);

			if (m_bAngry == true && m_bBerserkerMode == false)
				return new CBattle_HowLingState(m_pOwner);

			if (m_bAngry == false)
				return new CBattle_WalkState(m_pOwner);

			if (m_bBerserkerMode == true && m_bEvadeChance == false)
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_HIT);

			else if (m_bBerserkerMode == true && m_bEvadeChance == true)
				return new CBattle_BackStepState(m_pOwner);
			break;

		case Client::CBerserkerState::STATE_DOWN:
			return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_ARISE);
		case Client::CBerserkerState::SMASH_DAMAGE:
		{
			if(CCameraManager::Get_Instance()->Get_CamState() ==CCameraManager::CAM_DYNAMIC)
				return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_ARISE);
		}	
		}
	}


	//if (m_bIsAnimationFinished)
	//{
	//	if (pBerserker_Stats.m_fCurrentHp <= 100 && false == m_bAngry)
	//	{
	//		m_bAngry = true;
	//		return new CBattle_HowLingState(m_pOwner);
	//	}

	//	else if (/*pBerserker_Stats.m_fCurrentHp <= 2 && */true == m_bAngry)
	//	{
	//		switch (m_eEmotion)
	//		{
	//		case EMO_CALM:
	//			return new CBattle_WalkState(m_pOwner);
	//			break;

	//		case EMO_ANGRY:
	//			return new CBattle_BackStepState(m_pOwner);
	//			break;

	//		default:
	//			break;
	//		}
	//	}
	//}

	

	
	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	

	switch (m_eStateId)
	{
	case Client::CBerserkerState::STATE_TAKE_DAMAGE:
		if (m_pOwner->Get_IsFly())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::BLOW_UP_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_B);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Hit.wav"), SOUND_VOICE, 0.4f);
		m_pOwner->SetOff_BedamagedCount();
		m_pOwner->Set_BedamageCount_Delay();
		break;

	case Client::CBerserkerState::STATE_DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DOWN_F);
		m_pOwner->Set_OnGoingDown();
		break;
	case Client::CBerserkerState::SMASH_DAMAGE:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_B);
		break;
	}

	

	/*if (rand() % 6 == 0)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_L);
	else if (rand() % 6 == 1)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_R);
	else if(rand() % 6 == 2)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_SMALL_L);

	else if (rand() % 6 == 3)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_SMALL_R);

	else if (rand() % 6 == 4)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_SMALL_B);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_B);

*/

	

}

void CBattle_Damage_LargeB_State::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	//if (m_eStateId == Client::CBerserkerState::STATE_TAKE_DAMAGE)
	//	m_pOwner->SetOff_BedamageCount_Delay();
}

void CBattle_Damage_LargeB_State::Move(_float fTimeDelta)
{
	_vector vOwnerPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vDir = XMVector4Normalize(vOwnerPos - XMVectorSetY(m_vCauserPos, XMVectorGetY(vOwnerPos)));

	if (HIT_NORMAL != m_eHitType)
	{
		m_fTime += fTimeDelta * 2.5f;

		m_pOwner->Get_Transform()->Jump(m_fTime, 2.3f, 3.f, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
	}

	m_pOwner->Get_Transform()->Go_PosDir(fTimeDelta * m_fMoveLength, vDir, m_pOwner->Get_Navigation());
}