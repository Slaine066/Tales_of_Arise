#include "stdafx.h"

#include "BerserkerBattle_Shock_WaveState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_TurnState.h"
#include "BerserkerBattle_WalkState.h"

using namespace Berserker;

CBattle_Shock_WaveState::CBattle_Shock_WaveState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 3000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_Shock_WaveState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.4f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), XMConvertToRadians(0.f), m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				if (!m_bAnimFinish)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_ShockWave.wav"), SOUND_VOICE, 0.4f);
					m_bAnimFinish = true;
				}
			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				_matrix R_matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				R_matWorld.r[0] = XMVector4Normalize(R_matWorld.r[0]);
				R_matWorld.r[1] = XMVector4Normalize(R_matWorld.r[1]);
				R_matWorld.r[2] = XMVector4Normalize(R_matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);

					pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
				}

				else if (nullptr != m_pAtkColliderCom)
					m_pAtkColliderCom->Update(matWorld);

				if (nullptr == m_p2th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc2th;

					ColliderDesc2th.vScale = _float3(5.f, 5.f, 5.f);
					ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

					m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
					m_p2th_AtkColliderCom->Update(R_matWorld);

					pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
				}

				else if (nullptr != m_p2th_AtkColliderCom)
					m_p2th_AtkColliderCom->Update(R_matWorld);

				RELEASE_INSTANCE(CCollision_Manager);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_AtkColliderCom);

			m_pAtkColliderCom = nullptr;
			m_p2th_AtkColliderCom = nullptr;

			pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);

			RELEASE_INSTANCE(CCollision_Manager);
		}
	}


	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;
	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;
	
	m_bAngry = m_pOwner->Get_IsAngry();

	if (m_bIsAnimationFinished)
	{
		if (m_bAngry)
		{
			switch (m_iRand)
			{
			case 0:
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);
				break;

			case 1:
				return new CBattle_WalkState(m_pOwner);
				break;
			}
		}
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);

	if (nullptr != m_p2th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p2th_AtkColliderCom);
#endif // _DEBUG

	return nullptr;
}

void CBattle_Shock_WaveState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_SHOCK_WAVE);

	m_pOwner->SetOff_BedamageCount_Delay();
}

void CBattle_Shock_WaveState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);
}