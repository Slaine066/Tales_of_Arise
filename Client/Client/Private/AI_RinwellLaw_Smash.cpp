#include "stdafx.h"
#include "..\Public\AI_RinwellLaw_Smash.h"
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
#include "PlayerIdleState.h"
#include "UI_Dialogue_Caption.h"
#include "UI_Skillmessage.h"

using namespace AIPlayer;
using namespace Player;

CAI_RinwellLaw_Smash::CAI_RinwellLaw_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	m_pTarget = pTarget;
}

CAIState * CAI_RinwellLaw_Smash::Tick(_float fTimeDelta)
{
	CGameInstance::Get_Instance()->StopSound(SOUND_ALPHEN_NORMAL_ATTACK);
	CGameInstance::Get_Instance()->StopSound(SOUND_ALPHEN_VOICE);
	CGameInstance::Get_Instance()->StopSound(SOUND_ALPHEN_SKILL);
	CGameInstance::Get_Instance()->StopSound(SOUND_SION_VOICE);
	CGameInstance::Get_Instance()->StopSound(SOUND_SION_SKILL);
	CGameInstance::Get_Instance()->StopSound(SOUND_RINWELL_NORMAL_ATTCK);
	CGameInstance::Get_Instance()->StopSound(SOUND_RINWELL_VOICE);
	CGameInstance::Get_Instance()->StopSound(SOUND_RINWELL_SKILL);
	CGameInstance::Get_Instance()->StopSound(SOUND_LAW_NORMAL_ATTACK);
	CGameInstance::Get_Instance()->StopSound(SOUND_LAW_VOICE);
	CGameInstance::Get_Instance()->StopSound(SOUND_LAW_SKILL);
	CGameInstance::Get_Instance()->StopSound(SOUND_LAW_SKILL);


	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL1);
	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL2);
	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL3);
	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL4);
	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL5);
	CGameInstance::Get_Instance()->StopSound(SOUND_SKILL);
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	CGameInstance::Get_Instance()->StopSound(SOUND_OBJECT);
	CGameInstance::Get_Instance()->StopSound(SOUND_NATURE);
	CGameInstance::Get_Instance()->StopSound(SOUND_CROWD);

	if (m_bStrikeBlur)
		StrikeBlur(fTimeDelta);

	m_fTimer += fTimeDelta;

	if (m_pOwner->Get_PlayerID() == CPlayer::RINWELL)
	{
		if (m_fTimer > 2.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();
			m_fTimer = -100.f;
		}

	}

	if (m_pOwner->Get_PlayerID() == CPlayer::LAW)
	{
		if (m_fTimer > 4.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_RINWELLLAWSTRIKE);
			m_fTimer = -100.f;
		}

	}


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60")), false);
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::LAW:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					{

						if ((m_fEventStart1 != pEvent.fStartTime))
						{
							if (m_iEventIndex == 0)
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(1);
							else
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Next_Dialogueindex();

							++m_iEventIndex;
							m_fEventStart1 = pEvent.fStartTime;
						}

					}
					else if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							if (!strcmp(pEvent.szName, "Begin"))
							{
								/* Make Effect */
								CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);

								_matrix mWorldMatrix2 = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix2.r[3] = m_vEffectPos[0];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_Begin.dat"), mWorldMatrix2);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix2.r[3]);

								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "Punch1"))
							{
								CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[0]);

								dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_HitState();
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[1];
								CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_BeginPunchFlash.dat"), mWorldMatrix);

								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_BeginPunchFlash.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
									dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.f, 0.1f);


								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "Punch2"))
							{
								CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);
								dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_HitState();

								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[2];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_Punch.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
									dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.f, 0.1f);

					
								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "FootPunch"))
							{
								dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_HitState();
								CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);
								
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[2];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_Punch.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
									dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.f, 0.1f);

								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "FootEnd"))
							{
								
								dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_HitState();
								CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);

								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[3];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_FootEnd.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
									dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.02f);

								m_bStrikeBlur = true;
								m_bBullet = true;

							}
							m_fEventStart = pEvent.fStartTime;
						}
					}
				}
				}
			}
		}

	}

	

	return nullptr;
}

