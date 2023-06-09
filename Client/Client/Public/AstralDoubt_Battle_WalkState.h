#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattleWalkState final : public CAstralDoubt_State
{
public:
	CBattleWalkState(class CAstralDoubt* pIceWolf, STATE_ID ePreState = STATE_ID::STATE_IDLE);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

	


private:
	_uint		m_iRand = 0;
	_float		m_fWalkMoveTimeAcc = 0.f;
	_float		m_fMoveTime = 0;
	_float		m_fSoundStart = -1.f;

	STATE_ID	m_ePreState_Id;
	_bool		m_Testbool;
	_bool		m_bBeamTargetOn = false;
	_bool		m_bTargetPickOn = false;
	_bool		m_b_IsTargetInsight = false;
	_bool		m_PreState_IsSpin = false;
	_bool		m_PreState_FootPress = false;
	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = nullptr;
	

};
END
END