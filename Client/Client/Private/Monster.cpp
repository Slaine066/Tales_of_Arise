#include "stdafx.h"

#include "Monster.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "BattleManager.h"
#include "Player.h"
#include "DamageFont.h"
#include "Effect.h"
#include "ParticleSystem.h"
#include "UI_font_Hits_number.h"
#include "UI_Combo_font_Hits.h"
#include "UI_Combo_font_Damages.h"
#include "UI_font_Damage_number.h"
#include "UI_Comboline.h"
#include "CriticalFont.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseObj(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	NONANIMDESC ModelDesc;
	if (pArg)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (pArg)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(ModelDesc.m_fAngle));
	}
	Check_NearTrigger();
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	return S_OK;
}

int CMonster::Tick(_float fTimeDelta)
{
	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
		
	if (m_eLevel == LEVEL_SNOWFIELD &&  m_pTrigger == nullptr)
		Check_NearTrigger();

	if (m_bDead)
		return OBJ_DEAD;

	if (this == CBattleManager::Get_Instance()->Get_LackonMonster() && CGameInstance::Get_Instance()->Key_Down(DIK_V))
		m_tStats.m_fLockonSmashGuage = 4.f;

	__super::Tick(fTimeDelta);

	if (m_bhitboost)
	{
		m_fBoostattackedtimer += fTimeDelta;
		if (m_fBoostattackedtimer > 4.f)
		{
			m_fBoostattackedtimer = 0.f;
		}

	}
		


	if (m_bTakeDamage)
	{
		m_fTime_TakeDamageDeltaAcc += fTimeDelta;
	}
		

	if (m_fTakeDamgeTime <= m_fTime_TakeDamageDeltaAcc)
	{
		m_bTakeDamage = false;
		m_fTime_TakeDamageDeltaAcc = 0.f;
	}

	m_fTimeDeltaAcc += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));

	if (ExceptionHanding() == false)
		return OBJ_NOSHOW;

	Reset_StrikeBlur(fTimeDelta);

	return OBJ_NOEVENT;
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_ACTION)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);

		if (!m_bGlowUp)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_B) && false == m_bTakeDamage)
	{
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;
		

		Take_Damage(9999, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
		m_bTakeDamage = true;
	}
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_N) && false == m_bTakeDamage)
	{
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;
	

		Take_Damage(50000, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
		m_bTakeDamage = true;
	}

	if (m_bGlowUp)
	{
		if (m_pDissolveParticles.empty())
		{
			_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
			_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

			_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			CGameInstance::Get_Instance()->PlaySounds(TEXT("KiraKira.wav"), SOUND_OBJECT, 0.1f);
			m_pDissolveParticles = CEffect::PlayEffectAtLocation(TEXT("Dissolve_Particles.dat"), mWorldMatrix);
		}
			
		if (m_bDissolve)
		{
			if (m_fDissolveTimer > m_fDissolveLifespan)
				m_bDead = true;
			else
				m_fDissolveTimer += fTimeDelta;
		}
		else
		{
			if (m_fGlowUpTimer > m_fGlowUpLifespan)
			{
				if (!m_pDissolveParticles.empty())
				{
					for (auto& pDissolveParticle : m_pDissolveParticles)
					{
						CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(pDissolveParticle);
						if (pDissolveParticle)
							pParticleSystem->Set_Stop(true);
					}
				}

				m_bDissolve = true;
			}
			else
				m_fGlowUpTimer += fTimeDelta;
		}
	}
	else
		Collision_Object(fTimeDelta);

	m_eCurLevel = CGameInstance::Get_Instance()->Get_CurrentLevelIndex();

	if (LEVEL_BATTLE == m_eCurLevel)
		m_bBattleMode = true;

	if (LEVEL_SNOWFIELD == m_eCurLevel)
	{
		CCollider* pPlayerCollider = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Collider();
		if (m_pSPHERECom->Collision(pPlayerCollider))
			CBattleManager::Get_Instance()->Set_BattleMode(true, m_eMonsterID);
	}
}

