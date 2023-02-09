#include "stdafx.h"
#include "..\Public\Player_SionSkillAttack.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "BattleManager.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Player_SionNormalAttack_State.h"


using namespace Player;

CPlayer_SionSkillAttack::CPlayer_SionSkillAttack(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CPlayer_SionSkillAttack::HandleInput(void)
{
	return nullptr;
}


CPlayerState * CPlayer_SionSkillAttack::Tick(_float fTimeDelta)
{
	/*if (CAlphen::ANIM::ANIM_: == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

		m_pOwner->Check_Navigation();
	}
	else
	{*/
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
		if (CSion::ANIM::BTL_ATTACK_TRESVENTOS)
		{
			if (!m_bIsAnimationFinished)
			{
				_vector vecTranslation;
				_float fRotationRadian;

				m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

				if (CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
					m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());
				else
					m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
			}
			else
				m_pOwner->Check_Navigation_Jump();
		}
		else
		{
			if (!m_bIsAnimationFinished)
			{
				_vector vecTranslation;
				_float fRotationRadian;

				m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

				if (CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
					m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());
				else
					m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
			}
			else
				m_pOwner->Check_Navigation();
		}
		
//	}

	if (!m_pBlastEffect.empty())
	{
		for (auto& iter : m_pBlastEffect)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;

			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	if (m_bIsFly)
		m_fTime += fTimeDelta;

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK_E:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{	
					if ((m_fEventStart != pEvent.fStartTime))
					{
						if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CSion::ANIM::BTL_MAGNARAY))
						{
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.fVelocity = 1.f;
							BulletDesc.eBulletType = CSionSkills::MAGNA_RAY;
							BulletDesc.iDamage = 150;
							BulletDesc.fDeadTime = 2.f;
							BulletDesc.vTargetDir = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)*4.f);
							BulletDesc.pOwner = m_pOwner;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							m_fEventStart = pEvent.fStartTime;
						}

						else if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CSion::ANIM::BTL_ATTACK_TRESVENTOS))
						{
				
							_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));
							_vector vRight = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_RIGHT));
							_vector vPostion = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.eBulletType = CSionSkills::TRESVENTOS;
							BulletDesc.vInitPositon = XMVectorSetY(vPostion, XMVectorGetY(vPostion) + 3.f) + vLook*2.f;
							if (m_iCount == 0)
								BulletDesc.vInitPositon -= vRight*2.f;
							if (m_iCount == 1)
								BulletDesc.vInitPositon += vRight*2.f;

							BulletDesc.pOwner = m_pOwner;
							BulletDesc.fVelocity = 5.f;
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
							BulletDesc.vTargetDir = vLook;
							BulletDesc.pTarget = pTarget;
							
							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							m_fEventStart = pEvent.fStartTime;
							m_iCount++;
						}
					}
				}
				
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (GetKeyState('E') < 0)
						m_iSkillEvent = 1;

					if (GetKeyState('R') < 0)
						m_iSkillEvent = 2;

					if (GetKeyState('F') < 0)
						m_iSkillEvent = 3;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK_R:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !m_bBulletMade)
				{
					if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE))
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 1.f;
						BulletDesc.eBulletType = CSionSkills::GRAVITY;
						BulletDesc.iDamage = 200;
						BulletDesc.fDeadTime = 10.f;
						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							//BulletDesc.vTargetPosition
							BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
						}
						else
							BulletDesc.vTargetDir = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)*2.f);
						BulletDesc.pOwner = m_pOwner;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_bBulletMade = true;
					}
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (GetKeyState('E') < 0)
						m_iSkillEvent = 1;

					if (GetKeyState('R') < 0)
						m_iSkillEvent = 2;

					if (GetKeyState('F') < 0)
						m_iSkillEvent = 3;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK_F:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					
				}
			
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (GetKeyState('E') < 0)
						m_iSkillEvent = 1;

					if (GetKeyState('R') < 0)
						m_iSkillEvent = 2;

					if (GetKeyState('F') < 0)
						m_iSkillEvent = 3;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				break;

			case Client::CPlayerState::STATE_SKILL_ATTACK4:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 2.f;
						BulletDesc.eBulletType = CSionSkills::GLACIA;
						BulletDesc.iDamage = 200;
						BulletDesc.fDeadTime = 10.f;
				

					   BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
					   BulletDesc.vTargetPosition.m128_f32[2] += 6.f;


						BulletDesc.vInitPositon = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.vInitPositon.m128_f32[2] += 6.f;
						
						// +vOffset;
						BulletDesc.vInitPositon.m128_f32[1] = 15.f;
						BulletDesc.pOwner = m_pOwner;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] += 6.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] -= 12.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							//vOffset = XMVectorSet(0.f, 0.f, 3.f, 0.f);
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);// -vOffset;
							BulletDesc.vTargetPosition.m128_f32[2] -= 12.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
					}
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (GetKeyState('E') < 0)
						m_iSkillEvent = 1;

					if (GetKeyState('R') < 0)
						m_iSkillEvent = 2;

					if (GetKeyState('F') < 0)
						m_iSkillEvent = 3;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				break;

			case Client::CPlayerState::STATE_SKILL_ATTACK5:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

						if ((m_fEventStart != pEvent.fStartTime))
						{
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.eBulletType = CSionSkills::AQUA_LUINA;
							if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

							
						

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							
							m_fEventStart = pEvent.fStartTime;
						
					}
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (GetKeyState('E') < 0)
						m_iSkillEvent = 1;

					if (GetKeyState('R') < 0)
						m_iSkillEvent = 2;

					if (GetKeyState('F') < 0)
						m_iSkillEvent = 3;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				break;
			}
		}

	}



	return nullptr;
}

