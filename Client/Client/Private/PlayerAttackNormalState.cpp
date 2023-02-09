#include "stdafx.h"

#include "PlayerAttackNormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "PlayerSkillState.h"
#include "PlayerJumpState.h"
#include "BattleManager.h"
#include "SionSkills.h"

using namespace Player;

CAttackNormalState::CAttackNormalState(CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	 
	m_fStartHeight = fStartHeight;
	m_fTime = fTime;

	if (m_fStartHeight != 0.f)
		m_pOwner->On_IsFly();
}

CPlayerState * CAttackNormalState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						//EventInput();

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();

					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 20.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						BulletDesc.vTargetPosition = (CBattleManager::Get_Instance()->Get_LackonMonster())->Get_TransformState(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
					}
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						//EventInput();

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{

					}
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{

					}
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				}
			}
		}
		else
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{

			}
		}
	}
	
	return nullptr;
}

CPlayerState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			if (!m_bIsFly)
				return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK4, m_fStartHeight, m_fTime);
			break;
		}
	}

	if (0 != m_iSkillEvent)
	{
		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_iSkillEvent)
			{
			case 1:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
				break;
			case 2:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
				break;
			case 3:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
				break;
			}
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

CPlayerState * CAttackNormalState::EventInput(void)
{
	return nullptr;
}

void CAttackNormalState::Enter()
{
	__super::Enter();

	if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
	{
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_2);
				break;
			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack1.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack2.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_8);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack3.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_6);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack4.wav"), SOUND_EFFECT, 1.0f);
				break;
			}
		}
	}
	else if (CPlayer::SION == m_pOwner->Get_PlayerID())
	{
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_2);
				break;
			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_3);
				break;
			}
		}
	}

	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	
	RELEASE_INSTANCE(CBattleManager);
}

void CAttackNormalState::Exit()
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}