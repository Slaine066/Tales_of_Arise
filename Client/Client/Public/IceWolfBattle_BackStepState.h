#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_BackStepState : public CIceWolfState
{
public:
	CBattle_BackStepState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fRedayAttackTimer = 0.f;
	_bool		m_bTargetSetting = false;
	_float		m_iRandTime = 0.f;
	

private:
	_matrix m_StartMatrix;
};

END
END