HRESULT CMonster::Render()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	_float3 vRimColor = _float3(.86f, 0.07f, 0.23f);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vRimColor", &vRimColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vRimTimer", &m_fFresnelTimer, sizeof(_float))))
		return E_FAIL;

	_float4 vCameraLook = (_float4)(CGameInstance::Get_Instance()->Get_CamWorldMatrix().m[2]);
	_float3 vCamLook = _float3(vCameraLook.x, vCameraLook.y, vCameraLook.z);

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCameraLook", &vCamLook, sizeof(_float3))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	_bool bRimLight = false;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bRimLight", &bRimLight, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
			return S_OK;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster::Render_EdgeDetection()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
			return E_FAIL;
	}
	return S_OK;
}

_bool CMonster::ExceptionHanding()
{
	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();
	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	if (pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		_uint eCamMode = dynamic_cast<CCamera_Dynamic*>(pCamera)->Get_CamMode();
		if (eCamMode == CCamera_Dynamic::CAM_AIBOOSTON )
			return false;

	}

	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return false;

	if (m_eLevel == LEVEL_SNOWFIELD && m_bBattleMode)
		return false;

	if (pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return false;

	return true;
}

_bool CMonster::ExceptingActionCamHanding()
{
	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	if (pCameraManager->Get_CamState() == CCameraManager::CAM_ACTION && m_bIsActiveAtActionCamera == false)
		return false;
	return true;
}

CMonster::DMG_DIR CMonster::Calculate_DmgDirection()
{
	if (!m_pTarget)
		return FRONT;

	// New Logic
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matTargetWorld = dynamic_cast<CPlayer*>(m_pTarget)->Get_Transform()->Get_WorldMatrix();

	_vector fLookDot = XMVector3Dot(matWorld.r[2], matTargetWorld.r[2]);
	_vector fRightDot = XMVector3Dot(matWorld.r[0], matTargetWorld.r[0]);
	_vector fLookRightDot = XMVector3Dot(matWorld.r[2], matTargetWorld.r[0]);

	cout << XMVectorGetX(fLookDot) << endl;
	if (XMVectorGetX(fLookDot) >= cos(45))
	{
		cout << "BACK" << endl;
		m_eDmg_Direction = BACK;
	}
	else if (XMVectorGetX(fLookDot) < cos(135))
	{
		cout << "FRONT" << endl;
		m_eDmg_Direction = FRONT;
	}
	else if (XMVectorGetX(fLookRightDot) >= cos(45))
	{
		cout << "RIGHT" << endl;
		m_eDmg_Direction = RIGHT;
	}
	else
	{
		cout << "LEFT" << endl;
		m_eDmg_Direction = LEFT;
	}

	return m_eDmg_Direction;
}

_vector CMonster::Calculate_DirectionByPos()
{
	if (!m_pTarget)
		return _vector();

	// New Logic
	_vector vMyPos = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_vector vTargetPos = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_TRANSLATION));

	return vTargetPos - vMyPos;
}

_bool CMonster::Check_AmILastMoster()
{
	vector<CBaseObj*> pMonsterList = CBattleManager::Get_Instance()->Get_BattleMonster();

	_uint iMonsterSize = (_uint)pMonsterList.size();
	_uint iCheckIsDead = 0;

	for (auto& iter : pMonsterList)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0 || iter->Get_Dead())
		{
			if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			{
				CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
				pCamera->Set_TargetPosition(Get_TransformState(CTransform::STATE_TRANSLATION));	
			}
			iCheckIsDead++;
		}
	}

	if ((iMonsterSize - iCheckIsDead) <= 0)
	{
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_TargetPosition(Get_TransformState(CTransform::STATE_TRANSLATION));
			pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLE_CLEAR);

		}
	
		if(CUI_Manager::Get_Instance()->Get_LockOn() != nullptr)
			CUI_Manager::Get_Instance()->Get_LockOn()->Set_Dead(true);
		return true;
	}

	return false;
}

void CMonster::Find_Target()
{
	if (!m_bDead)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
		{
			if (pPlayer->Get_Dead())
			{
				m_pTarget = nullptr;
				m_bAggro = false;
				return;
			}

			_vector vTargetPos = pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - vTargetPos));
			m_pTarget = pPlayer;
		}
	}
}

