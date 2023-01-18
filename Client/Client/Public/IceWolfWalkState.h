#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CWalkFrontState final : public CHawkState
{
public:
	CWalkFrontState(class CIce_Wolf* pIceWolf);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;



private:
	_float m_fWalkMoveTimer = 0.f;
	_float m_fWalkAttackTimer = 1.5f;
	_uint  m_iRand = 0;
};
END
END