#include "..\Public\VIBuffer_Terrain.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "Transform.h"


CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;
	_ulong*					pPixel = nullptr;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	pPixel = new _ulong[m_iNumVerticesX * m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3(_float(j), (pPixel[iIndex] & 0x000000ff) / 10.0f , _float(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexture = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;

	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFaces = 0;
	
	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1, 
				iIndex
			};

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];

			_vector		vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	
	/* ������ ��� ���� ������ �Ҵ��ϰ�, ���� �������� �迭�� ������ ��ī���Ѵ�. */
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۸� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* �������۸� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* ������ ��� ���� ������ �Ҵ��ϰ�, ���� �������� �迭�� ������ ��ī���Ѵ�. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
#pragma endregion

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

	_uint		iLT = m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX;
	_uint		iRT = m_iNumVerticesX * m_iNumVerticesZ - 1;
	_uint		iRB = m_iNumVerticesX - 1;
	_uint		iLB = 0;

	m_pQuadTree = CQuadTree::Create(iLT, iRT, iRB, iLB);
	if (nullptr == m_pQuadTree)
		return E_FAIL;

	m_pQuadTree->SetUp_Neighbors();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::Culling(const CTransform * pTransform)
{
	CFrustum*		pFrustum = GET_INSTANCE(CFrustum);

	_matrix			WorldMatrix = pTransform->Get_WorldMatrix();
	
	pFrustum->Transform_ToLocalSpace(WorldMatrix);

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(SubResource));

	m_pContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	FACEINDICES32*		pIndices = (FACEINDICES32*)SubResource.pData;


	_uint				iNumFaces = 0;

	m_pQuadTree->Culling(pFrustum, m_pVerticesPos, XMMatrixInverse(nullptr, WorldMatrix), pIndices, &iNumFaces);

	//for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	//{
	//	for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
	//	{
	//		_uint		iIndex = i * m_iNumVerticesX + j;

	//		_uint		iIndices[4] = {
	//			iIndex + m_iNumVerticesX,
	//			iIndex + m_iNumVerticesX + 1,
	//			iIndex + 1,
	//			iIndex
	//		};

	//		_bool		isIn[4] = {
	//			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&m_pVerticesPos[iIndices[0]]), 0.f),
	//			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&m_pVerticesPos[iIndices[1]]), 0.f),
	//			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&m_pVerticesPos[iIndices[2]]), 0.f),
	//			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&m_pVerticesPos[iIndices[3]]), 0.f)
	//		};

	//		/* ������ �� �ﰢ���� �׷�������? */
	//		if (true == isIn[0] &&
	//			true == isIn[1] &&
	//			true == isIn[2])
	//		{
	//			pIndices[iNumFaces]._0 = iIndices[0];
	//			pIndices[iNumFaces]._1 = iIndices[1];
	//			pIndices[iNumFaces]._2 = iIndices[2];
	//			iNumFaces++;

	//		}

	//		/* ���� �Ʒ� �ﰢ���� �׷�������? */
	//		if (true == isIn[0] &&
	//			true == isIn[2] &&
	//			true == isIn[3])
	//		{
	//			pIndices[iNumFaces]._0 = iIndices[0];
	//			pIndices[iNumFaces]._1 = iIndices[2];
	//			pIndices[iNumFaces]._2 = iIndices[3];
	//			iNumFaces++;
	//		}
	//	}
	//}

	m_pContext->Unmap(m_pIB, 0);

	m_iNumPrimitive = iNumFaces;

	RELEASE_INSTANCE(CFrustum);

}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
	
	Safe_Release(m_pQuadTree);
}
