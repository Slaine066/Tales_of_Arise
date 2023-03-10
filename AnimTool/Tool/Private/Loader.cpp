#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

#include "Camera_Dynamic.h"
#include "BackGround.h"
#include "Terrain.h"
#include "NonMovableObject.h"
#include "Player.h"
#include "Monster.h"
#include "PlayerHandle.h"
#include "MeshMap.h"
#include "NaviPoint.h"
#include "Room.h"
#include "Door.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	CoInitializeEx(nullptr, 0);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CImgui_Manager* pImgui = GET_INSTANCE(CImgui_Manager);

	/* �ؽ��� �ε� ��. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��� �ε� ��"));

	/* �� �ε� ��. */
	lstrcpy(m_szLoadingText, TEXT("�� �ε� ��."));
	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), 
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100))))
		return E_FAIL;

	_matrix PivotMatrix;

	/* For.Prototype_Component_Model_Alphen */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Alphen/Alphen.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Alphen");

	/* For.Prototype_Component_Model_IceWolf */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_IceWolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Ice_Wolf");

	/* For.Prototype_Component_Model_SWO2(R00) */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SWO2(R00)"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO00/SWO0.dat"))))
		return E_FAIL;
	pImgui->m_WeaponObj.push_back("SWO2(R00)");

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Sion/Sion.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Sion");

	/* For.Prototype_Component_Model_SIOW(00) */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SIOW(00)"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/SIOW(00)/SIOW(00).dat"))))
		return E_FAIL;
	pImgui->m_WeaponObj.push_back("SIOW(00)");

	/* For.Prototype_Component_Model_Berserker */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Berserker");


	/* For.Prototype_Component_Model_Hawk */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Hawk");

	/* For.Prototype_Component_Model_Astral_Doubt */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Astral_Doubt"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Astral_Doubt/Astral_Doubt.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Astral_Doubt");

	/* For.Prototype_Component_Model_Rinwell */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Rinwell/Rinwell.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Rinwell");
	

	/* For.Prototype_Component_Model_Law */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Law"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Law/Law.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Law");

	///* For.Prototype_Component_Model_RINWELLWEAPON */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_RINWELLWEAPON"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/RinwellWeapon/RINWELLWEAPON.dat"))))
	//	return E_FAIL;
	//pImgui->m_WeaponObj.push_back("RINWELLWEAPON");

	/* For.Prototype_Component_Model_Law */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Current_Astral_Doubt"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Astral_Doubt/Astral_Doubt.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Current_Astral_Doubt");

	/* For.Prototype_Component_Model_Law */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Duohalem"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Duohalem/Duohalem.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Duohalem");

	/* For.Prototype_Component_Model_Law */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kisara"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Kisara/Kisara.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Kisara");

	/* ���̴� �ε� ��. */
	lstrcpy(m_szLoadingText, TEXT("���̴� �ε� ��."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"),
		VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), 
		VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"),
		VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* �ݶ��̴� ���� ��. */
	lstrcpy(m_szLoadingText, TEXT("�ݶ��̴� ���� ��."));
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;
	
	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;
	
	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	/* ��ü ���� ��. */
	lstrcpy(m_szLoadingText, TEXT("��ü ���� ��."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"), CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonMovableObject"), CNonMovableObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"), CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerHandle"), CPlayerHandle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MeshMap"), CMeshMap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NaviPoint"), CNaviPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Room"), CRoom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"), CDoor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	RELEASE_INSTANCE(CImgui_Manager);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