CBaseObj* CMonster::Find_MinDistance_Target()
{
	list<CGameObject*>* pPlayerList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));

	m_fMinLengh = MAXDISTANCE;
	for (auto& iter : *pPlayerList)
	{
		if (dynamic_cast<CPlayer*>(iter)->Get_Info().fCurrentHp <= 0)
			continue;

		_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));
		if (m_fMinLengh > fDistance)
		{
			m_fMinLengh = fDistance;
			m_pTarget = dynamic_cast<CBaseObj*>(iter);
		}
	}

	m_fMinLengh = MAXDISTANCE;
	return m_pTarget;
}

CBaseObj * CMonster::Check_FiledTarget()
{
	list<CGameObject*>* pPlayerList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));

	for (auto& iter : *pPlayerList)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));
		if (fDistance <= 10.f)
		{
			m_pTarget = dynamic_cast<CBaseObj*>(iter);
		}

	}

	return m_pTarget;
}

_float  CMonster::Target_Distance(CBaseObj* pTarget)
{
	_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - pTarget->Get_TransformState(CTransform::STATE_TRANSLATION)));
	/*_vector fDir = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - Get_TransformState(CTransform::STATE_TRANSLATION);
	XMVectorSetY(fDir, 0.f);
	_float fDistance = XMVectorGetX(XMVector3Length(fDir));*/
	return fDistance;
}

HRESULT CMonster::Drop_Items()
{
	int iRadomItem = rand() % 3;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	//Setting Item Struct
	//And Add GameObject Item

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMonster::Make_GetAttacked_Effect(CBaseObj* DamageCauser)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(DamageCauser);
	if (!pPlayer)
		return;

	_int iRandX = rand() % 20 * 0.1f * (rand()% 2 == 0 ? 1.f : -1.f);
	_int iRandZ = rand() % 20 * 0.1f * (rand() % 2 == 0 ? 1.f : -1.f);
	_int iRandY = rand() % 20 * 0.1f;


	_vector vOffset = XMVectorSet(iRandX, m_fRadius + iRandY, iRandX, 0.f);
	_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

	_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	mWorldMatrix.r[3] = vLocation;
	vector<CEffect*> Impact;

	switch (pPlayer->Get_PlayerID())
	{
		case CPlayer::PLAYERID::ALPHEN:
			Impact = CEffect::PlayEffectAtLocation(TEXT("Alphen_Impact.dat"), mWorldMatrix);
			break;
		case CPlayer::PLAYERID::LAW:
		case CPlayer::PLAYERID::KISARA:
			Impact = CEffect::PlayEffectAtLocation(TEXT("Law_Impact.dat"), mWorldMatrix);
			break;
		case CPlayer::PLAYERID::RINWELL:
		case CPlayer::PLAYERID::SION:
		case CPlayer::PLAYERID::DUOHALEM:
			Impact = CEffect::PlayEffectAtLocation(TEXT("Monster_Hit.dat"), mWorldMatrix);
			break;
	}

	_vector vEffectPosition = Impact.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
	_vector vPlayerPosition = DamageCauser->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_vector vEffectLook = XMVector4Normalize(vPlayerPosition - vEffectPosition);

	Impact.front()->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vEffectPosition + XMVector4Normalize(vEffectLook) * .2f);

	RELEASE_INSTANCE(CGameInstance);
}

void CMonster::Make_DeadEffect(CBaseObj * Target)
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//Setting Effect Struct
	//And Add GameObject Effect

	RELEASE_INSTANCE(CGameInstance);

	m_bMakeEffect = true;
}

