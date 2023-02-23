#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_RunState : public CBerserkerState
{
public:
	CBattle_RunState(class CBerserker* pBerserker,STATE_ID ePreState = STATE_END);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;
	_bool		m_bAnimFinish = false;
	_bool		m_bTargetSetting = false;
	_bool		m_b_IsTargetInsight = false;
	_float		m_fRedayAttackTimer = 0.f;
	_float		m_fTarget_Distance;
	STATE_ID	m_ePreState_Id;

};
END
END