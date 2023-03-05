#include "stdafx.h"
#include "..\Public\Trigger.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Dialoguepopup.h"

CTrigger::CTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CTrigger::CTrigger(const CTrigger & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_TriggerDesc, pArg, sizeof(TRIGGERDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_TriggerDesc.m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_TriggerDesc.m_ModelDesc.vScale.x *= 2.f;
		m_TriggerDesc.m_ModelDesc.vScale.y *= 2.f;
		m_TriggerDesc.m_ModelDesc.vScale.z *= 2.f;
		Set_Scale(m_TriggerDesc.m_ModelDesc.vScale);

	}

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);

	return S_OK;
}

int CTrigger::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CTrigger::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (Check_IsinFrustum(2.f) == false)
		return;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	switch (m_TriggerDesc.eType)
	{
	case UI_TRIGGER:
	{
		if (m_pSPHERECom->Collision(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Collider()))
		{
			if (m_TriggerDesc.iIndex == 0)
			{

				dynamic_cast<CUI_Dialoguepopup*>(CUI_Manager::Get_Instance()->Get_Dialoguepopup())->Open_Dialogue(4, false, 1, 2);
			}
			else if (m_TriggerDesc.iIndex == 1)
			{
				dynamic_cast<CUI_Dialoguepopup*>(CUI_Manager::Get_Instance()->Get_Dialoguepopup())->Open_Dialogue(5, false, 0, 2);
			}
			else if (m_TriggerDesc.iIndex == 2)
			{
				int a = 0;
			}
		}
		break;
	}

		
	default:
		break;
	}

}

HRESULT CTrigger::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrigger::SetUp_ShaderResources()
{
	return S_OK;
}


CTrigger * CTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTrigger* pInstance = new CTrigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrigger::Clone(void * pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTrigger"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);

}
