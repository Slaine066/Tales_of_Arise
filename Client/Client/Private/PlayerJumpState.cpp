#include "stdafx.h"

#include "GameInstance.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"

#include "CloseChaseState.h"
#include "AlphenAttackState.h"
#include "AlphenSkillState.h"
#include "Player_RinwellNormalAttack_State.h"

#include "LawAttackState.h"
#include "LawSkillState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"
#include "Player_RinwellSkillAttack_State.h"

using namespace Player;

CJumpState::CJumpState(CPlayer* pPlayer, STATETYPE eType, JUMPTYPE eJumpType, _float fTime)
{
	m_pOwner = pPlayer;
	m_eStateType = eType;
	m_eJumpType = eJumpType;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_fTime = fTime;
}

CPlayerState * CJumpState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		if (STATETYPE_MAIN != m_eStateType)
		{
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::SION:
					return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::RINWELL:
					return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::LAW:
					return new CLawAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				}
			}

			if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_STRAIGHT_LEFT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_STRAIGHT_RIGHT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_BACKWARD_LEFT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_BACKWARD_RIGHT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_LEFT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_RIGHT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_BACKWARD, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_LSHIFT))
				return new CDodgeState(m_pOwner, DIR_STRAIGHT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT) && m_bIsFly)
				return new CDodgeState(m_pOwner, DIR_STRAIGHT, m_fTime);
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT) && !m_bIsFly)
				return new CDodgeState(m_pOwner, DIR_END, m_fTime);
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (pGameInstance->Key_Down(DIK_E))
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
				break;
			case CPlayer::SION:
				if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
				break;
			case CPlayer::RINWELL:
				if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4, m_fTime);
				break;
			case CPlayer::LAW:
				if (pGameInstance->Key_Down(DIK_E))
					return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CLawAirRSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CLawAirFSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			}
		}
	}

	if (JUMP_RUN != m_eJumpType)
	{
		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_STRAIGHT_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_STRAIGHT_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_BACKWARD_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_BACKWARD_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_S))
			m_eDirection = DIR_BACKWARD;
		else if (pGameInstance->Key_Pressing(DIK_W))
			m_eDirection = DIR_STRAIGHT;
		else
			m_eDirection = DIR_END;
	}

	return nullptr;
}

CPlayerState * CJumpState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());

		m_bIsJump = false;
	}

	if (CPlayer::ALPHEN == m_ePlayerID || CPlayer::LAW == m_ePlayerID)
	{
		if (STATETYPE_START == m_eStateType && JUMP_IDLE == m_eJumpType)
		{
			vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

			for (auto& pEvent : pEvents)
			{
				if (pEvent.isPlay)
				{
					switch (m_eStateType)
					{
					case STATETYPE_START:
						if (ANIMEVENT::EVENT_INPUT == pEvent.eType)
							m_bIsJump = true;
						break;
					}
				}
			}
		}
		else
			m_bIsJump = true;
	}
	
	if (STATETYPE_END == m_eStateType)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENT_STATE == pEvent.eType)
				{
					CPlayerState* pEvent = EventInput();
					if (nullptr != pEvent)
						return pEvent;
				}
			}
		}
	}
	else
		m_bIsJump = true;


	if (m_bIsJump)
		Move(fTimeDelta);

	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (STATETYPE_END == m_eStateType)
	{
		m_pOwner->Check_Navigation_Jump();

		
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (JUMP_RUN != m_eJumpType)
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}
				else
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_RunJumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}
				break;

			case CPlayer::SION:
				if (JUMP_RUN != m_eJumpType)
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}

				else
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_RunJumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}
				break;

			case CPlayer::RINWELL:
				if (JUMP_RUN != m_eJumpType)
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}

				else
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_RunJumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}
				break;

			case CPlayer::LAW:
				if (JUMP_RUN != m_eJumpType)
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}

				else
				{
					if (!m_bLandSound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_RunJumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_bLandSound = true;
					}
				}
				break;

			//default:
			//	if (!m_bLandSound)
			//	{
			//		CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
			//		m_bLandSound = true;
			//	}
			//	break;
			}
		
	}

	return nullptr;
}

