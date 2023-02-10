#include "stdafx.h"

#include "AI_BoostAttackState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"

using namespace AIPlayer;

CAI_BoostAttack::CAI_BoostAttack(CPlayer* pPlayer, CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_BoostAttack::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (!m_bIsAnimationFinished)
	{
		m_fTime += fTimeDelta;

		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
					case CPlayer::ALPHEN:
					{
						if (!strcmp(pEvent.szName, "Alphen_Strike_1"))
						{
							if (!m_bAlphenStrike_1)
							{
								_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));

								CBullet::BULLETDESC BulletDesc;
								BulletDesc.eCollisionGroup = PLAYER;
								BulletDesc.fDeadTime = 3.f;
								BulletDesc.eBulletType = CAlphenSkills::STRIKE_1;
								BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook * 2.f;
								BulletDesc.pOwner = m_pOwner;
								BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AlphenSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;

								m_bAlphenStrike_1 = true;
							}
						}
						if (!strcmp(pEvent.szName, "Alphen_Strike_2"))
						{
							if (!m_bAlphenStrike_2)
							{
								_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));

								CBullet::BULLETDESC BulletDesc;
								BulletDesc.eCollisionGroup = PLAYER;
								BulletDesc.fDeadTime = 3.f;
								BulletDesc.eBulletType = CAlphenSkills::STRIKE_2;
								BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook * 2.f;
								BulletDesc.pOwner = m_pOwner;
								BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AlphenSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;

								m_bAlphenStrike_2 = true;
							}
						}
						break;
					}

					/* Add SION Logic after refactor. */
				}
			}
		}

		if (!m_bBullet && m_eCurrentPlayerID == CPlayer::SION && m_fTime > 2.f)
		{
			switch (m_eCurrentPlayerID)
			{
				case CPlayer::SION:
				{
					_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));

					CBullet::BULLETDESC BulletDesc;
					BulletDesc.eCollisionGroup = PLAYER;
					BulletDesc.fDeadTime = 5.f;
					BulletDesc.eBulletType = CSionSkills::BOOST;
					BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook*2.f;
					BulletDesc.pOwner = m_pOwner;
					BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
						return nullptr;

					m_bBullet = true;

					break;
				}	
			}
			
		}

		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAI_BoostAttack::LateTick(_float fTimeDelta)
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (m_bIsAnimationFinished)
		return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);

	return nullptr;
}

void CAI_BoostAttack::Enter()
{
	//m_eStateId = STATE_ID::STATE_DEAD;
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE;

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Sion_Boost.dat"), mWorldMatrix);
	}
	//m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
	pCamera->Set_Target(m_pOwner);

}

void CAI_BoostAttack::Exit()
{
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);


	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	__super::Exit();
}