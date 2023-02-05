#include "stdafx.h"

#include "HawkTurnR_State.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"
#include "HawkWalkState.h"

using namespace Hawk;

CTurnR_State::CTurnR_State(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;

	m_fTimeDeltaAcc = 0;
	m_fTurnR_Time = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
	
}

CHawkState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CTurnR_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CHawkState * CTurnR_State::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fTurnR_Time)
		m_iRand = rand() % 2;

	if (m_pTarget)
		return new CChaseState(m_pOwner);

	/*if (m_fTarget_Distance <= 10)
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Hawk_Chase.wav"), SOUND_VOICE, 0.4f);*/

	else if (m_bIsAnimationFinished)
	{
		//���� Ʈ���� �ڽ��� �浹��������
		CBaseObj* pTrigger = m_pOwner->Get_Trigger();

		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
			return new CWalkState(m_pOwner,true);
		else
		{
			switch (m_iRand)
			{
			case 0:
				return new CWalkState(m_pOwner);
			case 1:
				return new CIdleState(m_pOwner);
			default:
				break;
			}
		}


	}
		
	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::TURN_R);

	
}

void CTurnR_State::Exit()
{
}