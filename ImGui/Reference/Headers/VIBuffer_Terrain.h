#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class CTexture;
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:
	typedef struct TerrainTagDesc
	{
		_int	m_iPositionX = 0;
		_int	m_iPositionZ = 0;
		_int	m_iVerticeNumX = 10;
		_int	m_iVerticeNumZ = 10;
		_float  m_fHeight = 0;
		_bool	m_isHeightMap = false;

	}TERRAINDESC;

protected:
	CVIBuffer_Terrain(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticeX, _uint iNumVerticeZ, _float fHeight, _bool bCreateQuadTree = false);
	virtual HRESULT Initialize_Prototype(const _tchar* pHeightMapFilePath, _bool bCreateQuadTree = false);
	virtual HRESULT Initialize(void* pArg);
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut) override;
	void Set_Terrain_Shape(_float fHeight, _float fRad, _float fSharp, _float3 vPoint, _float fTimeDelta);
	void Set_Terrain_UpDown(_float fVec, _float fRad, _float fSharp, _float3 vPoint, _float fTimeDelta);
	void Set_Terrain_Buffer(TERRAINDESC TerrainDesc);
	HRESULT Save_VertexPosition(HANDLE hFile, _ulong* dwByte);
	HRESULT Load_Prototype(HANDLE hFile, _ulong& dwByte, _bool bCreateQuadTree = false);
	

public:
	void Culling(const class CTransform* pTransform);
	TERRAINDESC Get_TerrainDesc() { return m_pTerrainDesc; }
	_float3 Get_PickingLocalPos() { return m_bPickingLocalPos; }

private:
	_uint				m_iNumVerticesX = 0, m_iNumVerticesZ = 0;
	class CQuadTree*	m_pQuadTree = nullptr;
	TERRAINDESC			m_pTerrainDesc;
	vector<VTXNORTEX>	vecVertex;
	_bool				m_bCreateQuadTree = false;
	_float3				m_bPickingLocalPos = _float3(0.0f, 0.f, 0.f);
	
public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath , _bool bCreateQuadTree = false);
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumVerticeX, _uint iNumVerticeZ, _float fHeight, _bool bCreateQuadTree = false);
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HANDLE hFile, _ulong& dwByte, _bool bCreateQuadTree = false);

	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
