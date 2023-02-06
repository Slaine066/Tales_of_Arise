#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CPhysX*		pPhysX = GET_INSTANCE(CPhysX);
	pPhysX->CreateScene();
	RELEASE_INSTANCE(CPhysX);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;


	//CGameInstance::Get_Instance()->PlaySounds(TEXT("LogoSong.wav"), SOUND_SYSTEM, 0.3f);
	
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		//CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Sys_Do_Start.wav"), SOUND_SYSTEM, 0.4f);

		
		pGameInstance->Set_DestinationLevel(LEVEL_SNOWFIELD);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SNOWFIELD))))
			return;

		Safe_Release(pGameInstance);
	}

	if (CGameInstance::Get_Instance()->Key_Down(DIK_ESCAPE))
		CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);
}

void CLevel_Logo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("�ΰ����Դϴ�."));
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Screen"), LEVEL_LOGO, pLayerTag, nullptr)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();

	//CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);
}
