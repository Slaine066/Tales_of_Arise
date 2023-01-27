#include "stdafx.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "PlayerRunState.h"
#include "PlayerAttackNormalState.h"

#include "Effect.h"
#include "EffectTexture.h"

using namespace Player;

CIdleState::CIdleState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CIdleState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CAttackNormalState(m_pOwner);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
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
	else if (pGameInstance->Key_Pressing(DIK_M))
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.01f);
	/* Particle Test */
	else if (pGameInstance->Key_Down(DIK_P))
	{
		//CEffect::PlayEffect(TEXT("SparkTest.dat"), m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
		CEffectTexture::TEXTUREEFFECTDESC tTextureEffectDesc;
		wcscpy_s(tTextureEffectDesc.wcPrototypeId, MAX_PATH, TEXT("Spark_00"));
		tTextureEffectDesc.bIsDistortion = true;
		CEffect* pEffect = nullptr;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectTexture"), LEVEL_STATIC, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tTextureEffectDesc);
		pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_TEXTURE);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + XMVectorSet(0.f, 1.f, 0.f, 0.f);
		pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
		RELEASE_INSTANCE(CGameInstance);
	}
	
	return nullptr;
}

CPlayerState * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CPlayer::ANIM::ANIM_IDLE);
}

void CIdleState::Exit()
{
}