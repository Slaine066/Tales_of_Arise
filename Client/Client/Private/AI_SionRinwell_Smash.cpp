#include "stdafx.h"
#include "..\Public\AI_SionRinwell_Smash.h"
#include "GameInstance.h"
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
#include "UI_Skillmessage.h"
#include "Animation.h"
#include "UI_Dialogue_Caption.h"

using namespace AIPlayer;
using namespace Player;

CAI_SionRinwell_Smash::CAI_SionRinwell_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_SionRinwell_Smash::Tick(_float fTimeDelta)
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


	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
	}
	else
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	if (m_pTarget == nullptr)
		return nullptr;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::SION:
				{
						
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType && !m_bBullet)
					{
						m_fEffectEventEndTime = pEvent.fEndTime;
						m_fEffectEventCurTime = m_pOwner->Get_Model()->Get_Animations()[m_pOwner->Get_Model()->Get_CurrentAnimIndex()]->Get_CurrentTime();

						m_bStrikeBlur = true;

						/* Make Effect */
						_vector vOffset = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = XMVectorSet(62.f, 0.11f, 54.f, 1.f) +  vOffset*2.f + XMVectorSet(0.f, 3.f, 0.f, 0.f);;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionRinwellStrikeBoom.dat"), mWorldMatrix);
						m_fFadeTime = 0.f;
						m_bBullet = true;
					}
					break;
				}

				case CPlayer::RINWELL:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					{

						if ((m_fEventStart1 != pEvent.fStartTime))
						{
							if (m_iEventIndex == 0)
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(4);
							else
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Next_Dialogueindex();

							++m_iEventIndex;
							m_fEventStart1 = pEvent.fStartTime;
						}

					}
					else if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime) && !m_bBullet)
						{

							/* Make Effect */
							_vector vOffset = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] +=  vOffset*2.f + XMVectorSet(0.f, 2.f, 0.f, 0.f);
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionRinwellStrike.dat"), mWorldMatrix);
							m_bBullet = true;
							m_fEventStart = pEvent.fStartTime;
						}
					}
				}
				}
			}
		}
		
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAI_SionRinwell_Smash::LateTick(_float fTimeDelta)
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

	if (!m_bShaking && m_bBullet)
	{
		m_fShakingTime += fTimeDelta;

		if (m_fShakingTime > 0.5f)
		{
			if (m_eCurrentPlayerID == CPlayer::SION)
			{
				CUI_Manager::Get_Instance()->Set_UIStrike(true);
				if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
					dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.1f);

				m_bShaking = true;
			}

		}
	}

	if (m_bBullet && !m_bScreen)
	{
		m_fFadeTime += fTimeDelta;
		if (m_fFadeTime > 4.5f)
		{
			if (m_eCurrentPlayerID == CPlayer::SION)
			{
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

void CAI_SionRinwell_Smash::Enter()
{
	if (m_eCurrentPlayerID == CPlayer::SION)
		m_pOwner->Get_Renderer()->Set_ZoomBlur(false);

	m_bStrikeBlur = false;

	m_pOwner->Set_StrikeAttack(true);
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::ANIM_RINWELLSION_STRIKE;
	   dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_SIONRINWELLSTRIKE);
	
		break;
	case CPlayer::RINWELL:

		m_iCurrentAnimIndex = CRinwell::ANIM::ANIM_SIONRINWELL_STRIKE;
	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Model()->Reset();

	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	 
	m_pOwner->Set_Manarecover(false);
	if (!m_bSoundStart)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("SionRinwell_Smash.wav"), SOUND_SMASH, 0.47f);
		m_bSoundStart = true;
	}
}

void CAI_SionRinwell_Smash::Exit()
{
	if (m_eCurrentPlayerID == CPlayer::SION)
	{
		if (m_bStrikeBlur)
		{
			/*m_pOwner->Set_ResetStrikeBlur(true);*/
			m_pOwner->Get_Renderer()->Set_ZoomBlur(false);
			m_bStrikeBlur = false;
		}
	}

	


	m_pOwner->Set_StrikeAttack(false);
	m_pOwner->Set_IsActionMode(false);

	if (m_eCurrentPlayerID == CPlayer::SION)
	{
		dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->offdialogue();
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bShaking = false;

		CBattleManager::Get_Instance()->Set_IsStrike(false);
		CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
		if (pLockOn != nullptr)
		{
			_vector vLastPosition = dynamic_cast<CMonster*>(pLockOn)->Get_LastPosition();
			if (!dynamic_cast<CMonster*>(pLockOn)->Get_LastStrikeAttack())
			{
				dynamic_cast<CMonster*>(pLockOn)->Set_LastStrikeAttack(true);
				dynamic_cast<CMonster*>(pLockOn)->Set_State(CTransform::STATE_TRANSLATION, vLastPosition);
				dynamic_cast<CMonster*>(pLockOn)->Take_Damage(9999, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
			}
			else
			{
				dynamic_cast<CMonster*>(pLockOn)->Set_State(CTransform::STATE_TRANSLATION, vLastPosition);
				dynamic_cast<CMonster*>(pLockOn)->Take_Damage(9999, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
			}
		}

	}
	else
	{
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();
	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

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
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	__super::Exit();
}

void CAI_SionRinwell_Smash::StrikeBlur(_float fTimeDelta)
{
	_float fDuration = 0.4f;
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