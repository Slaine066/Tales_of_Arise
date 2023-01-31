#include "stdafx.h"
#include "Loader.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Terrain.h"
#include "Weapon.h"
#include "Sky.h"
#include "Water.h"
#include "NonAnim_Instance.h"
#include "NonAnim.h"
#include "Trigger.h"

//Effect
#include "EffectTexture.h"
#include "EffectMesh.h"
#include "ParticleSystem.h"
#include "Particle_Rect.h"
#include "Particle_Point.h"


//Actor
#include "Alphen.h"
#include "Sion.h"


//UI
#include "UI_Portrait.h"
#include "UI_Screen.h"
#include "ScreenFadeEffect.h"
#include "UI_Loading.h"
#include "HPbar.h"
#include "HP_Font.h"
#include "UI_Comboline.h"
#include "UI_Combo_font_Damages.h"
#include "UI_Combo_font_Hits.h"
#include "UI_font_Damage_number.h"
#include "UI_font_Hits_number.h"
#include "CP_Guage.h"
#include "CP_Guage_font.h"
#include "UI_MPguage.h"
#include "HPbar_World.h"
#include "HP_Font_World.h"
#include "UI_Portraitback.h"
#include "UI_Portraitfront.h"
#include "UI_Portraitfront_top.h"
#include "UI_Portraitfront_right.h"
#include "UI_Portraitfront_bottom.h"
#include "UI_Portraitfront_left.h"
#include "UI_Skillbutton.h"
#include "UI_Changebutton.h"
#include "UI_LOCKON.h"
#include "UI_MENU_Back.h"
#include "UI_Dialogue.h"
#include "UI_RuneEffect.h"
#include "UI_Dialoguepopup.h"
#include "UI_InterectMsg.h"
#include "UI_Get_item_Popup.h"

//Monster
#include "Ice_Wolf.h"
#include "Berserker.h"
#include "Hawk.h"
#include "Slime.h"

//InteractObject
#include "TreasureBox.h"
#include "Item.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
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
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_BATTLE:
		pLoader->Loading_ForBattleLevel();
		break;
	case LEVEL_SNOWFIELD:
		pLoader->Loading_ForSnowFieldLevel();
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

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* Static Resource */
	lstrcpy(m_szLoadingText, TEXT("Static Resource Loading"));
	if (FAILED(Loading_ForStaticLevel()))
		return E_FAIL;

	/* Loading Prototype */
	lstrcpy(m_szLoadingText, TEXT("Loading Prototype"));
	if (FAILED(Loading_ForPrototype()))
		return E_FAIL;

	/* UI Loading */
	lstrcpy(m_szLoadingText, TEXT("UI Loading"));
	if (FAILED(Loading_ForUITexture()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Finished Loading"));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForPrototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	/*For.Prototype_Trigger */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Trigger"),
		CTrigger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_NonAnim */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim"),
		CNonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_NonAnim */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim_Instance"),
		CNonAnim_Instance::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_BackGround_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Screen"),
		CUI_Screen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI"),
		CUI_Portrait::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"),
		CScreenFadeEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPbar"),
		CHPbar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPbar_World"),
		CHPbar_World::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CPguage"),
		CCP_Guage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_CPguage_font"),
		CCP_Guage_font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MPguage"),
		CMP_Guage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPfont"),
		CHP_Font::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HPfont_World"),
		CHP_Font_World::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Comboline"),
		CUI_Comboline::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"),
		CUI_Combo_font_Damages::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_HITS_font"),
		CUI_Combo_font_Hits::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"),
		CUI_font_Damage_number::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"),
		CUI_font_Hits_number::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitback"),
		CPortraitback::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"),
		CUI_Portraitfront_top::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"),
		CUI_Portraitfront_right::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"),
		CUI_Portraitfront_bottom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"),
		CUI_Portraitfront_left::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_button"),
		CUI_Skillbutton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance ->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_change_button"),
		CUI_Changebutton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Rune_Effect"),
		CUI_RuneEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_LOCKON"),
		CUI_LOCKON::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_MENU_BACK"),
		CUI_MENU_Back::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialogue"),
		CUI_Dialogue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialoguepopup"),
		CUI_Dialoguepopup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"),
		CUI_Get_item_Popup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_INTERECTMSG"),
		CUI_InterectMsg::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/



	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Combo_Portraitfront"),
		CUI_Portraitfront::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/


	/*For.Prototype_GameObject_Sky */                                
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Alphen"),
		CAlphen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sion"),
		CSion::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Water*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Weapon */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectTexture*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectTexture"),
		CEffectTexture::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectMesh*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectMesh"),
		CEffectMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.GameObject_ParticleSystem*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ParticleSystem"),
		CParticleSystem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Particle_Rect */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Rect"),
		CParticle_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Particle_Point */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle_Point"),
		CParticle_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Monster//

	/*For.Prototype_GameObject_Ice_Wolf */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Ice_Wolf"),
		CIce_Wolf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Berserker*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Berserker"),
		CBerserker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Hawk"),
		CHawk::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Slime*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Slime"),
		CSlime::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//InteractObject

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBox"),
		CTreasureBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForStaticLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
#pragma region Static Shader Loading
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Effect*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_Effect.hlsl"), VTXTEXALPHA_DECLARATION::Elements, VTXTEXALPHA_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModelInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModelInstance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTexture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTexture"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxCubeTexture.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPoint */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPoint"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxPoint.hlsl"), VTXPOINT_DECLARATION::Elements, VTXPOINT_DECLARATION::iNumElements))))
		return E_FAIL;

