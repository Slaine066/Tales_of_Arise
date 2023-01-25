#pragma once

#include "UIBase.h"


BEGIN(Client)


class CUI_LOCKON final : public CUI_Base
{
private:
	CUI_LOCKON(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LOCKON(const CUI_LOCKON& rhs);
	virtual ~CUI_LOCKON() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

									 //CVIBuffer_Point*			m_pVIBufferCom1 = nullptr;

private:
	_float4 m_vRight;
	_float4 m_vUp;

public:
	static CUI_LOCKON* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_itexnum = 0;
	_float m_fcurrentmp = 0.f;
	_float m_fmaxmp = 4.f;

	_float m_fNext = 50.f;


	_float m_fcurrent_render_slot_mp = 0.f;



};

END