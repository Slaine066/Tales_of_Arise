#include "stdafx.h"
#include "AIAttackNormalState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AICheckState.h"
#include "AIIdleState.h"

using namespace AIPlayer;

CAIAttackNormalState::CAIAttackNormalState(CPlayer* pPlayer , STATE_ID eStateType , CBaseObj* pTarget)
{
	m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_pTarget = pTarget;
	
}

CAIState * CAIAttackNormalState::Tick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()) , "TransN");

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAIAttackNormalState::LateTick(_float fTimeDelta)
{
	switch (m_eCurrentPlayerID)
	{
	case 0:                      //alphen
		break;


	case 1:                               //sion
		if (m_bIsAnimationFinished)
		{

			if (m_iCurrentAnimIndex == CSion::ANIM::BTL_ATTACK_NORMAL_4)
				return new CAICheckState(m_pOwner, STATE_ID::STATE_IDLE);
			else
			{
				++m_iCurrentAnimIndex;
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			}

		}
		break;
	}

	return nullptr;
}

void CAIAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;
	m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);


}

void CAIAttackNormalState::Exit()
{
	__super::Exit();
}