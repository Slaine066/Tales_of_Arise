#include "stdafx.h"
#include "..\Public\IceWolfBattle_Damage_LargeB_State.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfJumpState.h"

using namespace IceWolf;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(class CIce_Wolf* pIceWolf, STATE_ID StateId, HITTYPE eType, _vector vCauserPos, _float fMoveLength)
{
	m_pOwner = pIceWolf;
	m_fTimeDeltaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_eHitType = eType;
	m_eStateId = StateId;
	m_vCauserPos = vCauserPos;
	m_fMoveLength = fMoveLength;
}

CIceWolfState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	switch (m_eStateId)
	{
	case Client::CIceWolfState::STATE_DEAD:
		if (!m_bThirdHit)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	case Client::CIceWolfState::STATE_BE_DAMAGED:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		Move(fTimeDelta);
		break;
	case Client::CIceWolfState::STATE_DOWN:
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		break;
	}
	}

	m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CIceWolfState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fCntChanceTime)
		m_iRand = rand() % 3;

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CIceWolfState::STATE_DEAD:
			m_bDeadAnimFinish = true;
			if (m_bDeadAnimFinish)
			{
				m_pOwner->Set_GlowUp();
				CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
				m_pAtkColliderCom = nullptr;

				pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
			}
			break;
		case Client::CIceWolfState::STATE_BE_DAMAGED:
			m_pOwner->Set_Done_HitAnimState();

			if (m_pOwner->Get_IsFly())
				return new CIceWolfJumpState(m_pOwner, 1.f);

			else
			{
				switch (m_iRand)
				{
				case 0:
				case 1:
					return new CBattle_RunState(m_pOwner, STATE_ID::STATE_END);
					break;
				case 2:
					return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BE_DAMAGED);
					break;
				}
			}
			break;
		case Client::CIceWolfState::STATE_DOWN:
			return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_ARISE);
		}
	}

	return nullptr;
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

void CBattle_Damage_LargeB_State::Enter()
{
	m_iRand = rand() % 9;

	switch (m_eStateId)
	{
	case Client::CIceWolfState::STATE_DEAD:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DEAD);

		if (!m_bDeadSound && !m_bThirdHit)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Dead.wav"), SOUND_VOICE, 0.4f);
			m_bDeadSound = true;
			break;
		}
		break;

	case Client::CIceWolfState::STATE_BE_DAMAGED:
		if (m_pOwner->Get_IsFly())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DAMAGE_AIR_SMALL_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DAMAGE_SMALL_B);

		m_pOwner->SetOff_BedamagedCount();
		m_pOwner->Set_BedamageCount_Delay();
		if (!m_bAnimFinish)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Hit.wav"), SOUND_VOICE, 0.4f);
			m_bAnimFinish = true;
			break;
		}
		break;

	case Client::CIceWolfState::STATE_DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DOWN_F);
		m_pOwner->Set_OnGoingDown();
		break;

	}

}

void CBattle_Damage_LargeB_State::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	if (m_eStateId == Client::CIceWolfState::STATE_BE_DAMAGED)
		m_pOwner->SetOff_BedamageCount_Delay();

	Safe_Release(m_pAtkColliderCom);
}
