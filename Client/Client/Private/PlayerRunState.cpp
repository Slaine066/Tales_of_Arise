#include "stdafx.h"

#include "GameInstance.h"
#include "CameraManager.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerCollectState.h"
#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"
#include "Player_RinwellNormalAttack_State.h"
#include "Player_RinwellSkillAttack_State.h"
#include "CloseChaseState.h"
#include "AlphenAttackState.h"

#include "Level_Restaurant.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir, _bool isDash)
	: m_eDirection(eDir)
	, m_bIsDash(isDash)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CRunState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CBattleManager* pBattleManager = CBattleManager::Get_Instance();

	if (pBattleManager->Get_IsBattleMode())
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
		case CPlayer::LAW:
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
			if (pGameInstance->Key_Down(DIK_E))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
			if (pGameInstance->Key_Down(DIK_R))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
			if (pGameInstance->Key_Down(DIK_F))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
		case CPlayer::SION:
			//for Sion State//
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
			break;
		case CPlayer::RINWELL:
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
			break;
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::SION:
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);

				break;

			case CPlayer::RINWELL: //For Rinwell State
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK5);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK6);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			}
		}

		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE);

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_END);
	}

	else if (!pBattleManager->Get_IsBattleMode())
	{
		if ((LEVEL_RESTAURANT == pGameInstance->Get_CurrentLevelIndex()) && ((CLevel_Restaurant*)pGameInstance->Get_CurrentLevel())->Get_MiniGameStart())
		{
			if (CPlayer::ALPHEN == m_ePlayerID && pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
		}

		else
		{
			if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
				return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_RUN);
		}
	}


	if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_STRAIGHT_LEFT;
	}
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_STRAIGHT_RIGHT;
	}
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_BACKWARD_LEFT;
	}
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_BACKWARD_RIGHT;
	}
	else if (pGameInstance->Key_Pressing(DIK_A))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_LEFT;
	}
	else if (pGameInstance->Key_Pressing(DIK_D))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_RIGHT;
	}
	else if (pGameInstance->Key_Pressing(DIK_S))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_BACKWARD;
	}
	else if (pGameInstance->Key_Pressing(DIK_W))
	{
		if (STATETYPE_END == m_eStateType)
		{
			m_eStateType = STATETYPE_START;
			m_fRunTime = 0.f;
			Enter();
		}
		m_eDirection = DIR_STRAIGHT;
	}
	else
	{
		if (STATETYPE_START == m_eStateType)
		{
			if (0.5f > m_fRunTime)
				return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
			else
			{
				m_eStateType = STATETYPE_END;
				Enter();

				return nullptr;
			}
		}
	}

	if (STATETYPE_END != m_eStateType)
	{
		if (!pBattleManager->Get_IsBattleMode())
		{
			if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				if (!m_bIsDash)
				{
					if (CPlayer::ALPHEN == m_ePlayerID)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
					else if (CPlayer::SION == m_ePlayerID)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH);
					else if (CPlayer::RINWELL == m_ePlayerID)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::DASH);
					else if (CPlayer::LAW == m_ePlayerID)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DASH);
				}

				m_bIsDash = true;

				if (!CBattleManager::Get_Instance()->Get_IsBattleMode() && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
				{
					CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
					pCamera->Set_Zoom(true);
				}
			}
			else
			{
				if (m_bIsDash)
				{
					switch (m_ePlayerID)
					{
					case CPlayer::ALPHEN:
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);
						break;
					case CPlayer::SION:
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::SYS_RUN);
						break;
					case CPlayer::RINWELL:
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN);
						break;
					case CPlayer::LAW:
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::RUN);
						break;
					}
				}

				m_bIsDash = false;

				if (!pBattleManager->Get_IsBattleMode() && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
				{
					CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
					pCamera->Set_Zoom(false);
				}
			}
		}
	}
	
	return nullptr;
}

