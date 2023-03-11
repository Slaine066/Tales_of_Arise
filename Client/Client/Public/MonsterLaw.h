#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CMonsterLawState;
class CMonsterLaw final : public CMonster
{
public:
	enum ANIM {
		BTL_ADVENT,

		BTL_ARISE_B,
		BTL_ARISE_F,

		BTL_ATTACK_BRAVE,
		BTL_ATTACK_BURN_KNUCKLE,
		BTL_ATTACK_ENHABAKUSAIKEN_END,
		BTL_ATTACK_ENHABAKUSAIKEN_LOOP,
		BTL_ATTACK_ENHABAKUSAIKEN_START,
		BTL_ATTACK_FLASH,
		BTL_ATTACK_FLASH_AIR,
		BTL_ATTACK_FLASH_LOOP,
		BTL_ATTACK_GARENKYAKU_START,
		BTL_ATTACK_GAROUHOUKO,
		BTL_ATTACK_HIENRENTENKYAKU,
		BTL_ATTACK_MUEISYOU,
		BTL_ATTACK_NORMAL_0,
		BTL_ATTACK_NORMAL_1,
		BTL_ATTACK_NORMAL_2,
		BTL_ATTACK_NORMAL_3,
		BTL_ATTACK_NORMAL_4,
		BTL_ATTACK_NORMAL_AIR_0,
		BTL_ATTACK_NORMAL_AIR_1,
		BTL_ATTACK_NORMAL_AIR_2,
		BTL_ATTACK_NORMAL_AIR_3,
		BTL_ATTACK_NORMAL_AIR_4,
		BTL_ATTACK_RENGADAN,
		BTL_ATTACK_SANKAMOUSYUUKYAKU_END,
		BTL_ATTACK_SANKAMOUSYUUKYAKU_LOOP,
		BTL_ATTACK_SANKAMOUSYUUKYAKU_START,
		BTL_ATTACK_STRIKE,
		BTL_ATTACK_STRIKE_AIR,
		BTL_ATTACK_STRIKE_AIR_SUB,
		BTL_ATTACK_STRIKE_START,
		BTL_ATTACK_STRIKE_SUB,
		BTL_ATTACK_TENKETUBAKUTAI,
		BTL_ATTACK_TYOURENGADAN,
		BTL_ATTACK_UKIGUMOHUUGA_START,

		BTL_BLOW_DOWN_F,

		BTL_DAMAGE_AIR_LARGE_B,
		BTL_DAMAGE_AIR_LARGE_F,
		BTL_DAMAGE_AIR_LARGE_L,
		BTL_DAMAGE_AIR_LARGE_R,
		BTL_DAMAGE_AIR_LOOP,
		BTL_DAMAGE_AIR_SMALL_B,
		BTL_DAMAGE_AIR_SMALL_F,
		BTL_DAMAGE_AIR_SMALL_L,
		BTL_DAMAGE_LARGE_B,
		BTL_DAMAGE_LARGE_F,
		BTL_DAMAGE_LARGE_L,
		BTL_DAMAGE_LARGE_R,
		BTL_DAMAGE_LOOP,
		BTL_DAMAGE_SMALL_B,
		BTL_DAMAGE_SMALL_F,
		BTL_DAMAGE_SMALL_L,
		BTL_DAMAGE_SMALL_R,

		BTL_DEAD,
		BTL_DOWN_B,
		BTL_DOWN_F,
		BTL_FALL,

		BTL_GUARD_HIT,
		BTL_GUARD_LOOP,
		BTL_GUARD_START,
		BTL_GUARD_WALK_B,
		BTL_GUARD_WALK_F,
		BTL_GUARD_WALK_L,
		BTL_GUARD_WALK_R,

		BTL_ITEM,
		BTL_JUMP,
		BTL_LAND,

		BTL_MOVE_BRAKE,
		BTL_MOVE_IDLE,
		BTL_MOVE_RUN,
		BTL_MOVE_WALK_B,
		BTL_MOVE_WALK_F,
		BTL_MOVE_WALK_L,
		BTL_MOVE_WALK_R,

		BTL_MYSTIC_GURENTENSYOU,
		BTL_MYSTIC_ZINRAIROUEIKYAKU,

		BTL_STEP_AIR,
		BTL_STEP_LAND,
		BTL_STEP_LAND_BACK,

		DASH,
		DASH_BREAK_START,
		DASH_BREAK_END,

		DOOR_OPEN_BOTH_HAND,
		DOOR_OPEN_RIGHT_HAND,

		IDLE,
		IDLE_TRANS_WALK,

