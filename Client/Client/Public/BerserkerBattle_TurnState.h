#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_TurnState : public CBerserkerState
{
public:
	CBattle_TurnState(class CBerserker* pBerserker);

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
	_bool		m_bIsAnimationFinished = false;
	_bool		m_bSetAnim = false;
private:
	_matrix m_StartMatrix;

};
END
END