CPlayerState * CRunState::Tick(_float fTimeDelta)
{
	CBattleManager* pBattleManager = CBattleManager::Get_Instance();
	if (STATETYPE_START == m_eStateType)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.15f);
		
		Move(fTimeDelta);

		m_fRunTime += fTimeDelta;

		if (m_bIsDash)
		{
			vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

			for (auto& pEvent : pEvents)
			{
				if (pEvent.isPlay)
				{
					switch (m_ePlayerID)
					{

					case CPlayer::ALPHEN:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fDashSoundStart != pEvent.fStartTime)
							{
								if (m_iDashSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound0.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 1;
								}

								else if (m_iDashSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound1.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 2;
								}

								else if (m_iDashSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound2.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 3;
								}

								else if (m_iDashSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound3.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 4;
								}

								else if (m_iDashSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound4.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 5;
								}

								else if (m_iDashSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_DashSound5.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 0;
								}

								m_fDashSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::SION:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fDashSoundStart != pEvent.fStartTime)
							{
								if (m_iDashSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSound0.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 1;
								}

								else if (m_iDashSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSound1.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 2;
								}

								else if (m_iDashSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSound2.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 3;
								}

								else if (m_iDashSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSound3.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 4;
								}

								else if (m_iDashSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSound4.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 0;
								}

								m_fDashSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::RINWELL:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fDashSoundStart != pEvent.fStartTime)
							{
								if (m_iDashSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound0.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 1;
								}

								else if (m_iDashSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound1.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 2;
								}

								else if (m_iDashSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound2.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 3;
								}

								else if (m_iDashSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound3.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 4;
								}

								else if (m_iDashSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound4.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 5;
								}

								else if (m_iDashSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound5.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 6;
								}

								else if (m_iDashSoundCount == 6)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound6.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 7;
								}

								else if (m_iDashSoundCount == 7)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSound7.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 8;
								}

								else if (m_iDashSoundCount == 8)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSoun8.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 0;
								}

								m_fDashSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::LAW:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fDashSoundStart != pEvent.fStartTime)
							{
								if (m_iDashSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound0.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 1;
								}

								else if (m_iDashSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound1.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 2;
								}

								else if (m_iDashSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound2.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 3;
								}

								else if (m_iDashSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound3.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 4;
								}

								else if (m_iDashSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound4.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 5;
								}

								else if (m_iDashSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSound5.wav"), SOUND_FOOT, 0.3f);
									m_iDashSoundCount = 0;
								}

								m_fDashSoundStart = pEvent.fStartTime;
							}
						}
						break;

					}
				}
			}
		}

		else if (!m_bIsDash)
		{
			vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

			for (auto& pEvent : pEvents)
			{
				if (pEvent.isPlay)
				{
					switch (m_ePlayerID)
					{

					case CPlayer::ALPHEN:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fSoundStart != pEvent.fStartTime)
							{
								if (m_iRunSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_FootSound1.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 1;
								}

								else if (m_iRunSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_FootSound2.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 2;
								}

								else if (m_iRunSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_FootSound3.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 3;
								}

								else if (m_iRunSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_FootSound4.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 0;
								}

								m_fSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::SION:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fSoundStart != pEvent.fStartTime)
							{
								if (m_iRunSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound1.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 1;
								}

								else if (m_iRunSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound2.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 2;
								}

								else if (m_iRunSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound3.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 3;
								}

								else if (m_iRunSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound4.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 4;
								}

								else if (m_iRunSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound5.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 5;
								}

								else if (m_iRunSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound6.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 6;
								}

								else if (m_iRunSoundCount == 6)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_FootSound7.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 0;
								}

								m_fSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::RINWELL:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fSoundStart != pEvent.fStartTime)
							{
								if (m_iRunSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_FootSound1.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 1;
								}

								else if (m_iRunSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_FootSound2.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 2;
								}

								else if (m_iRunSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_FootSound3.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 3;
								}

								else if (m_iRunSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_FootSound4.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 0;
								}

								m_fSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::LAW:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fSoundStart != pEvent.fStartTime)
							{
								if (m_iRunSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound1.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 1;
								}

								else if (m_iRunSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound2.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 2;
								}

								else if (m_iRunSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound3.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 3;
								}

								else if (m_iRunSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound4.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 4;
								}

								else if (m_iRunSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound5.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 5;
								}

								else if (m_iRunSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_FootSound6.wav"), SOUND_FOOT, 0.3f);
									m_iRunSoundCount = 0;
								}

								m_fSoundStart = pEvent.fStartTime;
							}
						}
						break;
					}
				}
			}

		}

		else if (pBattleManager->Get_IsBattleMode())
		{
			vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

			for (auto& pEvent : pEvents)
			{
				if (pEvent.isPlay)
				{
					switch (m_eStateId)
					{
					case CPlayer::ALPHEN:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fBattleSoundStart != pEvent.fStartTime)
							{
								if (m_iBattleSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run1.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 1;
								}

								else if (m_iBattleSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run2.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 2;
								}

								else if (m_iBattleSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run3.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 3;
								}

								else if (m_iBattleSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run4.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 4;
								}

								else if (m_iBattleSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run5.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 5;
								}

								else if (m_iBattleSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run6.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 0;
								}

								else if (m_iBattleSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Battle_Run7.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 0;
								}

								m_fBattleSoundStart = pEvent.fStartTime;
							}
						}

						break;

					case CPlayer::SION:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fBattleSoundStart != pEvent.fStartTime)
							{
								if (m_iBattleSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run1.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 1;
								}

								else if (m_iBattleSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run2.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 2;
								}

								else if (m_iBattleSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run3.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 3;
								}

								else if (m_iBattleSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run4.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 4;
								}

								else if (m_iBattleSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run5.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 5;
								}

								else if (m_iBattleSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run6.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 6;
								}

								else if (m_iBattleSoundCount == 6)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Battle_Run7.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 0;
								}

								m_fBattleSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::RINWELL:
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fBattleSoundStart != pEvent.fStartTime)
							{
								if (m_iBattleSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run1.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 1;
								}

								else if (m_iBattleSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run2.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 2;
								}

								else if (m_iBattleSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run3.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 3;
								}

								else if (m_iBattleSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run4.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 4;
								}

								else if (m_iBattleSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run5.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 5;
								}

								else if (m_iBattleSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run6.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 6;
								}

								else if (m_iBattleSoundCount == 6)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Battle_Run7.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 0;
								}

								m_fBattleSoundStart = pEvent.fStartTime;
							}
						}
						break;

					case CPlayer::LAW :
						if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
						{
							if (m_fBattleSoundStart != pEvent.fStartTime)
							{
								if (m_iBattleSoundCount == 0)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run1.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 1;
								}

								else if (m_iBattleSoundCount == 1)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run2.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 2;
								}

								else if (m_iBattleSoundCount == 2)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run3.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 3;
								}

								else if (m_iBattleSoundCount == 3)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run4.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 4;
								}

								else if (m_iBattleSoundCount == 4)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run5.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 5;
								}

								else if (m_iBattleSoundCount == 5)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run6.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 6;
								}

								else if (m_iBattleSoundCount == 6)
								{
									CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Battle_Run7.wav"), SOUND_FOOT, 0.2f);
									m_iBattleSoundCount = 0;
								}

								m_fBattleSoundStart = pEvent.fStartTime;
							}
						}
						break;


					}
				}
			}
		}
	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());
		}
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CRunState::LateTick(_float fTimeDelta)
{
	if (STATETYPE_END == m_eStateType && m_bIsAnimationFinished)
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	
	return nullptr;
}

