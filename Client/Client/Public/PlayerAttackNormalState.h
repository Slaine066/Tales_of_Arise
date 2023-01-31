#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)

class CAttackNormalState final : public CPlayerState
{
public:
	CAttackNormalState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_matrix m_StartMatrix;

	_bool m_bEffectSlashSpawned = false;
};
END
END