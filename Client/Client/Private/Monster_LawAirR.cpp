#include "stdafx.h"
#include "..\Public\Monster_LawAirR.h"
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


using namespace MonsterLaw;

CMonster_LawAirR::CMonster_LawAirR(CMonsterLaw* pPlayer)//, _float fStartHeight, _float fTime)
{
	//m_eStateId = eStateType;
	m_pOwner = pPlayer;

}

CMonsterLawState * CMonster_LawAirR::Tick(_float fTimeDelta)
{
	
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iPhase);

	if ((STATETYPE_END == m_eStateType))
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	if (STATETYPE_MAIN == m_eStateType)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 3.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (STATETYPE_MAIN == m_eStateType)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.1f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	/* Animation Events */
	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			switch (m_eStateType)
			{
			case Client::STATETYPE_START:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					m_bIsLoop = true;
				break;
			case Client::STATETYPE_MAIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr == m_pLeftFootCollider)
						m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);
				}
				break;
			case Client::STATETYPE_END:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (!strcmp(pEvent.szName, "RH"))
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);
					}
					else if (!strcmp(pEvent.szName, "LF"))
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);

					}
					else if (!strcmp(pEvent.szName, "LH"))
					{
						if (nullptr == m_pLeftHandCollider)
							m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
					}
				}
				break;
			}
		}
		else
		{
			switch (m_eStateType)
			{
			case Client::STATETYPE_MAIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr != m_pLeftFootCollider)
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
						m_pLeftFootCollider = nullptr;
					}
				}
				break;
			case Client::STATETYPE_END:
				if (!strcmp(pEvent.szName, "RH"))
				{
					if (nullptr != m_pRightHandCollider)
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
						m_pRightHandCollider = nullptr;
					}
				}
				else if (!strcmp(pEvent.szName, "LF"))
				{
					if (nullptr != m_pLeftFootCollider)
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
						m_pLeftFootCollider = nullptr;
					}
				}
				else if (!strcmp(pEvent.szName, "LH"))
				{
					if (nullptr != m_pLeftHandCollider)
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
						m_pLeftHandCollider = nullptr;
					}
				}
				break;
			}
		}
	}

	_matrix	WorldBoneMatrix = XMMatrixIdentity();

	if (nullptr != m_pLeftHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pRightHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pRightHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pLeftFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftFootCollider->Update(WorldBoneMatrix);
	}

	return nullptr;
}


CMonsterLawState * CMonster_LawAirR::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftHandCollider);
#endif
	}

	if (nullptr != m_pRightHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pRightHandCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}
#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightHandCollider);
#endif
	}

	if (nullptr != m_pLeftFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pLeftFootCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftFootCollider);
#endif
	}

	if (m_bIsLoop)
	{
		if (m_pOwner->Check_Navigation_Jump())
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_END);
			m_eStateType = STATETYPE_END;
		}
		else
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_LOOP);
			m_eStateType = STATETYPE_MAIN;
		}

		m_bIsLoop = false;
	}

	if ((STATETYPE_MAIN == m_eStateType) && m_pOwner->Check_Navigation_Jump())
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_END);
		m_eStateType = STATETYPE_END;
	}

	if (m_bIsAnimationFinished)
	{
		if (STATETYPE_MAIN == m_eStateType)
			m_pOwner->Get_Model()->Reset_Anim(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_LOOP);

		if (STATETYPE_END == m_eStateType)
		{
			m_pOwner->Off_IsFly();
			return new CMonster_LawIdleState(m_pOwner);
		}
	}

	return nullptr;
}

void CMonster_LawAirR::Enter()
{


	m_eStateType = STATETYPE_START;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_START);

//	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

//	m_pTarget = pBattleMgr->Get_LackonMonster();
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iPhase);

	if (nullptr != m_pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_Jump_R.wav"), SOUND_EFFECT, 1.0f);
	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_SANHWAMANGSUP);


}

void CMonster_LawAirR::Exit()
{
	//__super::Exit();

	
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);

	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightHandCollider);

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}


CCollider * CMonster_LawAirR::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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