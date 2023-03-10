#include "stdafx.h"
#include "..\Public\AI_LAW_AIRSKILLF.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "UI_Skillmessage.h"
#include "ParticleSystem.h"
#include "AI_JumpState.h"
#include "Law.h"


using namespace AIPlayer;

CAI_LAW_AIRSKILLF::CAI_LAW_AIRSKILLF(CPlayer* pPlayer, STATE_ID eStateType)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	/*if (nullptr == pTarget)
	{
	m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
	(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	m_iCurrentAnimIndex = skillindex;
	m_fTime = ftime;*/
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_LAW_AIRSKILLF::Tick(_float fTimeDelta)
{
	Update_Skill();

	if (CheckTarget() == false)
		return nullptr;

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

						m_ColliderMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
							XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

						m_ColliderMatrix.r[0] = XMVector4Normalize(m_ColliderMatrix.r[0]);
						m_ColliderMatrix.r[1] = XMVector4Normalize(m_ColliderMatrix.r[1]);
						m_ColliderMatrix.r[2] = XMVector4Normalize(m_ColliderMatrix.r[2]);
					}

					if (!m_bSkill_F_Sound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_F_SkillHitSound.wav"), SOUND_EFFECT, 0.2f);
						m_bSkill_F_Sound = true;
					}
				}
				//if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					//return EventInput();
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!strcmp(pEvent.szName, "EnhaBakusaiken_2"))
					{
						if (!m_bEnhaBakusaiken_2)
						{
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

							CEffect::PlayEffectAtLocation(TEXT("Enha_Bakusaiken_Ring.dat"), mWorldMatrix);
							vector<CEffect*> pFloor = CEffect::PlayEffectAtLocation(TEXT("Enha_Bakusaiken_Floor.dat"), mWorldMatrix);

							_vector vPosition = pFloor[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							mWorldMatrix.r[3] = vPosition;
							CEffect::PlayEffectAtLocation(TEXT("Enha_Bakusaiken_Floor_Particles.dat"), mWorldMatrix);
							
							m_bEnhaBakusaiken_2 = true;
						}
					}
				}
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr != m_pLandCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLandCollider);
						m_pLandCollider = nullptr;
					}
				}
			}
		}

		if (nullptr != m_pLandCollider)
			m_pLandCollider->Update(m_ColliderMatrix);
	}
	else
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!strcmp(pEvent.szName, "EnhaBakusaiken_1"))
					{
						if (!m_bEnhaBakusaiken_1)
						{
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							m_EnhaBakusaiken_1 = CEffect::PlayEffectAtLocation(TEXT("Enha_Bakusaiken_1.dat"), mWorldMatrix);
							m_bEnhaBakusaiken_1 = true;
						}
					}
				}
			}
		}
	}

	return nullptr;
}


CAIState * CAI_LAW_AIRSKILLF::LateTick(_float fTimeDelta)
{
	Remove_Skill();

	if ((STATETYPE_END == m_eStateType) && (nullptr != m_pLandCollider))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
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
			return new CAICheckState(m_pOwner, STATE_ATTACK);
			break;
		}
	}

	return nullptr;
}

void CAI_LAW_AIRSKILLF::Enter()
{
	Reset_Skill();

	m_HitLagDesc.bHitLag = false;
	m_HitLagDesc.bLockOnChange = false;
	m_HitLagDesc.bShaking = false;

	m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);

	m_eStateType = STATETYPE_START;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_START);

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_F_SkillVoice.wav"), SOUND_VOICE, 0.25f);
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_F_SkillSoundBegin.wav"), SOUND_EFFECT, 0.251f);
	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_YUMPAEFUCKSAY);

}

void CAI_LAW_AIRSKILLF::Exit()
{
	__super::Exit();

	if (m_pOwner->Get_IsFly())
		m_pOwner->Off_IsFly();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	
	Safe_Release(m_pLandCollider);
}

void CAI_LAW_AIRSKILLF::Update_Skill(void)
{
	for (auto& pEffect : m_EnhaBakusaiken_1)
	{
		if (!pEffect || wcscmp(pEffect->Get_PrototypeId(), TEXT("Akizame")))
			continue;

		_float4 vPlayerPosition;
		XMStoreFloat4(&vPlayerPosition, m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
		_float4 vPlayerLook;
		XMStoreFloat4(&vPlayerLook, m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK));

		_float4 vEffectPosition;
		vEffectPosition.y = vPlayerPosition.y;

		XMStoreFloat4(&vEffectPosition, XMLoadFloat4(&vPlayerPosition) + XMLoadFloat4(&vPlayerLook));

		pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, XMLoadFloat4(&vEffectPosition));
	}
}

void CAI_LAW_AIRSKILLF::Remove_Skill(void)
{
	for (auto& pEffect : m_EnhaBakusaiken_1)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}
}

void CAI_LAW_AIRSKILLF::Reset_Skill(void)
{
	m_bEnhaBakusaiken_1 = false;
	m_bEnhaBakusaiken_2 = false;
}


CCollider * CAI_LAW_AIRSKILLF::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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