#pragma endregion Static Shader Loading

#pragma region Buffer Loading
	/*For.Prototype_Component_VIBuffer_RectInstance */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_RectInstance"),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, 50))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_PointInstance*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, 50))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Point*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/HeightMap3.bmp"), true))))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Terrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_BattleZone_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Terrain.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_SnowField_Terrain"), CVIBuffer_Terrain::Create(m_pDevice, m_pContext, hFile, dwByte, true))))
		return E_FAIL;
	CloseHandle(hFile);

#pragma endregion Buffer Loading

#pragma region Texture Loading
	/*For.Prototype_Component_Texture_Spark */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_00.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_02.png"), 1))))
		return E_FAIL;
	/*For.Prototype_Component_Texture_Dissolve*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Dissolve_Texture.png"), 1))))
		return E_FAIL;
	/*For.Prototype_Component_Texture_Distortion_Noise*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Distortion_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distortion_Noise.png"), 1))))
		return E_FAIL;
	/*For.Prototype_Component_Texture_Distortion_Strength*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Distortion_Strength"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distortion_Strength.png"), 1))))
		return E_FAIL;
#pragma endregion Texture Loading

#pragma region Model Loading
	/* For.Prototype_Component_Model_Alphen */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Alphen/Alphen.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Sion/Sion.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sion */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NpcFemaleYoung"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/NpcFemaleYoung/NpcFemaleYoung.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_Plane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Water/Plane.dat"))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Alphen_Weapon01 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SWO1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO01/SWO01.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_IceWolf*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Berserker*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Slime*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Slime"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Slime/Slime.dat"))))
		return E_FAIL;
#pragma endregion Model Loading

#pragma region Navigation Loading

	/* Loading Navigation */
	lstrcpy(m_szLoadingText, TEXT("Loading Navigation "));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Battle_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Field_Data/Field_Navi.dat")))))
		return E_FAIL;
