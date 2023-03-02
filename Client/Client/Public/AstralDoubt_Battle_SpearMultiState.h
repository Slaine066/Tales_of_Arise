#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattle_SpearMultiState : public CAstralDoubt_State
{
public:
	CBattle_SpearMultiState(class CAstralDoubt* pAstral_Doubt, STATE_ID eState = STATE_ID::STATE_FOOTPRESS);

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
	//STATE_ID	m_eState_Id;
	STATE_ID	m_ePreTurn_Id;

	_float		m_fIdleTime;
	_float		m_fColliderStart = -1.f;
	_float		m_fAtkCollision_Delay = 0.f;
	_float		m_f2th_AtkCollision_Delay = 0.f;
	_float		m_f3th_AtkCollision_Delay = 0.f;
	_float		m_f4th_AtkCollision_Delay = 0.f;
	_float		m_f5th_AtkCollision_Delay = 0.f;
	_float		m_f6th_AtkCollision_Delay = 0.f;

	_float		m_fAtkFootCollision_Delay = 0.f;
	_float		m_f2th_AtkFootCollision_Delay = 0.f;

	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = nullptr;
	CCollider*	m_p3th_AtkColliderCom = nullptr;
	CCollider*	m_p4th_AtkColliderCom = nullptr;
	CCollider*	m_p5th_AtkColliderCom = nullptr;
	CCollider*	m_p6th_AtkColliderCom = nullptr;
	CCollider*  m_pFootColliderCom = nullptr;
	CCollider*  m_p2th_FootColliderCom = nullptr;

	_bool		m_bCausedDamage= false;
	_bool		m_bFootCauseDamage = false;
	_bool		m_bAdventSound = false;
	_bool		m_bSpearMultiEndSound = false;

	_bool		m_bCollision = false;
	_bool		m_b2th_Collision = false;
	_bool		m_b3th_Collision = false;
	_bool		m_b4th_Collision = false;
	_bool		m_b5th_Collision = false;
	_bool		m_b6th_Collision = false;

	_bool		m_bFootCollision = false;
	_bool		m_b2th_FootCollision = false;

	/* Effect Booleans */

	// Spear_HandStand
	_bool m_bHit_1 = false; 
	_bool m_bHit_2 = false;
	_bool m_bHit_3 = false;
	_bool m_bHit_4 = false;
	_bool m_bHit_5 = false;
	_bool m_bHit_6 = false;
	_bool m_bHit_7 = false;

	// Spear_Multi
	_bool m_bBullet_1 = false;
	_bool m_bBullet_2 = false;
	_bool m_bBullet_3 = false;
	_bool m_bBullet_4 = false;
	_bool m_bBullet_5 = false;
	_bool m_bBullet_6 = false;
};
END
END