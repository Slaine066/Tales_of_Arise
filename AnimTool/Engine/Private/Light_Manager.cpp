#include "..\Public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
	if (iIndex >= m_Lights.size())
		return nullptr;
	
	auto iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();
}

void CLight_Manager::Set_LightDesc(_uint iIndex, LIGHTDESC _tLightDesc)
{
	if (iIndex >= m_Lights.size())
		return;

	auto iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	
	return (*iter)->Set_LightDesc(_tLightDesc);
}

void CLight_Manager::Erase_Light(_uint iIndex)
{
	if (iIndex >= m_Lights.size())
		return;

	_int i = 0;

	for (auto& iter = m_Lights.begin(); iter != m_Lights.end();)
	{
		if (i == iIndex)
		{
			Safe_Release(*iter);
			iter = m_Lights.erase(iter);
		}
		else
			++iter;

		++i;
	}
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight* pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;
	
	m_Lights.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Free(void)
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
