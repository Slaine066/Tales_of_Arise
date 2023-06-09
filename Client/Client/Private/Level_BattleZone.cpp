#include "stdafx.h"
#include "..\Public\Level_BattleZone.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "Level_Loading.h"
#include "BattleManager.h"
#include "Monster.h"

CLevel_BattleZone::CLevel_BattleZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_BattleZone::Initialize()
{
	MONSTER_ID eMonsterID = CBattleManager::Get_Instance()->Get_MonsterType();

	if(eMonsterID == RINWELL)
		CBattleManager::Get_Instance()->Set_BattleMode(true, eMonsterID, true);
	else
		CBattleManager::Get_Instance()->Set_BattleMode(true, eMonsterID);


	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_BackGround")) == false)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
			return E_FAIL;
	}

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

	if (FAILED(Ready_Layer_Battle_UI(TEXT("Layer_UI"))))
		return E_FAIL;


	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_BATTLE);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
	m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLEZONE);
	m_pCamera->Set_Zoom(false);
	m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

	g_fSoundVolume = 0.f;
	CGameInstance::Get_Instance()->StopAll();
	
	switch (eMonsterID)
	{
	case Client::ICE_WOLF:
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BattleZoneEnter.dat"), XMMatrixIdentity());
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BattleZoneBgm0.wav"), g_fSoundVolume);
		break;
	case Client::HAWK:
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BattleZoneEnter.dat"), XMMatrixIdentity());
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BattleZoneBgm1.wav"), g_fSoundVolume);
		break;
	case Client::BERSERKER:
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BattleZoneEnter.dat"), XMMatrixIdentity());
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BattleZoneBgm2.wav"), g_fSoundVolume);
		break;
	case Client::RINWELL:
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellBattleEnter.dat"), XMMatrixIdentity());
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_FightWith_Rinwell.wav"), g_fSoundVolume);
		break;
	default:
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BattleZoneBgmOnlyRinwell.wav"), g_fSoundVolume);
		break;
	}

	CPlayerManager::Get_Instance()->Update_StrikePosition(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Strike_Position.dat"));

	
	return S_OK;
}

void CLevel_BattleZone::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CBattleManager* pBattleManager = GET_INSTANCE(CBattleManager);
	if(pBattleManager->Get_LackonMonster() != nullptr && dynamic_cast<CMonster*>(pBattleManager->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage >= 4.f)
		CPlayerManager::Get_Instance()->Set_SmashAttack();

	//if ( ( CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
	//	dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() != CCamera_Dynamic::CAM_LOCKON ) ||
	//	CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION )
	//{
	//	//+= 0.01f;
	//						 /*if (g_fSoundVolume >= 0.2f)
	//						 g_fSoundVolume = 0.2f;*/
	//}

	g_fSoundVolume = 0.f;
	
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);

	if (CBattleManager::Get_Instance()->Get_IsBattleMode() == false)
	{
		
		CUI_Manager::Get_Instance()->ReSet_Arrived_Count();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		LEVEL eNextLevel = LEVEL_SNOWFIELD;

		m_pCollision_Manager->Clear_AllCollisionGroup();
		pGameInstance->Set_DestinationLevel(eNextLevel);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;
		Safe_Release(pGameInstance);

	}


	if (CBattleManager::Get_Instance()->Get_IsHitLeg() == true)
	{
		m_fHitLegTime += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
		if (m_fHitLegTime > CBattleManager::Get_Instance()->Get_HitLegTimer())
		{
			CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
			m_fHitLegTime = 0.f;
		}
	}



	RELEASE_INSTANCE(CBattleManager);
}

void CLevel_BattleZone::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("BattleZoneLevel"));


	_int iMonsterSize = (_int)CBattleManager::Get_Instance()->Get_BattleMonster().size();

	if (iMonsterSize == 0)
	{
		CBattleManager::Get_Instance()->Set_BattleMode(false);

	}
		

	Update_LockOnSetting();
	Set_FogShader();

}

