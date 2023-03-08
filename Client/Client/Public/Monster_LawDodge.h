#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)
class CMonster_LawDodge final : public CMonsterLawState
{
public:
	CMonster_LawDodge(class CMonsterLaw* pPlayer, _bool gofront = false);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;


private:
	_bool m_bbackstep = false;
	_bool m_bLookat = true;

	_bool m_bGofront = false;


};
END
END