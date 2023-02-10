#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Skillbutton final : public CUI_Base
{
private:
	CUI_Skillbutton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skillbutton(const CUI_Skillbutton& rhs);
	virtual ~CUI_Skillbutton() = default;


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
	static CUI_Skillbutton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }


private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_uint m_iSkill1name1 = 0;
	_uint m_iSkill1name2 = 0;
	_uint m_iSkill1name3 = 0;

	_bool m_bSkill1non = true;
	_bool m_bSkill2non = true;
	_bool m_bSkill3non = true;

}; 

END