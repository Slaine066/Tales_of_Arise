#include "stdafx.h"
#include "..\Public\IceWolfHowLingState.h"


using namespace IceWolf;

CHowLingState::CHowLingState(CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CHowLingState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CHawkState * CHowLingState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())))
	{
			//return new CWalkFrontState(m_pOwner);
	}

	return nullptr;
}

CHawkState * CHowLingState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CHowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_HOWLING);
}

void CHowLingState::Exit()
{
}



