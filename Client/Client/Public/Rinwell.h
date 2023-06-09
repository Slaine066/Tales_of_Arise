#pragma once
#include "Player.h"

BEGIN(Client)
class CRinwell final : public CPlayer
{	
public:
	enum ANIM {
		BTL_ADVENT,
		BTL_ARISE_B,
		BTL_ARISE_F,
		BTL_ATTACK_BRAVE,
		BTL_ATTACK_DENGEKISYOUHEKI,
		BTL_ATTACK_DENZIHOU,
		BTL_ATTACK_FLASH,
		BTL_ATTACK_FUATU,
		BTL_ATTACK_FUZIN,
		BTL_ATTACK_HIEN,
		BTL_ATTACK_HOUDEN,
		BTL_ATTACK_HYOUROU,
		BTL_ATTACK_MIZUTAMARI_END,
		BTL_ATTACK_MIZUTAMARI_LOOP,
		BTL_ATTACK_MIZUTAMARI_START,
		BTL_ATTACK_NORMAL_0,
		BTL_ATTACK_NORMAL_1,
		BTL_ATTACK_NORMAL_2,
		BTL_ATTACK_NORMAL_3,
		BTL_ATTACK_NORMAL_AIR_0,
		BTL_ATTACK_NORMAL_AIR_1,
		BTL_ATTACK_NORMAL_AIR_2,
		BTL_ATTACK_SENKOU,
		BTL_ATTACK_STRIKE,
		BTL_ATTACK_STRIKE_AIR,
		BTL_ATTACK_SUIHOU,
		BTL_ATTACK_TUMUZIKAZE,
		BTL_DAMAGE_AIR_LARGE_B,
		BTL_DAMAGE_AIR_LARGE_F,
		BTL_DAMAGE_AIR_LARGE_L,
		BTL_DAMAGE_AIR_LARGE_R,
		BTL_DAMAGE_AIR_LOOP,
		BTL_DAMAGE_AIR_SMALL_B,
		BTL_DAMAGE_AIR_SMALL_F,
		BTL_DAMAGE_AIR_SMALL_L,
		BTL_DAMAGE_AIR_SMALL_R,
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

		DASH,
		DASH_BRAKE_000,
		DASH_BRAKE_001,
		IDLE,
		IDLE_SIDE,
		IDLE_TRANS_WALK,
		JUMP_LANDING,
		JUMP_LANDING_HIGH,
		JUMP_LOOP,
		JUMP_RUN_END,
		JUMP_RUN_LANDING,
		JUMP_RUN_LOOP,
		JUMP_RUN_START,
		JUMP_START,
		LADDER_DOWN_END,
		LADDER_DOWN_LOOP,
		LADDER_DOWN_START,
		LADDER_DOWN_TRANS_LADDER_IDLE,
		LADDER_IDLE,
		LADDER_TRANS_JUMP_RUN_LOOP,
		LADDER_UP_END,
		LADDER_UP_LOOP,
		LADDER_UP_START,
		LADDER_UP_TRANS_LADDER_IDLE,
		RUN,
		RUN_BRAKE_000,
		RUN_BRAKE_001,
		TOUCH_STAND,
		TREASURE_OPEN,
		WALK,
		WALK_TRANS_IDLE_000,
		WALK_TRANS_IDLE_001,


		BTL_STEP_AIR,
		BTL_STEP_LAND,
		BTL_STEP_LAND_BACK,

		BTL_ATTACK_MAGIC_EMIT_AIR,
		BTL_ATTACK_MAGIC_EMIT,
		BTL_ATTACK_MAGIC_STACK,
		BTL_MAGIC_EMIT,
		BTL_MAGIC_END,
		BTL_MAGIC_LOOP,
		BTL_MAGIC_START,
		BTL_MAGIC_WALK_B,
		BTL_MAGIC_WALK_F,
		BTL_MAGIC_WALK_L,
		BTL_MAGIC_WALK_R,

		BTL_MOVE_IDLE,
		BTL_MOVE_RUN,
		BTL_LAND,
		SYS_DOOR_OPEN_HAND_RIGHT,
		IDLE_TO_IDLE_SIDE,
		BOUI,
		ANIM_SIONRINWELL_STRIKE,
		ANIM_ALPHENRINWELL_STRIKE

	};

private:
	CRinwell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRinwell(const CRinwell& rhs);
	virtual ~CRinwell() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public: /*For.State*/
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;

private:
	virtual HRESULT Ready_Parts() override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Change_Level(LEVEL eLevel) override;
	HRESULT SetUp_ShaderResources();

public:
	static CRinwell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END