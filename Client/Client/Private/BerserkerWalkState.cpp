#include "stdafx.h"

#include "BerserkerWalkState.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerHowLing_State.h"
#include "BerserkerTrigger_TurnState.h"


using namespace Berserker;

CWalkState::CWalkState(CBerserker* pBerserker, FIELD_STATE_ID ePreState, _bool bTriggerTurn)
{
	m_pOwner = pBerserker;
	m_ePreState_Id = ePreState;
	
	m_bTriggerTurn = bTriggerTurn;
	m_fTimeDletaAcc = 0;
	m_fMoveTime = ((rand() % 10000 + 6000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CWalkState::Tick(_float fTimeDelta)
{
	Find_Target();


	m_pOwner->Check_Navigation();
	
	return nullptr;
}

CBerserkerState * CWalkState::LateTick(_float fTimeDelta)
{
	//���� Ʈ���� �ڽ��� �浹��������
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		//�����ϰ�

		if (false == m_bTriggerTurn)
			return new CTrigger_TurnState(m_pOwner);

		// �� Ʈ���� �ڽ��� ��ġ �������� �̵��ϴ� ���¸� �����Ѵ�.

		else
		{
			_vector vPosition = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());
			m_pOwner->Get_Transform()->LookAt(vPosition);
			
		}
	}

	else
	{
		//m_bTriggerTurn = false;

		m_fTimeDletaAcc += fTimeDelta;

		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

		if (m_pTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDletaAcc > m_fMoveTime)
		{
			switch (rand() % 3)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);

			case 1:
				return new CHowLing_State(m_pOwner);
			case 2:
				return new CTurnR_State(m_pOwner);
			/*case 3:
				return new CIdleState(m_pOwner);*/


			default:
				break;
			}
		}
	}











	////////���� �ڵ� 
	//m_fWalkMoveTimer += fTimeDelta;

	//if (m_fWalkMoveTimer > 6.5f)
	//	switch (m_ePreState_Id)
	//	{
	//	case CBerserkerState::FIELD_STATE_ID::STATE_TURN_R:
	//		return new CHowLing_State(m_pOwner);

	//	case CBerserkerState::FIELD_STATE_ID::STATE_HOWLING:
	//		return new CTurnR_State(m_pOwner);

	//	default:
	//		break;
	//	}

	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_WALK_F);
}

void CWalkState::Exit()
{

}