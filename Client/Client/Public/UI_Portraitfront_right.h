#pragma once

#include "UI_Portraitfront.h"


BEGIN(Client)

class CUI_Portraitfront_right final : public CUI_Portraitfront
{
private:
	CUI_Portraitfront_right(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Portraitfront_right(const CUI_Portraitfront_right& rhs);
	virtual ~CUI_Portraitfront_right() = default;


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
	static CUI_Portraitfront_right* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;






};

END