void CMonster::Make_UIFont(_uint iDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc)
{

	if (dynamic_cast<CPlayer*>(DamageCauser)->Get_PlayingBoost() && !m_bhitboost)
	{
		m_bhitboost = true;
		m_bDownState = true;
		m_tStats.m_iHitcount = 0;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_damagefontboostbreak"), LEVEL_STATIC, TEXT("break"), this)))
			return;
	}
	else
	{
		if (m_tStats.m_iBedamagedCount >= 20)
		{
			m_bBedamageAnim = true;
			m_tStats.m_iBedamagedCount = 0;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_damagefontbreak"), LEVEL_STATIC, TEXT("break"), this)))
				return;
		}

		if (m_tStats.m_iHitcount >= 10)
		{
			m_bDownState = true;
			m_tStats.m_iHitcount = 0;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_damagefontbreak"), LEVEL_STATIC, TEXT("break"), this)))
				return;
		}
	}


	CDamageFont::DMGDESC testdesc;
	ZeroMemory(&testdesc, sizeof(CDamageFont::DMGDESC));
	testdesc.iDamage = iDamage;
	testdesc.pPointer = this;
	testdesc.itype = 1;
	testdesc.bisNormal = false;

	
	dynamic_cast<CUI_Combo_font_Hits*>(CUI_Manager::Get_Instance()->Get_Hitfont())->sethit();
	dynamic_cast<CUI_font_Hits_number*>(CUI_Manager::Get_Instance()->Get_HitMsg())->sethit();
	dynamic_cast<CUI_Combo_font_Damages*>(CUI_Manager::Get_Instance()->Get_DMGfont())->updatedmg();
	dynamic_cast<CUI_font_Damage_number*>(CUI_Manager::Get_Instance()->Get_DMGNUM())->updatedamage(iDamage);
	dynamic_cast<CUI_Comboline*>(CUI_Manager::Get_Instance()->Get_Comboline())->setline();

	if (HitDesc.bZoom == true && DamageCauser == CPlayerManager::Get_Instance()->Get_ActivePlayer())
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(true, HitDesc.fZoomDistance, HitDesc.fZoomSpeed, HitDesc.fBlurPower, HitDesc.fBlurDetail);
	}


	if (HitDesc.bCritical == true && DamageCauser == CPlayerManager::Get_Instance()->Get_ActivePlayer())
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("CriticalHit.wav"), SOUND_SKILL5, 1.0f);
		testdesc.itype = FONT_CRITYCAL;
		m_DamageOffset = 2.f;
		if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
		{

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
				return;
		}
	}
	else
	{
		switch (rand() % 10)
		{
		case 0:
			//Critical
			m_DamageOffset = 2.f;
			testdesc.itype = FONT_CRITYCAL;
			if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
			{

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
					return;
			}
			break;
		case 1:
			m_DamageOffset = 0.3f;
			testdesc.itype = FONT_RESIST;
			if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
			{

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
					return;
			}
			break;

		case 2:
			m_DamageOffset = 1.5f;
			testdesc.itype = FONT_WEAKNESS;
			if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
			{

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
					return;
			}
			break;
		default:
			m_DamageOffset = 1.f;
			testdesc.itype = 0;
			testdesc.bisNormal = true;
			if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
			{
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("Layer_Damage"), &testdesc)))
					return;
			}
			break;

		}
	}
	

}


_int CMonster::Take_Damage(int fDamage, CBaseObj * DamageCauser, HITLAGDESC HitDesc)
{
	if (fDamage <= 0 || m_bDead)
		return 0;
	
	++m_tStats.m_iHitcount;
	++m_tStats.m_iBedamagedCount;

	m_DamageOffset = 1.f;
	Make_UIFont(fDamage, DamageCauser, HitDesc);
	Make_GetAttacked_Effect(DamageCauser);

	m_fTakeDamgeTime = HitDesc.fTakeDamageTimer;
	m_pTarget = DamageCauser;
	
	m_tStats.m_fCurrentHp-= (int)fDamage * m_DamageOffset;
	
	

	if (DamageCauser == CPlayerManager::Get_Instance()->Get_ActivePlayer())
	{
		if(HitDesc.bLockOnChange)
			CBattleManager::Get_Instance()->Set_LackonMonster(this);

		if (HitDesc.bHitLag)
		{
			CBattleManager::Get_Instance()->Set_IsHitLeg(HitDesc.bHitLag);
			CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 0.f);
			CBattleManager::Get_Instance()->Set_HitLegTimer(HitDesc.fHitLagTimer);

		}
		
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC && HitDesc.bShaking == true)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(HitDesc.bShaking, HitDesc.fShakingPower, HitDesc.fShakingMinusPower);
	}

	if (m_pTarget != nullptr)
	{
		if (dynamic_cast<CPlayer*>(m_pTarget)->Get_BoostGuage() <= 100.f)
			dynamic_cast<CPlayer*>(m_pTarget)->Set_BoostGuage(dynamic_cast<CPlayer*>(m_pTarget)->Get_BoostGuage() + 2.f);

	}
	


	if(CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LAWBATTLE)
		m_tStats.m_fLockonSmashGuage += 0.01f;
	else
		m_tStats.m_fLockonSmashGuage += 0.05f; 


	if (m_tStats.m_fLockonSmashGuage >= 4.f)
		m_tStats.m_fLockonSmashGuage = 4.f;

	


	if (!CBattleManager::Get_Instance()->Get_IsBossBattle())
	{
		if (m_tStats.m_fCurrentHp <= 0)
		{
			return _int(m_tStats.m_fCurrentHp);
		}
	}

	m_bHit = true;
	m_dwHitTime = GetTickCount();
	m_bTakeDamage = true;

	return _int(m_tStats.m_fCurrentHp);
}