		JUMP_LANDING,
		JUMP_LANDING_DAMAGE,
		JUMP_LANDING_HIGH,
		JUMP_LOOP,
		JUMP_START,
		JUMP_RUN_END,
		JUMP_RUN_LANDING,
		JUMP_RUN_LOOP,
		JUMP_RUN_START,

		RUN,
		RUN_BRAKE,

		TREASURE_OPEN,

		WALK,
		WALK_TRANS_IDLE_START,
		WALK_TRANS_IDLE_END,

		IDLE_SIDE,
		IDLE_TRANS_IDLE_SIDE,

		BTL_ATTACK_RONDSENPU,
		BTL_ATTACK_GARYOUKUUHA,
		BTL_ATTACK_EISYOUTAI,
		ANIM_SIONLAW_STRIKE
	};

	enum SKILL_TYPE
	{
		NORMALATTACK_1,
		NORMALATTACK_2,
		NORMALATTACK_3,
		NORMALATTACK_4,
		NORMALATTACK_5,
		PHOTONFLASH,
		GALEFORCE,
		CROSSBLADE,
		AIR_DUST,
		SKILL_END
	};

public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CMonsterLawState* Get_State() { return m_pState; }
	_uint Get_SkillIndex() { return m_eSkillIndex; }
	void Set_PlayerState(class CMonsterLawState* pPlayerState) { m_pState = pPlayerState; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void Set_SkillIndex(_uint iIndex) { m_eSkillIndex = iIndex; }
	virtual void	 Set_BattleMode(_bool type) override;
	virtual void	 Set_HitState() override;


	/* Air Check*/
	_bool			Get_IsFly(void) { return m_bIsFly; }
	void			On_IsFly(void) { m_bIsFly = true; }
	void			Off_IsFly(void) { m_bIsFly = false; }


public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	CMonsterLaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterLaw(const CMonsterLaw& rhs);
	virtual ~CMonsterLaw() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);
	void Battle_Animation(_float fTimeDelta);
	_bool Get_NpcMode() { return m_bNpcMode; }

		/*For Navigation*/
	virtual void Check_Navigation() override;
    _bool Check_Navigation_Jump();

	void Set_AfterThunder(_bool tof) { m_bAfterThunder = tof; }
	void Set_AfterKick(_bool tof) { m_bAfterKick = tof; }

	_int Get_Phase() { return m_iPhase; }
	void Set_Phase(_int phase) { m_iPhase = phase; }


	void Reset_StrikeBlur(_float fTimeDelta);

	_bool Get_Debug() { return m_bDebug; }
	void Set_Debug(_bool tof) { m_bDebug = tof; }

	_bool Get_EventFinish() { return m_bEventFinish; }
	void Set_EventFinish(_bool tof) { m_bEventFinish = tof; }

	void Set_Daguri(_bool tof) { m_bDaguri = tof; }
	_bool Get_Daguri() { return m_bDaguri; }

	_bool Get_doStrike1() { return m_bdoStrike1; }
	void Set_doStrike1(_bool tof) { m_bdoStrike1 = tof; }

	_bool Get_doStrike2() { return m_bdoStrike2; }
	void Set_doStrike2(_bool tof) { m_bdoStrike2 = tof; }

	

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CMonsterLawState*  m_pState = nullptr;

	_float   m_fSpeed = 3.f;
	_uint	 m_eAnim = IDLE_SIDE;
	_uint	 m_ePreAnim = IDLE_SIDE;

	_bool	m_bMotion_Finished = false;
	_bool	m_bNpcMode = false;
	_uint	 m_eSkillIndex = SKILL_END;

	_bool m_bIsFly = false;

	_int m_iLawhitcount = 0;

	_bool m_bAfterThunder = false;
	_bool m_bAfterKick = false;

	_float m_fThunderHitTime = 0.f;
	_float m_fKickHitTIme = 0.f;

	_int m_iPhase = 0;


	_bool m_bDebug = true;

	_bool m_bEventFinish = false;
	_bool m_bDaguri = false;
	_bool m_bReadytoChangeTarget = false;


	_float m_fFinalTimer = 0.f;
	_bool m_fFinalStrikeOnetime = true;

	
private:
		/* Strike Screen Effect */
		_bool m_bResetStrikeBlur = false;
		_float m_fStrikeBlurResetDuration = .45f;
		_float m_fStrikeBlurResetTimer = 0.f;
		_float m_fRandomTargetTimer = 0.f;

		_bool m_bdoStrike1 = false;

		_bool m_bdoStrike2 = false;
public:
	static CMonsterLaw* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END