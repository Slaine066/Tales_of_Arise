#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CHawk final : public CMonster
{
public:
	enum ANIM {

		ARI00,   //3D뷰어에서 1번 
		FLAPPING,
		ATTACK_ROTATION,
		ATTACK_ROTATION2,//4
		ATTACK_CHARGE,//5
		ATTACK_CHARGE_ROTATION, //6
		FLYING_BACK,//7
		ATTACK_GRAB_START1, //8
		ATTACK_GRAB_START2, //9//스타트로 쓰기엔 동작이 너무 짧음.
		REVOLVING_FLIGHT, //10
		ATTACK_GRAB_END,  //11
		ATTACK_GRAB_LOOP2, //12
		HANGING_INTHE_AIR, //13
		ATTACK_BOMBING_DUP, //6//ATTACK1
		ATTACK_BRAVE, //7
		ATTACK_CHARGE2, //8
		ATTACK_DASH, //9
		ATTACK_FLUTTER,
		ATTACK_GRAB_END2,
		ATTACK_GRAB_LOOP3, //12
		ATTACK_GRAB_START3, //13
		ATTACK_PECK,
		ATTACK_TORNADE, //15
		BLOW_DOWN_B,
		BLOW_DOWN_F,
		BLOW_UP_B,
		BLOW_UP_F,
		DAMAGE_AIR_LARGE_B,
		DAMAGE_AIR_LARGE_F,
		DAMAGE_AIR_LARGE_L,
		DAMAGE_AIR_LARGE_R,
		DAMAGE_AIR_LOOP,
		DAMAGE_AIR_SMALL_B,
		DAMAGE_AIR_SMALL_F,
		DAMAGE_AIR_SMALL_L,
		DAMAGE_AIR_SMALL_R,
		DAMAGE_LARGE_B,
		DAMAGE_LARGE_F,
		DAMAGE_LARGE_L,
		DAMAGE_LARGE_R,
		DAMAGE_LOOP,
		DAMAGE_SMALL_B,
		DAMAGE_SMALL_F,
		DAMAGE_SMALL_L,//33 피격모션 
		DAMAGE_SMALL_R,//34 피격모션 
		DEAD, 
		FAST_FLAP_OF_WINGS,//47
		DOWN_B,//35 DEAD
		DOWN_F,//36 DEAD
		DOWN_UNIQUE,
		MOVE_IDLE,//38공중에서 멈춰있기
		MOVE_RUN,//39빨리날기
		MOVE_WALK_B,//40천천히 날기
		MOVE_WALK_F,//41천천히 날기
		TURN_L, //42
		TURN_POSE,
		TURN_R, //43
		SYMBOL_DETECT_IDLE,//44
		SYMBOL_DETECT_STOP,//45 앉아있다 날아오르기
		SYMBOL_IDLE,
		SYMBOL_RUN,//47
		SYMBOL_STOP,//x
		SYMBOL_TURN_LEFT, //49x존재하지 않음 
		SYMBOL_TURN_POSE,//x존재하지 않음 
		SYMBOL_TURN_RIGHT,//x존재하지 않음 . 50
		SYMBOL_WALK, 
		ADVENT, //x존재하지 않음 52 멀리서 등장 
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CHawkState* Get_State() { return m_pHawkState; }
	void Set_PlayerState(class CHawkState* pHawkState) { m_pHawkState = pHawkState; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _float Take_Damage(float fDamage, CBaseObj* DamageCauser) override;
	
private:
	CHawk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHawk(const CHawk& rhs);
	virtual ~CHawk() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);
		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CHawkState*  m_pHawkState = nullptr;

public:
	static CHawk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END