#include "stdafx.h"
#include "..\Public\UI_SystemMessageBox.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Level.h"



CUI_SystemMessageBox::CUI_SystemMessageBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_SystemMessageBox::CUI_SystemMessageBox(const CUI_SystemMessageBox & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_SystemMessageBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_SystemMessageBox::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&m_msgboxdesc, pArg, sizeof(MSGBOXDESC));

	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0.f;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_SystemMessageBox::Tick(_float fTimeDelta)
{
	//	if (m_btick)
	//	{


	if (m_bfadein)
	{
		m_fAlpha += 0.15f;
		if (m_fAlpha >= 1.f)
		{

			m_fAlpha = 1.f;
			m_bfadein = false;
			//m_bfadein1 = true;
		}
	}






	if (CGameInstance::Get_Instance()->Key_Up(DIK_RETURN))
	{
		if (m_iCursor == 1)
			m_bfadeout = true;
		else
		{
			CUI_Manager::Get_Instance()->Set_Dialogue_section(7);
				CLevel* pCurrentLevel = CGameInstance::Get_Instance()->Get_CurrentLevel();
				pCurrentLevel->Set_NextLevel(true, LEVEL_CITY);
				m_bfadeout = true;
			
		}

	}


	if (m_bfadeout)
	{
		m_fAlpha -= 0.15f;



		if (m_fAlpha <= 0.f)
		{
			return OBJ_DEAD;
		}
	}
	



	if(CGameInstance::Get_Instance()->Key_Up(DIK_DOWN))
	{
		if (m_iCursor == 0)
			m_iCursor = 1;
		else
			m_iCursor = 0;
			
	}
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_UP))
	{
		if (m_iCursor == 1)
			m_iCursor = 0;
		else
			m_iCursor = 1;

	}






	return OBJ_NOEVENT;
}

void CUI_SystemMessageBox::Late_Tick(_float fTimeDelta)
{


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	//	}



}

HRESULT CUI_SystemMessageBox::Render()
{
	//	if (!m_btick)
	//		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/*if (m_bDeadtimeron)
	{
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fAlpha1, sizeof(_float))))
	return E_FAIL;
	}*/
	//	m_fAlpha = 1.f;
	//_float alpha = 0.4f;
	_float alpha = m_fAlpha * 0.6f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 400.f;
	m_fSize.y = 180.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_UV_sizeX", &m_fSize.x, sizeof(_float))))
	return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_UV_sizeY", &m_fSize.y, sizeof(_float))))
	return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_MiddlePoint", &m_fPosition, sizeof(_float2))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_OUTLINE);

	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 462.f;
	m_fPosition.y = 290.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	m_fPosition.x = 817.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	m_fPosition.y = 430.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	m_fPosition.x = 462.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(6))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	


	//m_fSize.x = 128.f;
	//m_fSize.y = 128.f;
	//m_fPosition.x = 650.f;
	//m_fPosition.y = 340.f;// +m_fFadeY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(1))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_ALPHASET);

	//m_pVIBufferCom->Render();

	m_fSize.x = 280.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 690.f;
	m_fPosition.y = 300.f;// +m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 280.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 670.f;
	m_fPosition.y = 370.f;// +m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.y = 400.f;// +m_fFadeY;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();




	
	m_fPosition.x = 490.f;
	m_fSize.x = 48.f ;
	m_fSize.y = 48.f ;

	if (m_iCursor == 0 )

		m_fPosition.y = 370.f;


	else if(m_iCursor == 1)
		m_fPosition.y = 400.f;
	

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_SystemMessageBox::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITemMsgobox"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMICON"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_back"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMINFO"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_systemmessage"), (CComponent**)&m_pTextureCom5)))
		return E_FAIL;



	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTNAME"), (CComponent**)&m_pTextureCom5)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CUI_SystemMessageBox::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
	return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
	return E_FAIL;*/


	return S_OK;
}








CUI_SystemMessageBox * CUI_SystemMessageBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_SystemMessageBox*	pInstance = new CUI_SystemMessageBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_SystemMessageBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_SystemMessageBox::Clone(void * pArg)
{
	CUI_SystemMessageBox*	pInstance = new CUI_SystemMessageBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_SystemMessageBox"));
		Safe_Release(pInstance);
	}

	//CUI_Manager::Get_Instance()->Set_SystemMsg(pInstance);

	return pInstance;
}

void CUI_SystemMessageBox::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTextureCom5);
	__super::Free();
}


