#pragma once
#include "Bullet.h"

BEGIN(Client)
class CRinwellSkills final : public CBullet
{
public:
	enum TYPE { PHOTON_FLASH, GALE_FORCE , METEOR , THUNDER_FIELD , DIVINE_SABER , DIVINE_SABER_BULLET};

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

private:
	vector<CEffect*> m_pBlastEffects;
	vector<CEffect*> m_pBlast2Effects;

public:
	static CRinwellSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_float m_fThunderStopTimer = 0.f;

	_int bulletcount = 40;
	_float m_fDivineTimer = 0.f;
	_int m_fDivineOffset = 70.f;

};

END