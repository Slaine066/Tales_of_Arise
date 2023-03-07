#include "stdafx.h"
#include "..\Public\LawAirRSkillState.h"

#include "Monster.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

#include "LawAttackState.h"
#include "LawSkillState.h"

using namespace Player;

CLawAirRSkillState::CLawAirRSkillState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CLawAirRSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CLawAirRSkillState::Tick(_float fTimeDelta)
{
	Update_Skill();

	if ((STATETYPE_END == m_eStateType) && (nullptr != m_pTarget))
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

				//Sound

						if (!m_bSkill_R_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_R_SkillSound.wav"), SOUND_EFFECT, 0.5f);
							m_bSkill_R_Sound = true;
						}
					

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_1"))
					{
						if (!m_bSankamousyuukyaku_1)
						{
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							m_Sankamousyuukyaku_1 = CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_1.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_1 = true;
						}
					}
				}
				break;
			case Client::STATETYPE_END:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (!strcmp(pEvent.szName, "RH"))
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "LF"))
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "LH"))
					{
						if (nullptr == m_pLeftHandCollider)
							m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_Punch_1"))
					{
						if (!m_bSankamousyuukyaku_Punch_1)
						{
							m_Sankamousyuukyaku_Punch_1 = CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_Punch_1.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_Punch_1 = true;

							if (m_Sankamousyuukyaku_Punch_1[1])
								m_vPunchPosition_1 = m_Sankamousyuukyaku_Punch_1[1]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						}
					}
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_Particles_1"))
					{
						if (!m_bSankamousyuukyaku_Particles_1)
						{
							mWorldMatrix.r[3] = m_vPunchPosition_1;

							CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_2.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_Particles_1 = true;
						}
					}
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_Punch_2"))
					{
						if (!m_bSankamousyuukyaku_Punch_2)
						{
							m_Sankamousyuukyaku_Punch_2 = CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_Punch_2.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_Punch_2 = true;

							if (m_Sankamousyuukyaku_Punch_2[1])
								m_vPunchPosition_2 = m_Sankamousyuukyaku_Punch_2[1]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						}
					}
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_Particles_2"))
					{
						if (!m_bSankamousyuukyaku_Particles_2)
						{
							mWorldMatrix.r[3] = m_vPunchPosition_2;

							CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_2.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_Particles_2 = true;
						}
					}
					if (!strcmp(pEvent.szName, "Sanka_Mousyuukyaku_3"))
					{
						if (!m_bSankamousyuukyaku_2)
						{
							CEffect::PlayEffectAtLocation(TEXT("Sanka_Moushuukyoku_3.dat"), mWorldMatrix);
							m_bSankamousyuukyaku_2 = true;
						}
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
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
						m_pRightHandCollider = nullptr;
					}
				}
				else if (!strcmp(pEvent.szName, "LF"))
				{
					if (nullptr != m_pLeftFootCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
						m_pLeftFootCollider = nullptr;
					}
				}
				else if (!strcmp(pEvent.szName, "LH"))
				{
					if (nullptr != m_pLeftHandCollider)
					{
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

CPlayerState * CLawAirRSkillState::LateTick(_float fTimeDelta)
{
	Remove_Skill();

	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pRightHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
		}
	}
	
	return nullptr;
}

CPlayerState * CLawAirRSkillState::EventInput(void)
{
	if (GetKeyState(VK_LBUTTON) < 0)
		return new CLawAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
	{
		if (GetKeyState('E') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
		else if (GetKeyState('R') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
		else if (GetKeyState('F') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_BACKWARD_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S))
		return new CRunState(m_pOwner, DIR_BACKWARD);
	else if (pGameInstance->Key_Pressing(DIK_W))
		return new CRunState(m_pOwner, DIR_STRAIGHT);

	return nullptr;
}

void CLawAirRSkillState::Enter(void)
{
	__super::Enter();

	Reset_Skill();

	m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);

	m_eStateType = STATETYPE_START;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_SANKAMOUSYUUKYAKU_START);

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	m_pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != m_pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_R_SkillVoice.wav"), SOUND_EFFECT, 0.7f);
}

void CLawAirRSkillState::Exit(void)
{
	__super::Exit();

	
	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightHandCollider);
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CLawAirRSkillState::Update_Skill(void)
{
	for (auto& pEffect : m_Sankamousyuukyaku_1)
	{
		if (!pEffect || wcscmp(pEffect->Get_PrototypeId(), TEXT("Akizame")))
			continue;
		
		_float4 vPlayerPosition;
		XMStoreFloat4(&vPlayerPosition, m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
		_float4 vPlayerLook;
		XMStoreFloat4(&vPlayerLook, m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK));
		
		_float4 vEffectPosition;
		vEffectPosition.y = vPlayerPosition.y + 3.f;
		
		XMStoreFloat4(&vEffectPosition, XMLoadFloat4(&vPlayerPosition) + XMLoadFloat4(&vPlayerLook) * 4);
		
		pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, XMLoadFloat4(&vEffectPosition));
	}
}

void CLawAirRSkillState::Remove_Skill(void)
{
	for (auto& pEffect : m_Sankamousyuukyaku_1)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}
}

void CLawAirRSkillState::Reset_Skill(void)
{
	m_bSankamousyuukyaku_1 = false;
	m_bSankamousyuukyaku_2 = false;
	m_bSankamousyuukyaku_Punch_1 = false;
	m_bSankamousyuukyaku_Punch_2 = false;
	m_bSankamousyuukyaku_Particles_1 = false;
	m_bSankamousyuukyaku_Particles_2 = false;
}	

CCollider * CLawAirRSkillState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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
