#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Screen final : public CUI_Base
{
private:
	CUI_Screen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Screen(const CUI_Screen& rhs);
	virtual ~CUI_Screen() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

public:
	static CUI_Screen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END