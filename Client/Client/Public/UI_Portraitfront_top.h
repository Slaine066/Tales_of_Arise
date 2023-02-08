#pragma once

#include "UI_Portraitfront.h"


BEGIN(Client)

class CUI_Portraitfront_top final : public CUI_Portraitfront
{
private:
	CUI_Portraitfront_top(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Portraitfront_top(const CUI_Portraitfront_top& rhs);
	virtual ~CUI_Portraitfront_top() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Glow();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CUI_Portraitfront_top* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;





	
};

END