#pragma endregion Navigation Loading



	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForBattleLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* Loading Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading Texture"));

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Terrain_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowBattleZone/Floor%d_C.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_SnowNormal */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_TerrainNormal_SnowBattle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowBattleZone/Floor%d_N.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/BattleZone1.bmp"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Snow/snow.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SnowDark.dds"), 1))))
		return E_FAIL;

	/* Loading Model */
	lstrcpy(m_szLoadingText, TEXT("Loading Model"));

	/*For.Prototype_Component_Model_IceWolf*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Hawk*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;

	///*For.Prototype_Component_Model_Berserker*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Berserker"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
	//	return E_FAIL;
	//
	///*For.Prototype_Component_Model_Hawk*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Hawk"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_Slime*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Slime"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Slime/Slime.dat"))))
	//	return E_FAIL;


	///*For.Prototype_Component_Model_TreasureBox00*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("TreasureBox00"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox00/TreasureBox00.dat"))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_TreasureBox02_Blue*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("TreasureBox02_Blue"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02_Blue/TreasureBox02_Blue.dat"))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_TreasureBox02_Red*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("TreasureBox02_Red"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02_Red/TreasureBox02_Red.dat"))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_TreasureBox02*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("TreasureBox02"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02/TreasureBox02.dat"))))
	//	return E_FAIL;

	///NON ANIM 

	
	/*For.Prototype_Component_Model_Apple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Apple/Apple.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Egg*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Egg"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Egg/Egg.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Mushroom*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Mushroom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Mushroom/Mushroom.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Lattice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lattice.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Scale"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Scale.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Snow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Snow2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Snow3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Stairs"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stairs.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Stalagmite3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Stalagmite4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite4.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Stalagmite5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("SteelFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/SteelFence.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Torch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Torch.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("WallPillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WallPillar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("WoodShielf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("WoodShielf2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf2.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Snow_Mountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Snow_Mountain/Snow_Mountain.dat"))))
		return E_FAIL;

	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	/* Loading Instancing */
	lstrcpy(m_szLoadingText, TEXT("Loading Instancing"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Conifer3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat", 
			"../../../Bin/Data/BattleZoneData/SnowPlane/Conifer.dat"))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Dead_Grass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Dead_Grass.dat"))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Dead_Tree1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree1.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Dead_Tree1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Bush"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Bush.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BATTLE, TEXT("Broken_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree.dat",
			"../../../Bin/Data/BattleZoneData/SnowPlane/Broken_Tree.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Finished Loading"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForSnowFieldLevel()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

#pragma region texture
	/* Loading Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading Texture"));

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField/Terrain%d_D.dds"), 4))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_NormalTerrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField/Terrain%d_N.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField.bmp"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Snow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Snow/snow.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/Sky_SnowDark.dds"), 1))))
		return E_FAIL;


#pragma endregion texture


	/* Loading Model */
	lstrcpy(m_szLoadingText, TEXT("Loading Model"));

	
	/*For.Prototype_Component_Model_TreasureBox02*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("TreasureBox02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02/TreasureBox02.dat"))))
		return E_FAIL;


#pragma region Deco NonAnim Object
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Lattice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lattice.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("SteelFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/SteelFence.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Torch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Torch.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Snow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("WoodShielf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("WoodShielf2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Snow_Mountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Snow_Mountain/Snow_Mountain.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bag2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bag3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bale"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bale2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bale3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Barrel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Barrel3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Basket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Basket2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Boat.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bonfire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bonfire.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bowl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bowl.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Box"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Box2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Box3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bucket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bucket.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Cage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Cage.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CampFire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CampFire.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Hut"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Hut.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("WoodFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/WoodFloor.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bridge_End"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_End.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bridge_Middle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_Middle.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("CommonBridge"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CommonBridge.dat"))))
		return E_FAIL;

#pragma endregion Deco NonAnim Object


#pragma region Interact Object 

	/*For.Prototype_Component_Model_Apple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Apple/Apple.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Egg*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Egg"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Egg/Egg.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Mushroom*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Mushroom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Mushroom/Mushroom.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Jewel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_003/ORE_003.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Lettuce_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_001.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Lettuce_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_002.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("GroundPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/GroundPlant.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("SlimPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/SlimPlant.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Crystal"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Crystal/Crystal.dat"))))
		return E_FAIL;

#pragma endregion Interact Object 


#pragma region Instancing
	/* Loading Instancing */
	lstrcpy(m_szLoadingText, TEXT("Loading Instancing "));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Bush"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush.dat",
			"../../../Bin/Data/Field_Data/Bush.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Birch1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch1.dat",
			"../../../Bin/Data/Field_Data/Birch1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Birch2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch2.dat",
			"../../../Bin/Data/Field_Data/Birch2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Broken_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree.dat",
			"../../../Bin/Data/Field_Data/Brokentree.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("BushWood"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/BushWood.dat",
			"../../../Bin/Data/Field_Data/BushWood.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Conifer3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat",
			"../../../Bin/Data/Field_Data/Conifer.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Dead_Grass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass.dat",
			"../../../Bin/Data/Field_Data/Dead_Grass.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Dead_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree1.dat",
			"../../../Bin/Data/Field_Data/Dead_Tree.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Fence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/Fence.dat",
			"../../../Bin/Data/Field_Data/Fence.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Lamppillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Light/StreetLight_002.dat",
			"../../../Bin/Data/Field_Data/Lamppillar.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("SmallRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Bld/SmallRock2.dat",
			"../../../Bin/Data/Field_Data/SmallRock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Snow2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow2.dat",
			"../../../Bin/Data/Field_Data/Snow.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Stalagmite5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite5.dat",
			"../../../Bin/Data/Field_Data/Stalagmite5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Stalagmite4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite4.dat",
			"../../../Bin/Data/Field_Data/Stalagmite4.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Tree5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree5.dat",
			"../../../Bin/Data/Field_Data/Tree5.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM_INSTANCE, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree1.dat",
			"../../../Bin/Data/Field_Data/Tree.dat"))))
		return E_FAIL;