CPlayerState * CPlayer_SionSkillAttack::LateTick(_float fTimeDelta)
{
	

	if (m_bIsStateEvent)
		return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if ((0 != m_iSkillEvent) && (floor(m_pOwner->Get_Info().fCurrentMp) > 0))
	{
		switch (m_iSkillEvent)
		{
		case 1:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_E);
			break;
		case 2:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_R);
			break;
		case 3:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_F);
			break;
		case 4:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK4);
			break;
		case 5:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK5);
			break;
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner);

	}

	return nullptr;
}

void CPlayer_SionSkillAttack::Enter(void)
{
	__super::Enter();


	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_TRESVENTOS);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
		//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			//				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
		{
			/* Make Effect */
			_vector vOffset = { 0.f,3.f,0.f,0.f };
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK));
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation + vOffset + vLook*2.f;
			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("MagnaRayStart.dat"), mWorldMatrix);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MAGNARAY);//���׳�
			break;
		}	
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
		{
			/* Make Effect */
			_vector vOffset = { 0.f,3.f,0.f,0.f };
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION); 
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation + vOffset;
			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GravitasField.dat"), mWorldMatrix);


			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE); // �߷�
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_BRAVE); // ���׿�
			break;

		case Client::CPlayerState::STATE_SKILL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_THUNDER_BOLT); //����������
			break;

		case Client::CPlayerState::STATE_SKILL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET); // �񰡿����
			break;
		}
	}


	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
	{
		_float4 fTargetPos;
		XMStoreFloat4(&fTargetPos, pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

		fTargetPos.y = m_pOwner->Get_Transform()->Get_World4x4().m[3][1];

		_vector		vLook = XMLoadFloat4(&fTargetPos) - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vRight = XMVector3Cross(vAxisY, vLook);

		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_RIGHT));
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_LOOK));
	}

	RELEASE_INSTANCE(CBattleManager);
}

void CPlayer_SionSkillAttack::Exit(void)
{
	__super::Exit();
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CPlayer_SionSkillAttack::CallbackFunction(_uint iIndex)
{
	m_HousyutigakuzinStart[iIndex] = nullptr;
}