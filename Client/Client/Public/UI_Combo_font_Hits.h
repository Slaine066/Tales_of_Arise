#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Combo_font_Hits final : public CUI_Base
{
private:
	CUI_Combo_font_Hits(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Combo_font_Hits(const CUI_Combo_font_Hits& rhs);
	virtual ~CUI_Combo_font_Hits() = default;


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
	static CUI_Combo_font_Hits* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;
};

END