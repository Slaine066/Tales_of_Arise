#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)

class CTexture;
class CNavigation;
class CVIBuffer_Terrain;

END


BEGIN(Client)

class CTerrain final : public CBaseObj
{
public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_BRUSH, TYPE_FILTER, TYPE_END };
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	HRESULT Initialize_Load(const _tchar * VIBufferTag, void * pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	CTexture*				m_pTextureCom[TYPE_END] = { nullptr };
	CNavigation*			m_pNavigationCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;

	ID3D11ShaderResourceView*			m_pFilterTexture = nullptr;
private:
	_uint					m_iPassIndex = 0;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */
	HRESULT Create_FilterTexture();


public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual CGameObject* Clone_Load(const _tchar* VIBufferTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END