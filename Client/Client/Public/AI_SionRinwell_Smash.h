#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_SionRinwell_Smash final : public CAIState
{
public:
	CAI_SionRinwell_Smash(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;

	_float m_fTime = 0.f;
	_float m_fFadeTime = 0.f;
	_float m_fShakingTime = 0.f;
	_bool  m_bBullet = false;

	_bool m_bAlphenStrike_1 = false;
	_bool m_bAlphenStrike_2 = false;

	_float m_fEventStart = -1.f;
	_bool m_bShaking = false;
};
END
END