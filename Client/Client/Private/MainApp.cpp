#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "CameraManager.h"
#include "UI_Loading.h"
#include "Collision_Manger.h"
#include "PlayerManager.h"
#include <time.h>
#include "UI_Dialogue.h"
#include "BattleManager.h"
#include "UI_Dialoguepopup.h"
#include "UI_Dialogue_Caption.h"
//#include "Imgui_Manager.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, m_pUI_Manager(CUI_Manager::Get_Instance())
{
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}


	srand(unsigned int(time(NULL)));

	GRAPHIC_DESC		Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, Graphic_Desc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	//if (FAILED(CImgui_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext)))
	//	return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	// MakeSpriteFont "��Ʈ�̸�" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 ��������̸�.spritefont
	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pContext, TEXT("Font_Nexon"), TEXT("../../../Bin/Resources/Fonts/JinGothic.spritefont"))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	if (m_pGameInstance->Key_Up(DIK_BACKSLASH))
		g_fSoundVolume = 0.f;

	if (m_pGameInstance->Key_Up(DIK_ESCAPE) && !(m_pUI_Manager->Get_StopTick()))
		m_pUI_Manager->Set_Mainmenuon(true);

	if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
	{
		CLevel* pCurrentLevel = CGameInstance::Get_Instance()->Get_CurrentLevel();
		pCurrentLevel->Set_NextLevel(true, LEVEL_LAWBATTLE);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_Y))
	{
		CUI_Manager::Get_Instance()->Set_Dialogue_section(6);
	}

	/*if(m_pGameInstance->Key_Up(DIK_7))
		dynamic_cast<CUI_Dialoguepopup*>(m_pUI_Manager->Get_Dialoguepopup())->Open_Dialogue(0 , false , 1 ,0);
	else if (m_pGameInstance->Key_Up(DIK_8))
		dynamic_cast<CUI_Dialoguepopup*>(m_pUI_Manager->Get_Dialoguepopup())->Open_Dialogue(1 , false , 0 , 1);*/

	if (m_pUI_Manager->Get_UI_OpenType() == CUI_Manager::UI_INVEN && m_pGameInstance->Get_CurrentLevelIndex() != LEVEL_LOADING)
		m_pUI_Manager->Tick_Inventory();
	else if (m_pUI_Manager->Get_UI_OpenType() == CUI_Manager::UI_MAP && m_pGameInstance->Get_CurrentLevelIndex() != LEVEL_LOADING)
		m_pUI_Manager->Tick_Map();

	m_pGameInstance->Tick_Engine(fTimeDelta);

//#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
//#endif // _DEBUG
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Render_GameObjects();


	if (CGameInstance::Get_Instance()->Key_Up(DIK_P))
		m_bImgui = !m_bImgui;

	

	if (m_bImgui)
	{
		/*CImgui_Manager::Get_Instance()->Tick(0.1f);
		CImgui_Manager::Get_Instance()->Render();*/
	}

	++m_iNumRender;

	if (m_fTimeAcc > 1.0f)
	{
		cout << "FPS : "<< m_iNumRender << "\n" << endl;
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*			pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevel);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_Component_VIBuffer_Point_Instance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Instance"), CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/*FOR LOADING UI*/
	/* For.Prototype_Component_Shader_VtxCubeTexture */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_UI.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loadingsprite"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/loadingsprite/loadingsprite%d.png"), 37))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	///////////////////////////////

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	CObject_Pool_Manager::Get_Instance()->Clear_AllPooling_Layer();

	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_BATTLE ||
		CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_BOSS)
	{
		CBattleManager::Get_Instance()->Return_AllPoolingMonster_AtClear();
	}
		

	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);
	
	CGameInstance::Release_Engine();


	Safe_Release(m_pUI_Manager);

	

	CUI_Manager::Get_Instance()->Destroy_Instance();
	CCollision_Manager::Get_Instance()->Destroy_Instance();
	CObject_Pool_Manager::Get_Instance()->Destroy_Instance();
	CCameraManager::Get_Instance()->Destroy_Instance();
	CPlayerManager::Get_Instance()->Destroy_Instance();
	CBattleManager::Get_Instance()->Destroy_Instance();

}

