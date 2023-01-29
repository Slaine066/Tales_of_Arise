#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

void CHierarchyNode::Set_TransformationMatrix(_fmatrix TransformationMatrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
}

HRESULT CHierarchyNode::Initialize(BINBONE tBone, CHierarchyNode* pParent)
{
	memcpy(&m_szName, tBone.szName, sizeof(char) * MAX_PATH);

	/* m_OffsetMatrix ���� ���ÿ������� ������ ���ÿ������� ��ȯ���ֱ����� ���.  */
	/* ���߿� ä�ﲨ�ϱ� �ϴ� �׵����� �ʱ�ȭ. */
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	/* ������ �ٲ� �� �ִ�. �ִϸ��̼��� ����Ǹ�. */
	/* m_TransformationMatrix : �� ������ ����. (�θ�������� ǥ����) (�θ��� ���¸� ���ܵ�) */
	m_TransformationMatrix = tBone.Transformation;
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	// ���� ��ȯ ��� ����
	m_PreTransforamtionMatrix = XMLoadFloat4x4(&m_TransformationMatrix);

	/* m_CombinedTransformationMatrix : �� �� + �θ��� ����. (������������ ǥ����) (�θ��� ���¸� ������) */
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	/* �ڽ��� �ּҸ� ������.(x)
	�θ��� �ּҸ� ������? : ���� m_CombinedTransformationMatrix�� ��������ؼ��� �θ��� m_CombinedTransformationMatrix�� �ʿ��߱� ����.
	�׷��� �θ��� �ּҰ� �ʿ���. */
	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

HRESULT CHierarchyNode::Bin_Initialize(DATA_BINNODE * pNode)
{
	strcpy_s(m_szName, pNode->cName);
	strcpy_s(m_szParentName, pNode->cParent);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	if (pNode->mTransform._11 == -431602080.)
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixIdentity());
	else
		memcpy(&m_TransformationMatrix, &pNode->mTransform, sizeof(_float4x4));


	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CHierarchyNode::Invalidate_CombinedTransformationmatrix(const char* pBoneName)
{
	Set_RootMotionMatrix(pBoneName);

	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;
}

void CHierarchyNode::Set_FindParent(CHierarchyNode * pNode)
{
	const char* pName = pNode->Get_Name();
	if (!strcmp(m_szParentName, pName))
	{
		m_pParent = pNode;
		Safe_AddRef(m_pParent);
		return;
	}
	m_pParent = nullptr;
}

//CHierarchyNode * CHierarchyNode::Create(const aiNode* pNode, CHierarchyNode* pParent)
//{
//	CHierarchyNode*		pInstance = new CHierarchyNode();
//
//	if (FAILED(pInstance->Initialize(pNode, pParent)))
//	{
//		ERR_MSG(TEXT("Failed to Created : CTransform"));
//		Safe_Release(pInstance);
//	}
//	return pInstance;	
//}

void CHierarchyNode::Set_RootMotionMatrix(const char* pBoneName)
{
	// ��Ʈ �� �̸� �񱳷� ã��
	if ((nullptr != pBoneName) && !strcmp(m_szName, pBoneName))
	{
		_matrix AfterTransformMatrix = XMLoadFloat4x4(&m_TransformationMatrix);

		// ���� Transformation���� ���� Transformation���� �̵� ��ȭ��
		m_fMoveTransformationLength = XMVectorGetX(XMVector4Length(AfterTransformMatrix.r[3] - m_PreTransforamtionMatrix.r[3]));

		_vector vPreQuat, vAfterQuat;
		vPreQuat = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_PreTransforamtionMatrix));
		vAfterQuat = XMQuaternionNormalize(XMQuaternionRotationMatrix(AfterTransformMatrix));

		// ���ʹϾ� ������ ���� �� ���ʹϾ� ���� �� ����
		m_fRotationRadian = acosf(XMVectorGetX(XMQuaternionDot(vPreQuat, vAfterQuat))) * -2.f;

		// ���� ��ȯ ��� ����
		m_PreTransforamtionMatrix = AfterTransformMatrix;

		// ��Ʈ �� �̵� �� ����
		AfterTransformMatrix = XMMatrixIdentity();
		XMStoreFloat4x4(&m_TransformationMatrix, AfterTransformMatrix);
	}
}

CHierarchyNode * CHierarchyNode::Create(BINBONE tBone, CHierarchyNode * pParent)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(tBone, pParent)))
	{
		ERR_MSG(TEXT("Failed to Created : CHierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode * CHierarchyNode::Bin_Create(DATA_BINNODE * pNode)
{
	CHierarchyNode*			pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Bin_Initialize(pNode)))
	{
		ERR_MSG(TEXT("Failed To Created : CHierarchyNode_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}