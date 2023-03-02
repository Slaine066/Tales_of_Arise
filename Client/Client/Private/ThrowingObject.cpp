#include "stdafx.h"
#include "..\Public\ThrowingObject.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Level_Restaurant.h"
#include "EffectObject.h"
#include "Effect.h"

CThrowingObject::CThrowingObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInteractObject(pDevice, pContext)
{
	ZeroMemory(&m_tThrowDesc, sizeof(THROWDESC));
}

CThrowingObject::CThrowingObject(const CThrowingObject & rhs)
	: CInteractObject(rhs)
	, m_tThrowDesc(rhs.m_tThrowDesc)
{
}

HRESULT CThrowingObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CThrowingObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tThrowDesc, pArg, sizeof(THROWDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_tThrowDesc.tNonDesc.vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_tThrowDesc.tNonDesc.vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_tThrowDesc.tNonDesc.vScale.z);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&m_tThrowDesc.tNonDesc.vPosition), 1.f));

	m_vStartPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float fPlayerY = XMVectorGetY((CPlayerManager::Get_Instance()->Get_ActivePlayer())->Get_TransformState(CTransform::STATE_TRANSLATION));
	m_vTargetPos = XMVectorSet(_float(rand() % 23) + 6.f, fPlayerY, _float(rand() % 14) + 25.f, 1.f);
	
	m_iTurnAxis = rand() % 3;

	return S_OK;
}

int CThrowingObject::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
	{
		Dead_Effect();
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MINIGAME1, this);

		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);

	switch (m_iTurnAxis)
	{
	case 0:
		m_vTurnAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		break;
	case 1:
		m_vTurnAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		break;
	case 2:
		m_vTurnAxis = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		break;
	}

	m_pTransformCom->Turn(m_vTurnAxis, fTimeDelta * 5.f);

	_float fMaxTime = 1.f;
	_float fEndHeight = XMVectorGetY(m_vTargetPos - m_vStartPos);
	_float fHeight = 7.f - XMVectorGetY(m_vStartPos);
	_float fGravity = 2.f * fHeight / (fMaxTime * fMaxTime);

	_float fVelocityY = sqrtf(2.f * fGravity * fHeight);

	_float fEndTime = ((2.f * fVelocityY) + sqrtf(4.f * fVelocityY * fVelocityY - 8.f * fGravity * fEndHeight)) / (2 * fGravity);

	_float fVelocityX = (XMVectorGetX(m_vStartPos - m_vTargetPos) * (-1.f)) / fEndTime;
	_float fVelocityZ = (XMVectorGetZ(m_vStartPos - m_vTargetPos) * (-1.f)) / fEndTime;

	m_fTime += fTimeDelta * m_tThrowDesc.fTime;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(m_vStartPos) + (fVelocityX * m_fTime), XMVectorGetY(m_vStartPos) + (fVelocityY * m_fTime) - (0.5f * fGravity * m_fTime * m_fTime), XMVectorGetZ(m_vStartPos) + (fVelocityZ * m_fTime), 1.f));
	
	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix.r[0] = XMVector4Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector4Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector4Normalize(WorldMatrix.r[2]);

	m_pSPHERECom->Update(WorldMatrix);

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MINIGAME1, this);

	return OBJ_NOEVENT;
}

void CThrowingObject::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	__super::Late_Tick(fTimeDelta);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	if (m_fWalkingHeight >= XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		Set_Dead(true);
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);
}

HRESULT CThrowingObject::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CThrowingObject::Render_EdgeDetection()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CThrowingObject::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_tThrowDesc.tNonDesc.pModeltag, (int)strlen(m_tThrowDesc.tNonDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_RestaurantMIniGameNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_Restaurant_MIniGameNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

_bool CThrowingObject::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}

HRESULT CThrowingObject::SetUp_ShaderID()
{
	//if (false == m_bIsGain)
		m_eShaderID = SHADER_NONANIMDEFAULT;
	/*else
		m_eShaderID = SHADER_NONANIM_DISSOLVE;*/

	return S_OK;
}

void CThrowingObject::Dead_Effect()
{
	_float iRandX = rand() % 2 == 0 ? -1.f : 1.f;
	_float iRandZ = rand() % 2 == 0 ? -1.f : 1.f;

	CEffectObject::EFFECTDESC EffectDesc; 
	ZeroMemory(&EffectDesc, sizeof(CEffectObject::EFFECTDESC));
	XMStoreFloat3(&EffectDesc.m_ModelDesc.vPosition, Get_TransformState(CTransform::STATE_TRANSLATION));
	

		
	switch (m_tThrowDesc.eType)
	{
	case TYPE_APPLE:
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius + 1.5f, 0.f, 0.f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		CEffect::PlayEffectAtLocation(TEXT("Alphen_Impact.dat"), mWorldMatrix);

		for (int i = 0; i < 5; ++i)
		{
			iRandX = rand() % 2 == 0 ? -1.f : 1.f;
			iRandZ = rand() % 2 == 0 ? -1.f : 1.f;
			EffectDesc.vTargetDir = XMVectorSet(rand() % 10 * 0.1f * iRandX, 0.5f, rand() % 10 * 0.1f, 0.f * iRandZ);
			EffectDesc.fVelocity = rand() % 3;
			EffectDesc.fDeadTime = 1.f;
			XMStoreFloat4x4(&EffectDesc.m_ModelDesc.WorldMatrix, XMMatrixIdentity());
			EffectDesc.m_ModelDesc.vRotation = _float3(rand() % 180, rand() % 180, rand() % 180);
			EffectDesc.m_ModelDesc.vScale = _float3(3.f, 3.f, 3.f);
			strcpy_s(EffectDesc.m_ModelDesc.pModeltag, "Fruit_Apple_Quarter");

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_EffectObject"), LEVEL_RESTAURANT, TEXT("Layer_Effect"), &EffectDesc)))
				return;
		}

		break;
	}
	case TYPE_BREAD:
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius + 1.5f, 0.f, 0.f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		CEffect::PlayEffectAtLocation(TEXT("Alphen_Impact.dat"), mWorldMatrix);
		
		for (int i = 0; i < 2; ++i)
		{
			iRandX = rand() % 2 == 0 ? -1.f : 1.f;
			iRandZ = rand() % 2 == 0 ? -1.f : 1.f;
			EffectDesc.vTargetDir = XMVectorSet(rand() % 10 * 0.1f * iRandX, -1.f, rand() % 10 * 0.1f, 0.f * iRandZ);
			EffectDesc.fVelocity = rand() % 3;
			EffectDesc.fDeadTime = 1.f;
			EffectDesc.m_ModelDesc.vScale = _float3(3.f, 3.f, 3.f);

			XMStoreFloat4x4(&EffectDesc.m_ModelDesc.WorldMatrix, XMMatrixIdentity());
			EffectDesc.m_ModelDesc.vRotation = _float3(rand() % 180, rand() % 180, rand() % 180);
			strcpy_s(EffectDesc.m_ModelDesc.pModeltag, "Bread_French_Slice");

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_EffectObject"), LEVEL_RESTAURANT, TEXT("Layer_Effect"), &EffectDesc)))
				return;
		}

		break;
	}
	default:
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius + 1.5f, 0.f, 0.f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		CEffect::PlayEffectAtLocation(TEXT("Alphen_Impact.dat"), mWorldMatrix);
		break;
	}
	}
}

CThrowingObject * CThrowingObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CThrowingObject* pInstance = new CThrowingObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CThrowingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThrowingObject::Clone(void * pArg)
{
	CThrowingObject* pInstance = new CThrowingObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CThrowingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThrowingObject::Free(void)
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
}
