#include "stdafx.h"
#include "..\Public\Level_BossZone.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Level_City.h"
#include "Level_Loading.h"
#include "Portal.h"
#include "PlayerIdleState.h"

using namespace Player;

CLevel_City::CLevel_City(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_City::Initialize()
{
	CBattleManager::Get_Instance()->Set_BattleMode(false);

	if (CGameInstance::Get_Instance()->Get_PastLevelIndex() == LEVEL_BOSS)
		g_bEnd = true;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}

	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (g_bEnd)
	{
		if (FAILED(Ready_Layer_PlayerEnding(TEXT("Layer_Player"))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
			return E_FAIL;
	}


	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Backgorund")) == false)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Backgorund"))))
			return E_FAIL;
	}
	

	
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	if (!g_bEnd)
	{
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco_Market"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing_Culling"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));
		//CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Child"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Gld"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NMM_SLV"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NFM"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Trigger"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Plc"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_FIA"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_DIM"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_GNL"));
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Duck"));
		//CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Effects"));
	}
	


	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_CITY);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
	m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

	g_fSoundVolume = 0.f;

	CGameInstance::Get_Instance()->StopAll();
	
	if (g_bEnd)
	{
		CGameInstance::Get_Instance()->PlayBGM(TEXT("LogoSong.wav"), g_fSoundVolume);
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("Ending.dat"), XMMatrixIdentity());
	}
	else
	{
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_City.wav"), g_fSoundVolume);
		//CGameInstance::Get_Instance()->PlaySounds(TEXT("Natrue_mountain_Bird_Bug.wav"), SOUND_NATURE, 0.1f);
		//CGameInstance::Get_Instance()->PlaySounds(TEXT("Natrue_Crowd.wav"), SOUND_CROWD, g_fSoundNatureVolume);

	}
		
	return S_OK;
}

void CLevel_City::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	g_fSoundVolume = 0.f;//+= 0.01f;
	/*g_fSoundVolume += 0.001f;
	if (g_fSoundVolume >= 0.3f)
		g_fSoundVolume = 0.3f;*/
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_CROWD, g_fSoundNatureVolume);


	if (m_bNextNevel)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
			pPlayer->Get_Renderer()->Set_ZoomBlur(false);

		CPlayerManager::Get_Instance()->Save_LastPosition();
		m_pCollision_Manager->Clear_AllCollisionGroup();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco_Market"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing_Culling"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Child"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Gld"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NMM_SLV"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NFM"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Trigger"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Plc"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_FIA"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_DIM"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_GNL"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Duck"));
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Effects"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = (LEVEL)m_iNextLevelIndex;

		if(eNextLevel == LEVEL_BOSS)
			CBattleManager::Get_Instance()->Set_BattleMode(true);
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}


	if (CGameInstance::Get_Instance()->Key_Up(DIK_0))
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
			pPlayer->Get_Renderer()->Set_ZoomBlur(false);

		CPlayerManager::Get_Instance()->Save_LastPosition();
		m_pCollision_Manager->Clear_AllCollisionGroup();
		
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco_Market"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing_Culling"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Child"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Gld"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NMM_SLV"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NFM"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Trigger"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Plc"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_FIA"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_DIM"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_GNL"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Duck"));
		
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = LEVEL_BOSS;
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);

		m_fBlurTimer = 0.f;
	}

}

void CLevel_City::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("Level_City"));

	/* Fog Shader */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (pPlayer)
	{
		pPlayer->Get_Renderer()->Set_Fog(true);

		CShader* pShaderPostProcessing = pPlayer->Get_Renderer()->Get_ShaderPostProcessing();
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_ViewMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return;
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_ProjMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return;

		_float3 FogColor = _float3(1.f, 1.f, 1.f);
		_float FogminRange = 20.f;
		_float FogmaxRange = 140.f;
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_vFogColor", &FogColor, sizeof(_float3))))
			return;
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_fMinRange", &FogminRange, sizeof(_float))))
			return;
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_fMaxRange", &FogmaxRange, sizeof(_float))))
			return;

		_float3 vPlayerPosition;
		XMStoreFloat3(&vPlayerPosition, pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

		if (FAILED(pShaderPostProcessing->Set_RawValue("g_vPlayerPosition", &vPlayerPosition, sizeof(_float3))))
			return;
	}

	RELEASE_INSTANCE(CGameInstance);
}