CAIState * CAI_RinwellLaw_Smash::LateTick(_float fTimeDelta)
{
	
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (m_bIsAnimationFinished)
	{

		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
			CUI_Manager::Get_Instance()->Get_UIStrike() == false)
		{

			PLAYER_MODE eMode = CPlayerManager::Get_Instance()->Check_ActiveMode(m_pOwner);
			switch (eMode)
			{
			case Client::ACTIVE:
			{
				CPlayerState* pState = nullptr;

				pState = new CIdleState(m_pOwner, CIdleState::IDLE_SIDE);
				m_pOwner->Set_PlayerState(m_pOwner->Get_PlayerState()->ChangeState(m_pOwner->Get_PlayerState(), pState));
			}

			case Client::AI_MODE:
			{
				return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
			}
			}

		}


	}

	if (m_bBullet && !m_bScreen)
	{
		m_fFadeTime += fTimeDelta;

		if (m_fFadeTime > 0.8f)
		{
			if (m_eCurrentPlayerID == CPlayer::LAW)
			{
				CUI_Manager::Get_Instance()->Set_UIStrike(true);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"));
				m_bScreen = true;
				if (m_bStrikeBlur)
				{
					m_pOwner->Set_ResetStrikeBlur(true);
					m_bStrikeBlur = false;
				}
			}
		}
	}

	return nullptr;
}

void CAI_RinwellLaw_Smash::Enter()
{
	if (m_eCurrentPlayerID == CPlayer::LAW)
		m_pOwner->Get_Renderer()->Set_ZoomBlur(false);

	m_bStrikeBlur = false;

	m_pOwner->Set_StrikeAttack(true);
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		break;
	case CPlayer::LAW:
	{		
	Set_EffectPosition();
	CBattleManager::Get_Instance()->Get_LackonMonster()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);
	_matrix mWorldMatrix2 = m_pOwner->Get_Transform()->Get_WorldMatrix();
	mWorldMatrix2.r[3] = m_vEffectPos[0];
	vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawFinishMove_LoopEffect.dat"), mWorldMatrix2);
	_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
	_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix2.r[3]);

	for (auto& pEffect : Punch)
	{
		vPosition += vCamDir*0.1f;
		pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
	}	
	m_iCurrentAnimIndex = CLaw::ANIM::BTL_MYSTIC_GURENTENSYOU;
	//dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_RAINOUI);
	

	break;
	}

	}
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Model()->Reset();

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_Target(m_pOwner);
	m_pOwner->Set_Manarecover(false);
	
	if (!m_bSoundStart)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("LawRinwell_Smash.wav"), SOUND_SMASH, 0.65f);
		m_bSoundStart = true;
	}
	
}

void CAI_RinwellLaw_Smash::Exit()
{
	if (m_eCurrentPlayerID == CPlayer::LAW)
	{
		if (m_bStrikeBlur)
		{
			/*m_pOwner->Set_ResetStrikeBlur(true);*/
			m_pOwner->Get_Renderer()->Set_ZoomBlur(false);
			m_bStrikeBlur = false;
		}
	}

	if (m_eCurrentPlayerID == CPlayer::LAW)
	{

		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();
		dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->offdialogue();

		if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		{
			HITLAGDESC m_HitLagDesc;
			m_HitLagDesc.bHitLag = false;
			m_HitLagDesc.bLockOnChange = false;
			m_HitLagDesc.bShaking = false;


			if (!dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_LastStrikeAttack())
			{
				dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_LastStrikeAttack(true);
				dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Take_Damage(9999, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
			}
			else
			{
				dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Take_Damage(9999, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
			}
		}

	}

	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();

	m_pOwner->Set_StrikeAttack(false);
	m_pOwner->Set_IsActionMode(false);
	CBattleManager::Get_Instance()->Set_IsStrike(false);


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

void CAI_RinwellLaw_Smash::StrikeBlur(_float fTimeDelta)
{
	_float fDuration = .45f;
	m_fResetTimer += fTimeDelta;

	/* Zoom Blur Lerp */
	_float fFocusPower = 10.f;

	_float fBlurInterpFactor = m_fResetTimer / fDuration;
	if (fBlurInterpFactor > 1.f)
		fBlurInterpFactor = 1.f;

	_int iDetailStart = 1;
	_int iDetailEnd = 10;
	_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
	m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
}

void CAI_RinwellLaw_Smash::Set_EffectPosition()
{
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC Model;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/EffectPosition/LawFinishMove_EffectPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;

	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	for (int i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(Model), sizeof(NONANIMDESC), &dwByte, nullptr);
		m_vEffectPos.push_back(XMVectorSetW(XMLoadFloat3(&Model.vPosition), 1.f));
	}

	CloseHandle(hFile);

	m_vStrikeLockOnPos = CPlayerManager::Get_Instance()->Get_StrikeLockOnPosition();
}
