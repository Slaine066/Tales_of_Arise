#pragma once

/* ��. */
#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}
	// �߰�
	const char* Get_ParentName() const {
		if (nullptr == m_pParent)
			return "";
		return m_pParent->Get_Name();
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	// �߰�
	XMFLOAT4X4 Get_OffSetMatrixXM() {
		return m_OffsetMatrix;
	}
	// �߰�
	_float4x4 Get_OriTransformationMatrix() { return m_OriTransformationMatrix; }
	// �߰�
	CHierarchyNode* Get_Parent() { return m_pParent; }

	_float4x4 Get_MoveTransformationMatrix(void) { return m_MoveTransformationMatrix; }

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, CHierarchyNode* pParent);
	HRESULT Bin_Initialize(DATA_BINNODE* pNode); // �߰�
	void Invalidate_CombinedTransformationmatrix(const char* pBoneName);
	void Set_FindParent(CHierarchyNode* pNode); // �߰�

private:
	char				m_szName[MAX_PATH] = "";
	char				m_szParentName[MAX_PATH] = ""; // �߰�
	CHierarchyNode*		m_pParent = nullptr;

	/* �⺻�� �׵�, �޽þȿ���(aiBone) ���� ä���ش�. */
	_float4x4			m_OffsetMatrix;
	_float4x4			m_OriTransformationMatrix; // �߰�

	_float4x4			m_TransformationMatrix;

	/* m_TransformationMatrix * m_pParent->m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix;

	/* �̵��� �츮�� ���� ��Ʈ���� */
	_float4x4 m_MoveTransformationMatrix;

public:
	//static CHierarchyNode* Create(const aiNode* pNode, CHierarchyNode* pParent);
	static CHierarchyNode* Create(HANDLE hFile, _ulong* pdwByte, CHierarchyNode* pParent);
	static CHierarchyNode* Bin_Create(DATA_BINNODE* pNode); // �߰�
	virtual void Free() override;
};

END