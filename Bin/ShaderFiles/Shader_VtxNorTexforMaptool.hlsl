
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vCamPosition;

/* For.Material */
texture2D		g_DiffuseTexture[4];

texture2D		g_BrushTexture;
float4			g_vBrushPos = float4(5.f, 0.f, 5.f, 1.f);
float			g_fBrushRange = 2.f;


texture2D		g_FilterTexture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(vWorldNormal);
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = vWorldPos;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse1 = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDiffuse2 = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDiffuse3 = g_DiffuseTexture[2].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDiffuse4 = g_DiffuseTexture[3].Sample(LinearSampler, In.vTexUV * 30.f);

	vector		vFilter = g_FilterTexture.Sample(LinearSampler, In.vTexUV);

	vector		vBrush = vector(0.f, 0.f, 0.f, 0.f);

	if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushRange)
	{
		float2		fNewUV;

		fNewUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		fNewUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(LinearSampler, fNewUV);
		vBrush.a = vBrush.r;
	}

	vector		vMtrlDiffuse = vDiffuse1 * vFilter.r + vDiffuse2 * vFilter.g + vDiffuse3 * vFilter.b;
	Out.vDiffuse = (1-vFilter.r - vFilter.g - vFilter.b)*vDiffuse4 + vMtrlDiffuse + vBrush;

	/*if (Out.vDiffuse.a == 0.0f)
	discard;*/

	Out.vDiffuse.a = 1.f;

	/* -1 ~ 1 => 0 ~ 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	/* near ~ far */
	/* 0 ~ far */
	/* 0 ~ 1 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);

	return Out;
}




PS_OUT PS_WIRE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse1 = vector(1.f, 0.f, 0.f, 1.f);
	vector		vDiffuse2 = vector(0.f, 1.f, 0.f, 1.f);
	vector		vDiffuse3 = vector(0.f, 0.f, 1.f, 1.f);
	vector		vDiffuse4 = vector(0.f, 0.f, 0.f, 1.f);

	/* -1 ~ 1 => 0 ~ 1*/

	vector		vFilter = g_FilterTexture.Sample(PointSampler, In.vTexUV);
	vector		vBrush = vector(0.f,0.f,0.f,0.f);

	if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushRange)
	{
		float2		fNewUV;

		fNewUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		fNewUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(LinearSampler, fNewUV);
		vBrush.a = vBrush.r;
	}

	vector		vMtrlDiffuse = vFilter.a* vDiffuse4 + vDiffuse1 * vFilter.r + vDiffuse2 * vFilter.g + vDiffuse3 * vFilter.b ;
	Out.vDiffuse = vMtrlDiffuse + vBrush;


	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);
	return Out;
}


PS_OUT PS_PICKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexUV);

	Out.vDiffuse = vSourDiffuse;
	Out.vDiffuse.rgb += 0.1f;
	Out.vDiffuse.a = 1.f;

	/* -1 ~ 1 => 0 ~ 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);
	return Out;
}


PS_OUT PS_BRUSH(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexUV * 30.f);
	vector		vFilter = g_FilterTexture.Sample(PointSampler, In.vTexUV);

	vector		vBrush = vector(0.f, 0.f, 0.f, 0.f);

	if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushRange)
	{
		float2		fNewUV;

		fNewUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		fNewUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(LinearSampler, fNewUV);
	}

	vector		vMtrlDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter);
	Out.vDiffuse = vMtrlDiffuse + vBrush;

	/*if (Out.vDiffuse.a == 0.0f)
	discard;*/

	Out.vDiffuse.a = 1.f;

	/* -1 ~ 1 => 0 ~ 1*/
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	/* near ~ far */
	/* 0 ~ far */
	/* 0 ~ 1 */
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.f, 0.f, 0.f);

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass WireFrame
	{
		SetRasterizerState(RS_Wireframe);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_WIRE();
	}

	pass Picked
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PICKED();
	}

	pass Brush
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BRUSH();
	}

}