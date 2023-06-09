#include "..\Public\Layer.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;
	
	auto iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Find_Component(pComponentTag);
}

HRESULT CLayer::Initialize(void)
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;
	
	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject = m_GameObjects.begin(); pGameObject != m_GameObjects.end();)
	{
		if (nullptr != (*pGameObject))
			(*pGameObject)->Tick(fTimeDelta);

		if ((*pGameObject)->Get_Dead())
		{
			Safe_Release(*pGameObject);
			pGameObject = m_GameObjects.erase(pGameObject);
		}
		else
			++pGameObject;
	}
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(fTimeDelta);
	}
}

CLayer * CLayer::Create(void)
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free(void)
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	
	m_GameObjects.clear();
}
