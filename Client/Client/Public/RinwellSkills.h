#pragma once
#include "Bullet.h"

BEGIN(Client)
class CRinwellSkills final : public CBullet
{
public:
	enum TYPE { PHOTON_FLASH, GALE_FORCE , METEOR , METEORDEAD,
		THUNDER_FIELD , DIVINE_SABER , DIVINE_SABER_BULLET , 
		HOLY_RANCE , HOLY_RANCE_BULLET , HOLY_RANCE_LASTBULLET, HOLY_RANCE_FISRTBULLET,
		BANGJEON, BANGEONDEAD};

public:
	CRinwellSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRinwellSkills(const CRinwellSkills& rhs);
	virtual ~CRinwellSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual void Collision_Check() override;
	void Dead_Effect();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;

private:
	void Tick_PhotonFlash(_float fTimeDelta);
	void Tick_GaleForce(_float fTimeDelta);
	void Tick_Meteor(_float fTimeDelta);
	void Tick_ThunderField(_float fTimeDelta);
	void Tick_DivineSaber(_float fTimeDelta);
	void Tick_DivineSaberBullet(_float fTimeDelta);
	void Tick_HolyRance(_float fTimeDelta);
	void Tick_HolyRanceBullet(_float fTimeDelta);
	void Tick_HolyRanceBullet2(_float fTimeDelta);
	void Tick_BangJeon(_float fTimeDelta);

private:
	vector<CEffect*> m_pBlastEffects;
	vector<CEffect*> m_pBlast2Effects;

public:
	static CRinwellSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_float m_fThunderStopTimer = 0.f;

	_bool	m_bFirst = false;
	_bool	m_bBullet = false;
	_int bulletcount = 60;
	_float m_fDivineTimer = 0.f;
	_int m_fDivineOffset = 70;
	_float m_fholytimer = 0.f;

	_bool m_bholybullet[7] = { true, true, true , true , true ,  true , true };
	
	LIGHTDESC OriginLightDesc;
	LIGHTDESC DevineLightDesc;
	
	_float3	  LightOffset = _float3(0.f, 0.f, 0.f);

	//Sound
	_bool	m_bMeteorSound = false;
	_bool	m_bThunder_Field = false;

	//����μ��̹� ����
	_bool	m_bDivineBullet0_Sound = false;
	_bool	m_bDivineBullet1_Sound = false;
	_bool	m_bDivineBullet2_Sound = false;
	_bool	m_bDivineBullet3_Sound = false;

	//Ȧ������ ����
	_bool	m_bHoly_RanceGround_Sound = false;
	_bool	m_bHoly_RanceFirstBullet_Sound = false;
	_bool	m_bHoly_RanceBullet0_Sound = false;
	_bool	m_bHoly_RanceBullet1_Sound = false;
	_bool	m_bHoly_RanceBullet2_Sound = false;
	_bool	m_bHoly_RanceLastBullet_Sound = false;
};

END