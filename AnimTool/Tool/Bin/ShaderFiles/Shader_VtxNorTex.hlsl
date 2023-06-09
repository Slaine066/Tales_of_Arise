
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vCamPosition;

float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vLightSpecular;

float4 g_vLightDir;

float4 g_vLightPos;
float g_fLightRange;

texture2D g_DiffuseTexture;
float4 g_vMtrlAmbient = float4(1.f, 1.f, 1.f, 1.f);
float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float fShade : COLOR0;
	float fSpecular : COLOR1;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN_DIRECTIONAL(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = In.vNormal;

	vector vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);

	Out.fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);

	vector vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vector vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector vLook = vWorldPos - g_vCamPosition;

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 25);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = vWorldPos;

	return Out;
}

VS_OUT VS_MAIN_POINT(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = In.vNormal;

	vector vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	vector vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vector vLightDir = vWorldPos - g_vLightPos;

	float fDistance = length(vLightDir);

	float fAtt = max(g_fLightRange - fDistance, 0.f) / g_fLightRange;

	Out.fShade = max(dot(normalize(vLightDir) * -1.f, normalize(vWorldNormal)), 0.f) * fAtt;

	vector vReflect = reflect(normalize(vLightDir), normalize(vWorldNormal));
	vector vLook = vWorldPos - g_vCamPosition;

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 25) * fAtt;

	Out.vTexUV = In.vTexUV * 30.f;

	return Out;
}

struct VS_OUT_PHONG
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

VS_OUT_PHONG VS_MAIN_PHONG(VS_IN In)
{
	VS_OUT_PHONG Out = (VS_OUT_PHONG)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vNormal = vWorldNormal;

	Out.vTexUV = In.vTexUV * 30.f;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float fShade : COLOR0;
	float fSpecular : COLOR1;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV * 30.f);

	Out.vColor = (g_vLightDiffuse * vDiffuse) * saturate(In.fShade + g_vLightAmbient * g_vMtrlAmbient) + (g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	return Out;
}

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = (g_vLightDiffuse * vector(0.f, 0.f, 0.f, 1.f)) * saturate(In.fShade + g_vLightAmbient * g_vMtrlAmbient) + (g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	return Out;
}

struct PS_IN_PHONG
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

PS_OUT PS_MAIN_PHONG(PS_IN_PHONG In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	vector vLook = In.vWorldPos - g_vCamPosition;
	float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 25);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + g_vLightAmbient * g_vMtrlAmbient) + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	return Out;
}

technique11 DefaultTechnique
{
	pass Directional_Terrain_Solid
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}
	pass Point_Terrain_Solid
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_POINT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}
	pass Directional_Terrain_Wire
	{
		SetRasterizerState(RS_WireFrame);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}
	pass Point_Terrain_Wire
	{
		SetRasterizerState(RS_WireFrame);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_POINT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}

	pass Directional
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Point
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_POINT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Phong
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN_PHONG();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_PHONG();
	}
}