CPlayerState * CJumpState::LateTick(_float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		if (m_bIsDrop)
		{
			switch (m_eJumpType)
			{
			case Client::Player::CJumpState::JUMP_IDLE:
				if (CPlayer::ALPHEN == m_ePlayerID)
				{
					if (Check_JumpEnd(1.57f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}

				}
				else if (CPlayer::SION == m_ePlayerID)
				{
					if (Check_JumpEnd(1.6f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::RINWELL == m_ePlayerID)
				{
					if (Check_JumpEnd(1.5f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::LAW == m_ePlayerID)
				{
					if (Check_JumpEnd(1.3f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				break;
			case Client::Player::CJumpState::JUMP_RUN:
				if (CPlayer::ALPHEN == m_ePlayerID)
				{
					if (Check_JumpEnd(1.75f))
					{
						//CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::SION == m_ePlayerID)
				{
					if (Check_JumpEnd(0.5f))
					{
						//GameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::RINWELL == m_ePlayerID)
				{
					if (Check_JumpEnd(1.f))
					{
						//CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::LAW == m_ePlayerID)
				{
					if (Check_JumpEnd(1.5f))
					{
						//CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.7f);
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_LANDING);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_LOOP);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				break;
			case Client::Player::CJumpState::JUMP_BATTLE:
				if (CPlayer::ALPHEN == m_ePlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_FALL);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::SION == m_ePlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_FALL);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::RINWELL == m_ePlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_FALL);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				else if (CPlayer::LAW == m_ePlayerID)
				{
					if (Check_JumpEnd(0.1f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_FALL);
						m_eStateType = STATETYPE_MAIN;
					}
				}
				break;
			}
		}
		break;
	case Client::STATETYPE_MAIN:
		if (Check_JumpEnd(1.f))
		{
			m_eStateType = STATETYPE_END;
			switch (m_eJumpType)
			{
			case Client::Player::CJumpState::JUMP_IDLE:
				if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
				else if (CPlayer::SION == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LANDING);
				else if (CPlayer::RINWELL == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LANDING);
				else if (CPlayer::LAW == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_LANDING);
				break;
			case Client::Player::CJumpState::JUMP_RUN:
				if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
				else if (CPlayer::SION == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_LANDING);
				else if (CPlayer::RINWELL == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_LANDING);
				else if (CPlayer::LAW == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_LANDING);
				break;
			case Client::Player::CJumpState::JUMP_BATTLE:
				if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
				else if (CPlayer::SION == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
				else if (CPlayer::RINWELL == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
				else if (CPlayer::LAW == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_LAND);
				break;
			}
		}
		break;
	case Client::STATETYPE_END:
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (!m_bLandSound)
					{
						//CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOTBREAK, 0.3f);
						m_bLandSound = true;
					}
				}
			}
		}
		//CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_JumpLand.wav"), SOUND_FOOT, 0.1f);
		if (m_bIsAnimationFinished)
			return new CIdleState(m_pOwner, CIdleState::IDLE_SIDE);
		break;
	}

	return nullptr;
}

CPlayerState * CJumpState::EventInput(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
			case CPlayer::LAW:
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
				break;
			case CPlayer::SION:
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
				//for Sion State//
				break;

			case CPlayer::RINWELL:
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
				break;
			}
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
			case CPlayer::LAW:
				if (pGameInstance->Key_Down(DIK_E))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::SION:
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::RINWELL:
				if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4, m_fTime);
				break;
			}
		}

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT) && !m_bIsFly)
			return new CDodgeState(m_pOwner, DIR_END, m_fTime);
	}

	if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_BACKWARD_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_S))
		return new CRunState(m_pOwner, DIR_BACKWARD, pGameInstance->Key_Pressing(DIK_LSHIFT));
	else if (pGameInstance->Key_Pressing(DIK_W))
		return new CRunState(m_pOwner, DIR_STRAIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));

	return nullptr;
}