HRESULT CLevel_City::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
		//if (0 == dwByte)
			//break;

		if (LightDesc.eType == LIGHTDESC::TYPE_POINT)
			LightDesc.vPosition.w = 1.f;
		pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc);
	}
	CloseHandle(hFile);

	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(50, 100, 100, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(50, 0, 0, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

	

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/PortalPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	CPortal::PORTALDESC PortalDesc;
	/* Ÿ���� ���� �޾ƿ��� */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_RESTAURANT;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;


	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_WORKTOOL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_LAWBATTLE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_BOSS;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);




	return S_OK;
}

HRESULT CLevel_City::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_PastLevelIndex() == LEVEL_LAWBATTLE)
	{
		if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::LAW) == nullptr)
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Law"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
				return E_FAIL;
		}
	}


	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);

	if(pGameInstance->Get_PastLevelIndex() == LEVEL_SNOWFIELD)
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20.f, 0.f, -135.f, 1.f));
	else if (pGameInstance->Get_PastLevelIndex() == LEVEL_LAWBATTLE)
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(13.f, 0.f, 327.f, 1.f));
	else
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());

	pPlayer->Change_Navigation(LEVEL_CITY);
	pPlayer->Compute_CurrentIndex(LEVEL_CITY);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_CITY);
	

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		if (pGameInstance->Get_PastLevelIndex() == LEVEL_SNOWFIELD)
			pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20.f, 0.f, -135.f, 1.f));
		else if (pGameInstance->Get_PastLevelIndex() == LEVEL_LAWBATTLE)
			pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(13.f, 0.f, 327.f, 1.f));
		else
			pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());
		iter->Change_Navigation(LEVEL_CITY);
		iter->Compute_CurrentIndex(LEVEL_CITY);
		iter->Check_Navigation();
		iter->Off_IsFly();
		iter->Change_Level(LEVEL_CITY);

		i++;
	}
	
	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_City::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_CITY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_CITY, pLayerTag, TEXT("Prototype_City_WaterTerrain"))))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Water.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Water"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_City::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vDistance = _float4(0, 10, -10, 0.f);

	CameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.1f;
	CameraDesc.CameraDesc.fFar = 1000.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_CITY, pLayerTag, &CameraDesc)))
		return E_FAIL;


	CCamera_Action::ACTIONCAMDESC				ActionCameraDesc;
	ZeroMemory(&ActionCameraDesc, sizeof(CCamera_Action::ACTIONCAMDESC));

	ActionCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	ActionCameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	ActionCameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	ActionCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	ActionCameraDesc.CameraDesc.fNear = 0.1f;
	ActionCameraDesc.CameraDesc.fFar = 1000.f;

	ActionCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	ActionCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_CITY, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_City::Ready_Layer_PlayerEnding(const _tchar * pLayerTag)
{
	CPlayerManager* pPlayerManager = CPlayerManager::Get_Instance();

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC Active1;
	NONANIMDESC Active2;
	NONANIMDESC AIplayer1;
	NONANIMDESC AIplayer2;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Ending/PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	ReadFile(hFile, &(Active1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(Active2), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer2), sizeof(NONANIMDESC), &dwByte, nullptr);
	CloseHandle(hFile);

	CPlayer* Alphen = pPlayerManager->Get_EnumPlayer(CPlayer::ALPHEN);
	CPlayer* Sion = pPlayerManager->Get_EnumPlayer(CPlayer::SION);
	CPlayer* Rinwell = pPlayerManager->Get_EnumPlayer(CPlayer::RINWELL);
	CPlayer* Law = pPlayerManager->Get_EnumPlayer(CPlayer::LAW);

	Alphen->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active1.vPosition), 1.f));
	Sion->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active2.vPosition), 1.f));
	Rinwell->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer1.vPosition), 1.f));
	Law->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer2.vPosition), 1.f));

	Alphen->Set_IsActionMode(true);
	Sion->Set_IsActionMode(true);
	Rinwell->Set_IsActionMode(true);
	Law->Set_IsActionMode(true);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_CITY);
	pPlayer->Get_Transform()->LookDir( XMVectorSet(0.f, 0.f, -1.f, 0.f));
	
	CPlayerState* pState = new Player::CIdleState(pPlayer, Player::CIdleState::IDLE_SIDE);
	pPlayer->Set_PlayerState(pState);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Off_IsFly();
		iter->Set_Overlimit(false);
		iter->Change_Level(LEVEL_CITY);
		iter->Get_Transform()->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
		i++;
	}

	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	return S_OK;
}



CLevel_City * CLevel_City::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_City*	pInstance = new CLevel_City(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_City"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_City::Free()
{
	__super::Free();

	//CBattleManager::Get_Instance()->Clear_Monster();
	Safe_Release(m_pCollision_Manager);

	CGameInstance::Get_Instance()->StopSound(SOUND_NATURE);
	CGameInstance::Get_Instance()->StopSound(SOUND_CROWD);
}
