#include "stdafx.h"

#include "PlayerRunState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir)
	: m_eDirection(eDir)
{
	m_pOwner = pPlayer;
}

CPlayerState * CRunState::HandleInput()
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
	else
		return new CIdleState(m_pOwner);

	return nullptr;
}

CPlayerState * CRunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	Move(fTimeDelta);

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CRunState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CRunState::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CPlayer::ANIM::ANIM_RUN);
}

void CRunState::Exit()
{

}

void CRunState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4 CameraMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

	_float fCosBtwCP = XMVectorGetX(XMVector3Dot(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK), XMVectorSet(CameraMatrix.m[2][0], 0.f, CameraMatrix.m[2][2], 0.f)));

	m_pOwner->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), acosf(fCosBtwCP));

	switch (m_eDirection)
	{
	/*case DIR_STRAIGHT_LEFT:
		m_pOwner->Get_Transform()->Set_RotationY(305.f);
		break;
	case DIR_STRAIGHT_RIGHT:
		m_pOwner->Get_Transform()->Set_RotationY(45.f);
		break;
	case DIR_BACKWARD_LEFT:
		m_pOwner->Get_Transform()->Set_RotationY(225.f);
		break;
	case DIR_BACKWARD_RIGHT:
		m_pOwner->Get_Transform()->Set_RotationY(135.f);
		break;*/
	case DIR_STRAIGHT:
		
		break;
	case DIR_BACKWARD:
		m_pOwner->Get_Transform()->Sliding_Backward(fTimeDelta, m_pOwner->Get_Navigation());
		break;
	case DIR_LEFT:
		m_pOwner->Get_Transform()->Sliding_Left(fTimeDelta, m_pOwner->Get_Navigation());
		break;
	case DIR_RIGHT:
		m_pOwner->Get_Transform()->Sliding_Right(fTimeDelta, m_pOwner->Get_Navigation());
		break;
	}

	m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta, m_pOwner->Get_Navigation());

	RELEASE_INSTANCE(CGameInstance);
}