#pragma endregion Instancing


	/* Loading Collider */
	lstrcpy(m_szLoadingText, TEXT("Loading Collider"));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SNOWFIELD, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading Finished"));


	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForUITexture()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* Loading UI_Texture */
	lstrcpy(m_szLoadingText, TEXT("Loading UI_Texture "));

	/*For.texturelogo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Logo/logo%d.dds"), 4))))
		return E_FAIL;

	/*For.texturefade */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fade"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/fade.png"), 1))))
		return E_FAIL;

	/*For.texturelogosprite */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logosprite"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/logosprite/logosprite%d.png"), 237))))
		return E_FAIL;

	/*For.hpbar */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/HP_bar%d.dds"), 2))))
		return E_FAIL;


	/*For.gradation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/HPbar/hpgrad.dds"), 1))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/tfont%d.dds"), 11))))
		return E_FAIL;

	/*For.hpfont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_smallfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/numberfont/smallfont%d.dds"), 11))))
		return E_FAIL;

	/*For.line */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/comboline/sline%d.png"), 3))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_hitfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/hitfont0.png"), 1))))
		return E_FAIL;

	/*For.combofont */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_damagesfont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/combofonts/damagefont0.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CPguage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/CPguage/cpguage%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MPguage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/MPguage/MPguage%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/portraitbase%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_front"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/portrait/battle_portrait_%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillbutton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/skillbutton%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_changebutton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/ctrlicon.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_RuneEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/runeeffect/rune%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/charactername/charactername%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lockon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lockon/lockon%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_BACK"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuback%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_ICON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuicon%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_SELECT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuselect%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/Inventoryback%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemname/itemname%d.dds"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENULINE"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menuline%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMICON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemicon/itemicon%d.dds"), 20))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMINFO"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/iteminfo/iteminfo%d.dds"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUGALD"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/menuback/menugald%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUBUTTON"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/menubutton%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUFONT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/skillkey/menufont%d.dds"), 8))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CATEGORYNAMEFONT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/categorynamefont/categorynamefont%d.dds"), 5))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FLAG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/flags/flag0.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMUSEPORTRAIT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/itemuseportrait/portrait%d.dds"), 12))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LIGHTEFFECT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/lighteffect/light%d.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/maintextbox%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sideportrait"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/dialogue/sideportrait%d.dds"), 4))))
		return E_FAIL;
	

	
	
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
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