#include "stdafx.h"
#include "..\Public\Camera_Manager.h"
#include "GameInstance.h"
#include "Model.h"
#include "GameObject.h"
#include "PipeLine.h"



IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{

}

HRESULT CCamera_Manager::Add_Camera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera_Action* pCameraAction = nullptr;
	CCamera_Action::ACTIONCAMDESC	ActionCamDesc;
	ZeroMemory(&ActionCamDesc, sizeof(CCamera_Action::ACTIONCAMDESC));

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Action"), LEVEL_GAMEPLAY, TEXT("Layer_Camera"), nullptr)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CCamera_Manager::Remove_Camera(_int iIndex)
{
	m_pCurrentCamera->Remove_Camdata(iIndex);
}


void CCamera_Manager::Clear_Camreras()
{
	m_iCamIndex = 0;
	Safe_Release(m_pCurrentCamera);
	m_pCurrentCamera = nullptr;
}

void CCamera_Manager::Set_CameraSymbolPosition(CAMERASYMBOL eSymbol, _vector vPosition)
{
	if (m_pClickedSymbol[eSymbol] == nullptr)
		return;

	m_pClickedSymbol[eSymbol]->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

HRESULT CCamera_Manager::Add_ClickedSymbol(_float4 vClickPos, CAMERASYMBOL Symboltype)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	NONANIMDESC NonAnimDesc;
	char cModelTag[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, TEXT("Picking_Symbol"), MAX_PATH, cModelTag, MAX_PATH, NULL, NULL);
	strcpy(NonAnimDesc.pModeltag, cModelTag);
	NonAnimDesc.vPosition = _float3(vClickPos.x, vClickPos.y, vClickPos.z);
	NonAnimDesc.vScale = _float3(1.f, 1.f, 1.f);
	m_iSymBolType = Symboltype;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, TEXT("Layer_PickingSymbol"), &NonAnimDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CCamera_Manager::Click_Position(_float4 vPosition)
{
	//���� ���� ������ 3���� �ȴٸ�. �ﰢ���� �׷����� Cell�� �߰��Ѵ�.
	if (2 == m_vClickedPoints.size())
		Clear_ClickPosition();

	if (0 == m_vClickedPoints.size())
		Add_ClickedSymbol(vPosition, CAM_EYE);
	else if (1 == m_vClickedPoints.size())
		Add_ClickedSymbol(vPosition, CAM_AT);

	m_vClickedPoints.push_back(vPosition);
}

void CCamera_Manager::Clear_ClickPosition()
{
	CGameInstance::Get_Instance()->Clear_Layer(LEVEL_GAMEPLAY, TEXT("Layer_PickingSymbol"));
	m_vClickedPoints.clear();

	m_pClickedSymbol[CAM_EYE] = nullptr;
	m_pClickedSymbol[CAM_AT] = nullptr;
}

void CCamera_Manager::Free()
{

}