HRESULT CLevel_BattleZone::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Light3.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

	XMStoreFloat4(&vLightEye, XMVectorSet(64, 200, 200, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(64, 0, 0, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc1;
	NONANIMDESC ModelDesc2;
	NONANIMDESC ModelDesc3;
	NONANIMDESC ModelDesc4;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	ReadFile(hFile, &(ModelDesc1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc2), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc3), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc4), sizeof(NONANIMDESC), &dwByte, nullptr);

	CloseHandle(hFile);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc1.vPosition), 1.f));
	pPlayer->Change_Navigation(LEVEL_BATTLE);
	pPlayer->Compute_CurrentIndex(LEVEL_BATTLE);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Set_BattlePose(false);
	pPlayer->Change_Level(LEVEL_BATTLE);
	pPlayer->Set_IsActionMode(true);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_vector vPosition[3] = { XMVectorSetW(XMLoadFloat3(&ModelDesc2.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc3.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc4.vPosition), 1.f) };
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, vPosition[i]);
		iter->Change_Navigation(LEVEL_BATTLE);
		iter->Compute_CurrentIndex(LEVEL_BATTLE);
		iter->Check_Navigation();
		iter->Change_Level(LEVEL_BATTLE);
		iter->Set_BattlePose(false);
		iter->Set_IsActionMode(true);
		i++;
	}
	
	CPlayerManager::Get_Instance()->Set_BattleMode(true);
	CPlayerManager::Get_Instance()->Set_Ai_Check();
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CBattleManager*			pBattleManager = GET_INSTANCE(CBattleManager);
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	MONSTER_ID eMonsterID = pBattleManager->Get_MonsterType();
	vector<CBaseObj*>		vecAllMonster = pBattleManager->Get_AllMonster();
	
	for (auto& iter : vecAllMonster)
	{
		dynamic_cast<CMonster*>(iter)->Save_LastPosition();
		if (dynamic_cast<CMonster*>(iter)->Get_MonsterID() == eMonsterID)
		{
			pBattleManager->Out_Monster(iter);
			pBattleManager->Add_BattleMonster(iter);

		}
		else
			pGameInstance->Out_GameObject(LEVEL_STATIC, TEXT("Layer_Monster"), iter);
	}

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/MonsterPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	vector<CBaseObj*>		vecBattleMonster = pBattleManager->Get_BattleMonster();
	for (auto& iter : vecBattleMonster)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		iter->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc.vPosition), 1.f));
		dynamic_cast<CMonster*>(iter)->Change_Navigation(LEVEL_BATTLE);
		dynamic_cast<CMonster*>(iter)->Compute_CurrentIndex(LEVEL_BATTLE);
		dynamic_cast<CMonster*>(iter)->Set_BattleMode(true);
		dynamic_cast<CMonster*>(iter)->Set_IsActionMode(true);
		pBattleManager->Set_LackonMonster(iter);

		if (eMonsterID == RINWELL)
			CBattleManager::Get_Instance()->Set_BossMonster(iter);
	}
	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CBattleManager);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_BATTLE, pLayerTag, TEXT("Prototype_BattleZone_Terrain"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_BATTLE, pLayerTag, nullptr)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_BattleZone::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_BATTLE, pLayerTag, &CameraDesc)))
		return E_FAIL;


	CCamera_Action::ACTIONCAMDESC				ActionCameraDesc;
	ZeroMemory(&ActionCameraDesc, sizeof(CCamera_Action::ACTIONCAMDESC));
	ActionCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	ActionCameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	ActionCameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	ActionCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	ActionCameraDesc.CameraDesc.fNear = 0.001f;
	ActionCameraDesc.CameraDesc.fFar = 1000.f;

	ActionCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	ActionCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_BATTLE, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}