void CRunState::Enter()
{
	__super::Enter();

	CBattleManager* pBattleManager = CBattleManager::Get_Instance();
	m_eStateId = STATE_ID::STATE_RUN;

	if (m_bIsDash)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (!pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH_BRAKE_LEFT_START);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSoundBreak.wav"), SOUND_FOOTBREAK, 0.65f);
				}
			}
			
			break;
		case CPlayer::SION:
			if (!pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH_BRAKE_000);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_DashSoundBreak.wav"), SOUND_FOOTBREAK, 0.55f);
				}
			}
			break;
		case CPlayer::RINWELL:
			if (!pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::DASH);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::DASH_BRAKE_000);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_DashSoundBreak.wav"), SOUND_FOOTBREAK, 0.55f);
				}
			}
			
			break;
		case CPlayer::LAW:
			if (!pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DASH);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DASH_BREAK_START);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_DashSoundBreak.wav"), SOUND_FOOTBREAK, 0.65f);
				}
			}
			break;
		}
	}
	else
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_RUN);

				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_BREAK);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.2f);
				}
			}
			else
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN_BRAKE_START);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.55f);
				}
			}
			break;
		case CPlayer::SION:
			if (pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_BRAKE);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.2f);
				}
			}
			else
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::SYS_RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::RUN_BRAKE_000);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.5f);
				}
			}
			break;
		case CPlayer::RINWELL:
			if (pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MOVE_RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN_BRAKE_000);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.2f);
				}
			}
			else
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN_BRAKE_000);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.5f);
				}
			}
			break;
		case CPlayer::LAW:
			if (pBattleManager->Get_IsBattleMode())
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_BRAKE);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.2f);
				}
			}
			else
			{
				if (STATETYPE_START == m_eStateType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::RUN);
				else
				{
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::RUN_BRAKE);
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Field_RunBreak.wav"), SOUND_FOOTBREAK, 0.55f);
				}
			}
			break;
		}
	}

	m_pOwner->Set_Manarecover(true);
}

void CRunState::Exit()
{
	//CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);

	if (m_bIsDash)
	{
		if (!CBattleManager::Get_Instance()->Get_IsBattleMode() && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(false);
		}
	}
}

void CRunState::Move(_float fTimeDelta)
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

	if (m_bIsDash)
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 4.f, m_pOwner->Get_Navigation());
	else
	{
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2.f, m_pOwner->Get_Navigation());
	}
}