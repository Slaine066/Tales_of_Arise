#include "stdafx.h"

#include "Effect.h"
#include "GameInstance.h"
#include "EffectTexture.h"
#include "EffectMesh.h"
#include "ParticleSystem.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void * pArg)
{
	return S_OK;
}

int CEffect::Tick(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CEffect::Late_Tick(_float fTimeDelta)
{
}

HRESULT CEffect::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CEffect::PlayEffect(_tchar * wcEffectName, _vector vPosition)
{
	/* Load Effect File. */
	HANDLE hFileEffect = nullptr;
	_tchar LoadPathEffect[MAX_PATH] = TEXT("../../../Bin/Data/EffectData/");
	wcscat_s(LoadPathEffect, MAX_PATH, wcEffectName);

	hFileEffect = CreateFile(LoadPathEffect, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileEffect == INVALID_HANDLE_VALUE)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DWORD dwByte = 0;
	_uint iEffectsCount = 0;

	/* Read how many Effects there are in this File. */
	ReadFile(hFileEffect, &iEffectsCount, sizeof(_uint), &dwByte, nullptr);

	while (true)
	{
		if (!dwByte)
			break;

		/* For every Effect in this File: */
		for (_uint i = 0; i < iEffectsCount; i++)
		{
			/* Read Effect Type. */
			CEffect::EFFECT_TYPE eType;
			ReadFile(hFileEffect, &eType, sizeof(CEffect::EFFECT_TYPE), &dwByte, nullptr);

			if (!dwByte)
				break;

			CEffectTexture::TEXTUREEFFECTDESC tTextureEffectDesc;
			CEffectMesh::MESHEFFECTDESC tMeshEffectDesc;
			CParticleSystem::PARTICLEDESC tParticleDesc;
			CEffect* pEffect = nullptr;

			/* Read Effect Type-specific Description. */
			switch (eType)
			{
				case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
				{
					ReadFile(hFileEffect, &tTextureEffectDesc, sizeof(CEffectTexture::TEXTUREEFFECTDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectTexture"), LEVEL_STATIC, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tTextureEffectDesc);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_TEXTURE);
					break;
				}
				case CEffect::EFFECT_TYPE::TYPE_MESH:
				{
					ReadFile(hFileEffect, &tMeshEffectDesc, sizeof(CEffectMesh::MESHEFFECTDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectMesh"), LEVEL_STATIC, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tMeshEffectDesc);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_MESH);
					break;
				}
				case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
				{
					ReadFile(hFileEffect, &tParticleDesc, sizeof(CParticleSystem::PARTICLEDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_ParticleSystem"), LEVEL_STATIC, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tParticleDesc);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_PARTICLE);
					break;
				}
			}

			pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);

			/* Read how many Velocity Curves there are for this Effect. */
			_uint iVelocityCurvesCount = 0;
			ReadFile(hFileEffect, &iVelocityCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Velocity Curves. */
			vector<_float3> VelocityCurves;
			_float3 VelocityCurve;
			for (_uint j = 0; j < iVelocityCurvesCount; j++)
			{
				ReadFile(hFileEffect, &VelocityCurve, sizeof(_float3), &dwByte, nullptr);
				VelocityCurves.push_back(VelocityCurve);
			}
			if (!VelocityCurves.empty())
				pEffect->Set_VelocityCurves(VelocityCurves);

			/* Read how many Size Curves there are for this Effect. */
			_uint iSizeCurvesCount = 0;
			ReadFile(hFileEffect, &iSizeCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Size Curves. */
			vector<_float3> SizeCurves;
			_float3 SizeCurve;
			for (_uint j = 0; j < iSizeCurvesCount; j++)
			{
				ReadFile(hFileEffect, &SizeCurve, sizeof(_float3), &dwByte, nullptr);
				SizeCurves.push_back(SizeCurve);
			}
			if (!SizeCurves.empty())
				pEffect->Set_SizeCurves(SizeCurves);

			/* Read how many Alpha Curves there are for this Effect. */
			_uint iAlphaCurvesCount = 0;
			ReadFile(hFileEffect, &iAlphaCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Alpha Curves. */
			vector<_float3> AlphaCurves;
			_float3 AlphaCurve;
			for (_uint j = 0; j < iAlphaCurvesCount; j++)
			{
				ReadFile(hFileEffect, &AlphaCurve, sizeof(_float3), &dwByte, nullptr);
				AlphaCurves.push_back(AlphaCurve);
			}
			if (!AlphaCurves.empty())
				pEffect->Set_AlphaCurves(AlphaCurves);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	CloseHandle(hFileEffect);
}

HRESULT CEffect::Ready_Components(void * pArg)
{
	return S_OK;
}

HRESULT CEffect::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}