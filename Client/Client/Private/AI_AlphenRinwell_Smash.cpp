#include "stdafx.h"
#include "..\Public\AI_AlphenRinwell_Smash.h"
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
#include "UI_Skillmessage.h"
#include "Animation.h"
#include "UI_Dialogue_Caption.h"

using namespace AIPlayer;
using namespace Player;
CAI_AlphenRinwell_Smash::CAI_AlphenRinwell_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
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

CAIState * CAI_AlphenRinwell_Smash::Tick(_float fTimeDelta)
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

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta,false);
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::ALPHEN:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					{
						
						if ((m_fEventStart1 != pEvent.fStartTime))
						{
							if (m_iEventIndex == 0)
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(3);
							else
								dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Next_Dialogueindex();

							++m_iEventIndex;
							m_fEventStart1 = pEvent.fStartTime;
						}

					}

						
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime) && !m_bBullet)
						{
							m_fEffectEventEndTime = pEvent.fEndTime;
							m_fEffectEventCurTime = m_pOwner->Get_Model()->Get_Animations()[m_pOwner->Get_Model()->Get_CurrentAnimIndex()]->Get_CurrentTime();

							m_bStrikeBlur = true;

							CUI_Manager::Get_Instance()->Set_UIStrike(true);
							if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
								dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 2.f, 0.1f);

							/* Make Effect */
							_vector vOffset = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] += vOffset*7.f + XMVectorSet(0.f, 0.f, 0.f, 0.f);
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Element2.dat"), mWorldMatrix);
							m_fFadeTime = 0.f;
							m_bBullet = true;
							m_fEventStart = pEvent.fStartTime;
						}
					}
					break;
				}

				case CPlayer::RINWELL:
				{
					
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime) && !m_bBullet)
						{
							/* Make Effect */
							_vector vOffset = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = mWorldMatrix.r[3] + vOffset*-20.f + XMVectorSet(0.f, 10.f, 0.f, 0.f);
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ElementBlue.dat"), mWorldMatrix);

							mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = mWorldMatrix.r[3] + vOffset*-20.f + XMVectorSet(3.f, 5.f, 0.f, 0.f);
							m_pEffects2 = CEffect::PlayEffectAtLocation(TEXT("ElementGreen.dat"), mWorldMatrix);

							mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = mWorldMatrix.r[3] +  vOffset*-20.f + XMVectorSet(-3.f, 5.f, 0.f, 0.f);
							m_pEffects3 = CEffect::PlayEffectAtLocation(TEXT("ElementYellow.dat"), mWorldMatrix);
							m_fFadeTime = 0.f;
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

CAIState * CAI_AlphenRinwell_Smash::LateTick(_float fTimeDelta)
{
	

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	for (auto& iter : m_pEffects2)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	for (auto& iter : m_pEffects3)
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
		if (m_fFadeTime > 1.7f)
		{
			if (m_eCurrentPlayerID == CPlayer::ALPHEN)
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

void CAI_AlphenRinwell_Smash::Enter()
{
	if (m_eCurrentPlayerID == CPlayer::ALPHEN)
		m_pOwner->Get_Renderer()->Set_ZoomBlur(false);

	m_bStrikeBlur = false;

	m_pOwner->Set_StrikeAttack(true);
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_RINWELLALPHEN_SMASH;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_ALPHENRINWELLSTRIKE);
		
		
		break;
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::ANIM_ALPHENRINWELL_STRIKE;

		break;


	}

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
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
		CGameInstance::Get_Instance()->PlaySounds(TEXT("AlphenRinwell_Smash.wav"), SOUND_SMASH, 0.6f);
		m_bSoundStart = true;
	}

	
}

void CAI_AlphenRinwell_Smash::Exit()
{
	if (m_eCurrentPlayerID == CPlayer::ALPHEN)
	{
		if (m_bStrikeBlur)
		{
			/*m_pOwner->Set_ResetStrikeBlur(true);*/
			m_pOwner->Get_Renderer()->Set_ZoomBlur(false);
			m_bStrikeBlur = false;
		}
	}

	if (m_eCurrentPlayerID == CPlayer::ALPHEN)
	{
		dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->offdialogue();
		/*if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"))))
			return;*/
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();
	}


	
	m_pOwner->Set_StrikeAttack(false);
	m_pOwner->Set_IsActionMode(false);
	CBattleManager::Get_Instance()->Set_IsStrike(false);
	if (m_eCurrentPlayerID == CPlayer::ALPHEN)
	{
		CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bShaking = false;
		if (pLockOn != nullptr)
		{
			_vector vLastPosition = dynamic_cast<CMonster*>(pLockOn)->Get_LastPosition();
			if (!CBattleManager::Get_Instance()->Get_Rinwellboss())
			{
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

	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}
	for (auto& iter : m_pEffects2)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}
	for (auto& iter : m_pEffects3)
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

	if (!m_pEffects2.empty())
	{
		for (auto& iter : m_pEffects2)
		{
			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	if (!m_pEffects3.empty())
	{
		for (auto& iter : m_pEffects3)
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
//	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	__super::Exit();
}

void CAI_AlphenRinwell_Smash::StrikeBlur(_float fTimeDelta)
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