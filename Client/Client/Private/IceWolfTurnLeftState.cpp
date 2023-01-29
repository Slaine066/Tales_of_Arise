#include "stdafx.h"
#include "..\Public\IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfChaseState.h"

using namespace IceWolf;

CTurnLeftState::CTurnLeftState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CTurnLeftState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CTurnLeftState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	

	return nullptr;
}

CIceWolfState * CTurnLeftState::LateTick(_float fTimeDelta)
{
	
	
	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	if (m_bIsAnimationFinished)
	{	
		
		return new CIdleState(m_pOwner, STATE_TURN_L, STATE_TURN_L);
	}



	return nullptr;
}

void CTurnLeftState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_LEFT);
}

void CTurnLeftState::Exit()
{
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	m_pOwner->Get_Model()->Reset();
}