void CJumpState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_JUMP;

	m_pOwner->On_IsFly();

	if (JUMP_IDLE == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LANDING);
			break;
		case CPlayer::RINWELL:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_START);
			else if (STATETYPE_MAIN == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LOOP);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LANDING);
			break;
		case CPlayer::LAW:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_START);
			else if (STATETYPE_MAIN == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_LOOP);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_LANDING);
			break;
		}
	}
	else if (JUMP_RUN == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_END);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_END);
			break;
		case CPlayer::RINWELL:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_START);
			else if (STATETYPE_MAIN == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_LOOP);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_RUN_LANDING);
			break;
		case CPlayer::LAW:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_START);
			else if (STATETYPE_MAIN == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_LOOP);
			else if (true)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::JUMP_RUN_LANDING);
			break;
		}
	}
	else if (JUMP_BATTLE == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_FALL);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_FALL);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
			break;
		case CPlayer::RINWELL:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_FALL);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
			break;
		case CPlayer::LAW:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_FALL);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_LAND);
			break;
		}
	}

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((7.f * 7.f) * (5.f / 23.f)) * -0.5f);
		m_fTime = 7.f / 4.6f;
	}
	else
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(true);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_JumpStart.wav"), SOUND_FOOT, 0.1f);
}

void CJumpState::Exit()
{
	if (STATETYPE_END == m_eStateType && Check_JumpEnd(1.f))
	{
		//CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_JumpLand.wav"), SOUND_FOOT, 0.7f);
		m_pOwner->Off_IsFly();
		m_fTime = 0.f;
	}

	m_bIsDrop = false;

	//CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
}

_bool CJumpState::Check_JumpEnd(_float fOffset)
{
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float EndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (EndHeight + fOffset > XMVectorGetY(vPosition))
		return true;

	return false;
}

void CJumpState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_float4x4 CameraMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

	_vector vCameraLook = XMVectorSet(CameraMatrix.m[2][0], 0.f, CameraMatrix.m[2][2], CameraMatrix.m[2][3]);

	switch (m_eDirection)
	{
	case DIR_STRAIGHT_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)));
		break;
	case DIR_STRAIGHT_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)));
		break;
	case DIR_BACKWARD_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.f)));
		break;
	case DIR_BACKWARD_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(135.f)));
		break;
	case DIR_STRAIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f)));
		break;
	case DIR_BACKWARD:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f)));
		break;
	case DIR_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));
		break;
	case DIR_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f)));
		break;
	case DIR_END:
		vCameraLook = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
	}

	_matrix NewCameraMatrix = XMMatrixIdentity();
	NewCameraMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	NewCameraMatrix.r[2] = vCameraLook;
	NewCameraMatrix.r[0] = XMVector3Cross(NewCameraMatrix.r[1], NewCameraMatrix.r[2]);

	CTransform* pPlayerTransform = m_pOwner->Get_Transform();

	_vector vPlayerScale, vPlayerRotQuat, vPlayerPos;
	_vector vNewCamearScale, vNewCameraRot, vNewCameraPos;

	XMMatrixDecompose(&vPlayerScale, &vPlayerRotQuat, &vPlayerPos, pPlayerTransform->Get_WorldMatrix());
	XMMatrixDecompose(&vNewCamearScale, &vNewCameraRot, &vNewCameraPos, NewCameraMatrix);

	vPlayerRotQuat = XMQuaternionSlerp(vPlayerRotQuat, XMQuaternionNormalize(vNewCameraRot), 0.5f);

	_matrix vPlayerWorld = XMMatrixScalingFromVector(vPlayerScale) * XMMatrixRotationQuaternion(vPlayerRotQuat) * XMMatrixTranslationFromVector(vPlayerPos);

	pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vPlayerWorld.r[0]);
	pPlayerTransform->Set_State(CTransform::STATE_UP, vPlayerWorld.r[1]);
	pPlayerTransform->Set_State(CTransform::STATE_LOOK, vPlayerWorld.r[2]);
	pPlayerTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerWorld.r[3]);

	_vector vPrePos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	m_fTime += fTimeDelta * 3.5f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 7.0f, 4.6f, m_fStartHeight);

	_vector vCurPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float4 fChangeHeight;
	XMStoreFloat4(&fChangeHeight, (vCurPos - vPrePos));

	if (0 > fChangeHeight.y)
		m_bIsDrop = true;
	else
		m_bIsDrop = false;

	if (JUMP_RUN == m_eJumpType)
	{
		if (STATETYPE_END != m_eStateType)
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 3.f);
		else
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f, m_pOwner->Get_Navigation());
	}
	else if ((JUMP_BATTLE == m_eJumpType) && (m_eDirection != DIR_END) && (STATETYPE_END != m_eStateType))
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f, m_pOwner->Get_Navigation());
}