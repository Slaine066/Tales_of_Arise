#pragma once

#include "UIBase.h"
#include "BaseObj.h"


BEGIN(Client)

class UI_Damagefont_BoostBreak final : public CUI_Base
{
private:
	UI_Damagefont_BoostBreak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	UI_Damagefont_BoostBreak(const UI_Damagefont_BoostBreak& rhs);
	virtual ~UI_Damagefont_BoostBreak() = default;


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
	static UI_Damagefont_BoostBreak* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

	//wchar_t* CharToWChar(const char* pstrSrc);

public:


	void Open_sysmsg(_uint index);//, _uint index1);l
								  //void Close_sysmsg() { m_bInterect = false; }


private:
	CTexture*				m_pTextureCom1 = nullptr;

	CBaseObj* m_pPointer = nullptr;

	_bool m_bfadein = false;
	_bool m_bfadein1 = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fFadeX = -50.f;
	_float m_fFadeY = -20.f;
	_float m_fFade1X = -50.f;
	_float m_fFade1Y = -20.f;


	_float m_fLevelupAlpha = 0.f;
	_float m_fLineAlpha = 1.f;
	_float m_fLineYScaler = 2.5f;
	_float m_fLineXScaler = 0.1f;
	_bool m_fLevelupAlphaup = true;
	_bool m_bLinemove = true;
	_bool m_bLinexup = true;
	_bool m_bLinexdown = false;
	_float m_fDietimer = 0.f;
	_bool m_bDeadtimeron = false;

	_float m_fIndex = 0;



};

END