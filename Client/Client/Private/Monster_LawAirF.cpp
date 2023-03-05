#include "stdafx.h"
#include "..\Public\Monster_LawAirF.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "UI_Skillmessage.h"
#include "ParticleSystem.h"
#include "Law.h"
#include "Monster_LawIdleState.h"
#include "Bullet.h"
#include "RinwellSkills.h"


using namespace MonsterLaw;

CMonster_LawAirF::CMonster_LawAirF(CMonsterLaw* pPlayer)//, _float fStartHeight, _float fTime)
{
	//m_eStateId = eStateType;
	m_pOwner = pPlayer;

}

CMonsterLawState * CMonster_LawAirF::Tick(_float fTimeDelta)
{

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iPhase);

	if (STATETYPE_MAIN == m_eStateType)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 0.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (STATETYPE_MAIN == m_eStateType)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.6f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	/* Animation Events */
	if (STATETYPE_MAIN != m_eStateType)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr == m_pLandCollider)
					{
						m_pLandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(10.f, 10.f, 10.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLandCollider, m_pOwner);
						m_ColliderMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
							XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

						m_ColliderMatrix.r[0] = XMVector4Normalize(m_ColliderMatrix.r[0]);
						m_ColliderMatrix.r[1] = XMVector4Normalize(m_ColliderMatrix.r[1]);
						m_ColliderMatrix.r[2] = XMVector4Normalize(m_ColliderMatrix.r[2]);

						if ((m_fEventStart != pEvent.fStartTime))
						{
							//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iPhase);
							//Bullet
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = MONSTER;
							BulletDesc.eBulletType = CRinwellSkills::THUNDER_FIELD;

							BulletDesc.fVelocity = 0.5f;
							BulletDesc.vInitPositon = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
							//BulletDesc.vTargetDir = { -0.3f, -1.f, -0.1f, 0.f };
							//BulletDesc.vTargetPosition = vTargetPosition;
							BulletDesc.fDeadTime = 5.5f;
							BulletDesc.pOwner = m_pOwner;
							for (_uint i = 0; i < 12; ++i)
							{
								m_pOwner->Get_Transform()->Set_Rotation({ 0.f,0.f + i * 30.f , 0.f });
								BulletDesc.vTargetDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;
							}
							m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
							//	m_bBulletMake = true;
							//m_bCollideFinsh = true;
							m_fEventStart = pEvent.fStartTime;
						}
					}
				}
				//if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				//return EventInput();
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr != m_pLandCollider)
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLandCollider);
						m_pLandCollider = nullptr;
					}
				}
			}
		}

		if (nullptr != m_pLandCollider)
			m_pLandCollider->Update(m_ColliderMatrix);
	}

	return nullptr;
}


CMonsterLawState * CMonster_LawAirF::LateTick(_float fTimeDelta)
{
	if (/*(STATETYPE_END == m_eStateType) && */(nullptr != m_pLandCollider))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pLandCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLandCollider);
#endif
	}

	if ((STATETYPE_MAIN == m_eStateType) && m_pOwner->Check_Navigation_Jump())
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_END);
		m_eStateType = STATETYPE_END;
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateType)
		{
		case Client::STATETYPE_START:
			if (!m_pOwner->Check_Navigation_Jump())
			{
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_LOOP);
				m_eStateType = STATETYPE_MAIN;
			}
			else
			{
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_END);
				m_eStateType = STATETYPE_END;
			}
			break;
		case Client::STATETYPE_MAIN:
			m_pOwner->Get_Model()->Reset_Anim(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_LOOP);
			break;
		case Client::STATETYPE_END:
			return new CMonster_LawIdleState(m_pOwner);
			break;
		}
	}

	return nullptr;
}

void CMonster_LawAirF::Enter()
{
	m_fEventStart = -1.f;

	m_eStateType = STATETYPE_START;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_START);


	

	if (nullptr == m_pTarget)
	{
		m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iPhase);
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_Jump_F.wav"), SOUND_EFFECT, 1.0f);
	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_YUMPAEFUCKSAY);


}

void CMonster_LawAirF::Exit()
{
	//__super::Exit();

	if (m_pOwner->Get_IsFly())
		m_pOwner->Off_IsFly();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);

	Safe_Release(m_pLandCollider);
}


CCollider * CMonster_LawAirF::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
{
	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.vScale = vScale;
	ColliderDesc.vRotation = vRotation;
	ColliderDesc.vPosition = vPosition;

	switch (eType)
	{
	case Engine::CCollider::TYPE_AABB:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_OBB:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
		break;
	}

	return nullptr;
}