HRESULT CLevel_BattleZone::Ready_Layer_Instancing(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	NONANIMDESC stModelDesc;
	strcpy(stModelDesc.pModeltag, "Conifer3_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Dead_Grass_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Dead_Tree1_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Bush_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Broken_Tree_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Deco_SnowField(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/CliffRock.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Snow.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Torch.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Battle_UI(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Alpha_Screen"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	_int numcreate = (_int)(CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 2);
	_int PlayerNum = (_int)(CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1);
	if (numcreate >= 5)
		numcreate = 5;
	for (int i = 0; i < numcreate; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar"), LEVEL_BATTLE, pLayerTag, &i)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont"), LEVEL_BATTLE, pLayerTag, &i)))
			return E_FAIL;

	}



	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Comboline"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_font"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	/*for (int i = 0; i < 6; ++i)
	{
	_uint number = i;

	*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MPguage"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitback"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;
	
	if (PlayerNum >= 3)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"), LEVEL_BATTLE, pLayerTag)))
			return E_FAIL;
	}
	
	
	if (PlayerNum >= 4)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"), LEVEL_BATTLE, pLayerTag)))
			return E_FAIL;

	}
	

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_button"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_change_button"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_LOCKON"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MonsterHP"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;


	if (CBattleManager::Get_Instance()->Get_IsBossBattle())
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_BossMonsterHP"), LEVEL_BATTLE, pLayerTag)))
			return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MainPlayerMark"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage"), LEVEL_BATTLE, pLayerTag)))
		return E_FAIL;

	for (int i = 0; i < 7; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage_font"), LEVEL_BATTLE, pLayerTag, &i)))
			return E_FAIL;


	}
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_BattleZone::Set_FogShader()
{
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayer* pPlayer1 = CPlayerManager::Get_Instance()->Get_EnumPlayer(CPlayer::ALPHEN);
	CPlayer* pPlayer2 = CPlayerManager::Get_Instance()->Get_EnumPlayer(CPlayer::SION);
	CPlayer* pPlayer3 = CPlayerManager::Get_Instance()->Get_EnumPlayer(CPlayer::RINWELL);
	CPlayer* pPlayer4 = CPlayerManager::Get_Instance()->Get_EnumPlayer(CPlayer::LAW);
	

	if( (pPlayer1 != nullptr && pPlayer1->Get_StrikeAttack() == false) &&
		(pPlayer2 != nullptr && pPlayer2->Get_StrikeAttack() == false) &&
		(pPlayer3 != nullptr && pPlayer3->Get_StrikeAttack() == false) &&
		(pPlayer4 != nullptr && pPlayer4->Get_StrikeAttack() == false) )
	{
		/* Fog Shader */
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		if (pPlayer)
		{
			pPlayer->Get_Renderer()->Set_Fog(true);

			CShader* pShaderPostProcessing = pPlayer->Get_Renderer()->Get_ShaderPostProcessing();
			if (FAILED(pShaderPostProcessing->Set_RawValue("g_ViewMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
				return;
			if (FAILED(pShaderPostProcessing->Set_RawValue("g_ProjMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
				return;

			_float3 FogColor = _float3(0.2f, 0.4f, 0.9f);
			_float FogminRange = 60.f;
			_float FogmaxRange = 90.f;
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
	else
	{
		if (pPlayer)
		{
			pPlayer->Get_Renderer()->Set_Fog(false);
		}
			
	}
	

}

void CLevel_BattleZone::Update_LockOnSetting()
{
	if (CGameInstance::Get_Instance()->Key_Down(DIK_CAPSLOCK))
	{
		CUI_RuneEffect::RUNEDESC desc;
		desc.position.x = 80.f;
		desc.position.y = 35.f;
		desc.m_etype = 1;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return;
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_CAPSLOCK))
	{
		
		CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
		if (pLockOn != nullptr)
		{
			m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LOCKON);
			m_pCamera->Set_TargetPosition(pLockOn->Get_TransformState(CTransform::STATE_TRANSLATION));
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_1))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::ALPHEN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.3f);

		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_2))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::SION);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.3f);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_3))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::RINWELL);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.3f);
			

		}
		if (CGameInstance::Get_Instance()->Key_Down(DIK_4))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::LAW);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.3f);
		}
		if (CGameInstance::Get_Instance()->Key_Down(DIK_Z))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_End.wav"), SOUND_CROWD, 0.3f);

			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_Z);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_X))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_End.wav"), SOUND_CROWD, 0.3f);

			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_X);
		}
	}
	else
	{
		if (m_pCamera->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		{
			if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
				CPlayerManager::Get_Instance()->Get_ActivePlayer()->Off_IsFly();

			m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LOCKOFF);
		}
	}
}



CLevel_BattleZone * CLevel_BattleZone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_BattleZone*	pInstance = new CLevel_BattleZone(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_BattleZone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BattleZone::Free()
{
	__super::Free();

	//CBattleManager::Get_Instance()->Clear_Monster();
	Safe_Release(m_pCollision_Manager);

	//CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);

}
