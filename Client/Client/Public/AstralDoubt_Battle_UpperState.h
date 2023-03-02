#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattle_UpperState : public CAstralDoubt_State
{
public:
	CBattle_UpperState(class CAstralDoubt* pAstral_Doubt, STATE_ID ePreState = STATE_ID::STATE_IDLE);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Reset_Effect();

private:
	//_float			m_fTimeDeltaAcc = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	STATE_ID		m_ePreState_Id;
	STATE_ID		m_ePreTurn_Id;

	_float			m_fIdleTime;
	_float		m_fSoundStart = -1.f;

	_float		m_fAtkCollision_Delay = 0.f;
	_float		m_f2th_AtkCollision_Delay = 0.f;
	_float		m_f3th_AtkCollision_Delay = 0.f;
	_float		m_f4th_AtkCollision_Delay = 0.f;
	_float		m_f5th_AtkCollision_Delay = 0.f;
	_float		m_f6th_AtkCollision_Delay = 0.f;

	_bool		m_bCollision = false;
	_bool		m_b2th_Collision = false;
	_bool		m_b3th_Collision = false;
	_bool		m_b4th_Collision = false;
	_bool		m_b5th_Collision = false;
	_bool		m_b6th_Collision = false;


	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = nullptr;
	CCollider*	m_p3th_AtkColliderCom = nullptr;
	CCollider*	m_p4th_AtkColliderCom = nullptr;
	CCollider*	m_p5th_AtkColliderCom = nullptr;
	CCollider*	m_p6th_AtkColliderCom = nullptr;

	/* Effect Booleans */
	_bool m_bSlash_1 = false;
	_bool m_bSlash_2 = false;
	_bool m_bSlash_3 = false;
};
END
END