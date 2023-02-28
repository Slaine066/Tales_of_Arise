#include "stdafx.h"
#include "..\Public\CityNpc.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
#include "UI_Manager.h"
#include "UI_Dialogue.h"
#include "UI_InterectMsg.h"
#include "Rinwell.h"
#include "Level.h"

CCityNpc::CCityNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNpc(pDevice, pContext)
{
}

CCityNpc::CCityNpc(const CCityNpc & rhs)
	: CNpc(rhs)
{
}

HRESULT CCityNpc::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CCityNpc::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	switch (m_NpcDesc.eNpcType)
	{
	default:
		m_eState = 0;
		break;
	}
	
	m_pModelCom->Set_CurrentAnimIndex(m_eState);

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	Check_Navigation();

	return S_OK;
}


int CCityNpc::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CCityNpc::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
   LateTick_State(fTimeDelta);

   CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
   if (!pPlayer)
	   return;

   m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());

   if (m_bCollision)
   {
	   dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(2);
	   m_bIsFirst_conversation = true;

	   if (CGameInstance::Get_Instance()->Key_Up(DIK_E))
			Talk_with_Npc(); 
		   
   }
}


_bool CCityNpc::Is_AnimationLoop(_uint eAnimId)
{

	return true;
}

void CCityNpc::Tick_State(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, true);
}

void CCityNpc::LateTick_State(_float fTimeDelta)
{
}

void CCityNpc::Talk_with_Npc()
{

	switch (m_NpcDesc.eNpcType)
	{
	case NPC_NMM_MHB_000:
		break;

	case MAN_GLD:
		switch (CUI_Manager::Get_Instance()->Get_Dialogue_section())
		{
		case 7:
			dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(7);
			CUI_Manager::Get_Instance()->Set_Dialogue_section(8);
		}
		break;

	default:
		break;
	}
}


HRESULT CCityNpc::Ready_Components(void * pArg)
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 2.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	
		_tchar			szModeltag[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, m_NpcDesc.Modeldesc.pModeltag, (int)strlen(m_NpcDesc.Modeldesc.pModeltag), szModeltag, MAX_PATH);
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
			return E_FAIL;
	

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_City_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CCityNpc * CCityNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCityNpc* pInstance = new CCityNpc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCityNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCityNpc::Clone(void * pArg)
{
	CCityNpc* pInstance = new CCityNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCityNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCityNpc::Free()
{
	__super::Free();
}