void CMonster::Reset_StrikeBlur(_float fTimeDelta)
{
	if (!m_bResetStrikeBlur)
		return;

	if (m_fStrikeBlurResetTimer < m_fStrikeBlurResetDuration)
	{
		/* Zoom Blur Lerp */
		_float fFocusPower = 10.f;

		_float fBlurInterpFactor = m_fStrikeBlurResetTimer / m_fStrikeBlurResetDuration;
		if (fBlurInterpFactor > 1.f)
			fBlurInterpFactor = 1.f;

		_int iDetailStart = 10;
		_int iDetailEnd = 1;
		_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
		m_pRendererCom->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);

		m_fStrikeBlurResetTimer += fTimeDelta;
	}
	else
	{
		m_fStrikeBlurResetTimer = 0.f;
		m_bResetStrikeBlur = false;
		m_pRendererCom->Set_ZoomBlur(false);
	}
}

void CMonster::Collision_Object(_float fTimeDelta)
{
	if(ExceptionHanding() == false)
		return;

	CBaseObj* pCollisionMonster = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionMonster))
	{
		if (pCollisionMonster == this)
			return;
		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionMonster->Get_TransformState(CTransform::STATE_TRANSLATION);

		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);

		m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, vDirection, m_pNavigationCom);
	}

}

void CMonster::Compute_CurrentIndex()
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

void CMonster::Change_Navigation(LEVEL eLevel)
{
	m_pNavigationCom = m_vecNavigation[eLevel];
}

void CMonster::Compute_CurrentIndex(LEVEL eLevel)
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

void CMonster::Check_NearTrigger()
{
	list<CGameObject*>* TriggerLIst = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
	if (TriggerLIst == nullptr)
		return;

	m_fMinLengh = MAXDISTANCE;
	for (auto& iter : *TriggerLIst)
	{
		_vector vTriggerPos = dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION);
		_float fDIstance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - vTriggerPos));

		if (m_fMinLengh > fDIstance)
		{
			m_fMinLengh = fDIstance;
			m_pTrigger = dynamic_cast<CBaseObj*>(iter);
		}
	}
}

HRESULT CMonster::SetUp_ShaderResources()
{
	if (!m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (m_pDissolveTexture)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTexture->Get_SRV(0))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveTimer", &m_fDissolveTimer, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveLifespan", &m_fDissolveLifespan, sizeof(_float))))
			return E_FAIL;
		_float4 vDissolveColor = _float4(0.85f, 0.9f, 1.f, 1.f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveColor", &vDissolveColor, sizeof(_float4))))
			return E_FAIL;
		_float4 vDissolveHighlight = _float4(0.55f, 0.9f, 1.f, 1.f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveHighlight", &vDissolveHighlight, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowUpTimer", &m_fGlowUpTimer, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowUpLifespan", &m_fGlowUpLifespan, sizeof(_float))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderID()
{
	return S_OK;
}

void CMonster::Save_LastPosition()
{
	m_vLastPos = Get_TransformState(CTransform::STATE_TRANSLATION);
}

void CMonster::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	for (auto& iter : m_vecNavigation)
		Safe_Release(iter);

	CBattleManager::Get_Instance()->Out_BattleMonster(this);
	CBattleManager::Get_Instance()->Out_Monster(this);
	Safe_Release(m_pDissolveTexture);
	Safe_Release(m_pModelCom);
	
}