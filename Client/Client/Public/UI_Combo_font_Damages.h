#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Combo_font_Damages final : public CUI_Base
{
private:
	CUI_Combo_font_Damages(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Combo_font_Damages(const CUI_Combo_font_Damages& rhs);
	virtual ~CUI_Combo_font_Damages() = default;


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
	static CUI_Combo_font_Damages* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void updatedmg() {m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;m_bmoveleft = true; m_bfadein = true;	m_fAlpha = 0; m_fDietimer = 0.f;
	m_bfadeout = false;}

	void moveleft() { m_fPosition.x -= 10.f; }


private:
	CTexture*				m_pTextureCom1 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fDietimer = 0.f;
};

END