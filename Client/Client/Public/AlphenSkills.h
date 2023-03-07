#pragma once
#include "Bullet.h"

BEGIN(Client)
class CAlphenSkills final : public CBullet
{
public:

	enum TYPE { BOOST_1, BOOST_2 };

public:
	CAlphenSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAlphenSkills(const CAlphenSkills& rhs);
	virtual ~CAlphenSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual void Collision_Check() override;
	void Dead_Effect();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;

private:
	void Tick_Boost(_float fTimeDelta);

public:
	static CAlphenSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END