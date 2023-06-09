#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CLawAirFSkillState final : public CPlayerState
{
public:
	CLawAirFSkillState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Update_Skill(void);
	void Remove_Skill(void);
	void Reset_Skill(void);

private:
	CCollider* m_pLandCollider = nullptr;

	_matrix m_ColliderMatrix = XMMatrixIdentity();

	/* Effect Booleans */
	_bool m_bEnhaBakusaiken_1 = false;	// F (Air)
	_bool m_bEnhaBakusaiken_2 = false;	// F (Air)

	/* Effect Vectors */
	vector<class CEffect*> m_EnhaBakusaiken_1;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);

	HITLAGDESC m_HitLagDesc;


	//Sound
	_bool m_bSkill_F_Sound = false;




};
END
END