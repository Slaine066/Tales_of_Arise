#include "stdafx.h"

#include "EffectTexture.h"
#include "GameInstance.h"

CEffectTexture::CEffectTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CEffectTexture::CEffectTexture(const CEffectTexture & rhs)
	: CEffect(rhs)
{
}

HRESULT CEffectTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectTexture::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

int CEffectTexture::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	/* Billboard */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pTransformCom->LookAt(XMLoadFloat4(&pGameInstance->Get_CamPosition()));
	RELEASE_INSTANCE(CGameInstance);

	m_fTimer += fTimeDelta;

	return OBJ_NOEVENT;
}

void CEffectTexture::Late_Tick(_float fTimeDelta)
{
	if (!Check_IsinFrustum())
		return;

	if (m_pRendererCom)
	{
		if (m_tTextureEffectDesc.bIsDistortion)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this);
		else
		{
			Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
		}
	}
}

HRESULT CEffectTexture::Render()
{
	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL;

	__super::Render();

	if (m_tTextureEffectDesc.bIsDistortion)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DistortionTexture", pGameInstance->Get_BackBufferCopySRV())))
			return E_FAIL;

		RELEASE_INSTANCE(CGameInstance);

		m_pShaderCom->Begin(SHADER_DISTORTION);
	}
	else
		m_pShaderCom->Begin(SHADER_ALPHAMASK);
	
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffectTexture::Ready_Components(void * pArg)
{
	memcpy(&m_tTextureEffectDesc, (TEXTUREEFFECTDESC*)pArg, sizeof(TEXTUREEFFECTDESC));

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 0.1f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, m_tTextureEffectDesc.wcPrototypeId, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (m_tTextureEffectDesc.bIsDistortion)
	{
		/* For.Com_NoiseTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_NoiseTexture"), LEVEL_STATIC, TEXT("Distortion_Noise"), (CComponent**)&m_pNoiseTextureCom)))
			return E_FAIL;
		/* For.Com_StrengthTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_StrengthTexture"), LEVEL_STATIC, TEXT("Distortion_Strength"), (CComponent**)&m_pStrengthTextureCom)))
			return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
}

HRESULT CEffectTexture::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (m_tTextureEffectDesc.bIsDistortion)
		SetUp_ShaderResources_Distortion();
	else
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV())))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CEffectTexture::SetUp_ShaderResources_Distortion()
{
	if (FAILED(m_pShaderCom->Set_RawValue("g_iWinX", &g_iWinSizeX, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_iWinY", &g_iWinSizeY, sizeof(_uint))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTextureCom->Get_SRV())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_StrengthTexture", m_pStrengthTextureCom->Get_SRV())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fDistortionTimer", &m_fTimer, sizeof(_float))))
		return E_FAIL;
}

CEffectTexture * CEffectTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffectTexture* pInstance = new CEffectTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffectTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffectTexture::Clone(void * pArg)
{
	CEffectTexture* pInstance = new CEffectTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffectTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectTexture::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pNoiseTextureCom);
	Safe_Release(m_pStrengthTextureCom);
}