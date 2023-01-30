#pragma once

#include "SlimeState.h"

BEGIN(Client)
BEGIN(Slime)
class CWalkState : public CSlimeState
{
public:
	CWalkState(class CSlime* pIceWolf);

	virtual CSlimeState* AI_Behaviour(_float fTimeDelta) override;
	virtual CSlimeState* Tick(_float fTimeDelta) override;
	virtual CSlimeState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;
	_float		m_fTarget_Distance;
	_float		m_fWalkMoveTimer = 0.f;
private:
	_matrix m_StartMatrix;
};
END
END