#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CBattle_PeckState : public CHawkState
{
public:
	CBattle_PeckState(class CHawk* pHawk);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_float		m_fTarget_Distance;
	_float		m_fAtkCollision_Delay = 0.f;
	_bool		m_bCollision = false;
	_bool		m_bTargetSetting = false;

	CCollider*  m_pAtkColliderCom = nullptr;

private:
	_matrix m_StartMatrix;
};
END
END