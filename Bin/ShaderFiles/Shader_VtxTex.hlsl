
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;

float3 g_vColor;
float g_fAlpha;
float g_fAlphaDiscard;
texture2D g_MaskTexture;
texture2D g_NoiseTexture;
texture2D g_DissolveTexture;
float g_fNoiseSpeed;
float g_fNoisePower;
float g_fTimer = 0.f;

texture2D g_DepthTexture;
texture2D g_GlowTexture;
bool g_bGlow = false;
float4 g_GlowColor;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

/* DrawIndexed�Լ��� ȣ���ϸ�. */
/* �ε������ۿ� ����ִ� �ε�����°�� ������ VS_MAIN�Լ��� ���ڷ� ������. VS_IN�� ����ȴ�. */
/* �Ϲ������� TriangleList�� �׸����, ���� ������ ���� VS_MAIN�Լ��� ���ڷ� ������. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* ������ ��ġ�� ���� �� ��������� ���Ѵ�. ���� ������ ViewSpace�� �����ϳ�. */
	/* ������ı��� ���ϸ� ������ġ�� w�� �佺���̽� ���� z�� �����Ѵ�. == Out.vPosition�� �ݵ�� float4�̾���ϴ� ����. */
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

/* TriangleList�� �׷ȴٸ� ���� ������ VS_MAIN�� ������ ��� ��ģ ����. */

/* w������ ������ �����Ͽ� ���������̽��� ����. */

/* !!!!!!!!!!!!!!!!���. ���������� ��� ������ ���ؼ��� ���� ������ �����Ѵ�. */

/* ����Ʈ�����̽�(Ŭ���̾�Ʈ���� == ��������ǥ)���� ��ȯ�� �����Ѵ�. */
/* �����Ͷ���� �����Ѵ�. == ������ ������ ��������Ͽ� �ȼ��� �����Ѵ�. VS_OUT == PS_IN�� ���� ����. */

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

struct PS_OUT_GLOW
{
	float4 vGlow : SV_TARGET0;
};

/* �̷��� ������� �ȼ��� PS_MAIN�Լ��� ���ڷ� ������. */
/* �����ϴ� ���� Target0 == ��ġ�� 0��°�� ���ε��Ǿ��ִ� ����Ÿ��(�Ϲ������� �����)�� �׸���. */
/* �׷��� ����ۿ� ���� �׷�����. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	
	return Out;
}

PS_OUT PS_ALPHAMASK(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.gba = Out.vColor.r;

	Out.vColor.rgb *= g_vColor; // Set Color
	Out.vColor.a *= g_fAlpha;

	if (Out.vColor.a < g_fAlphaDiscard) // Alpha Test
		discard;

	return Out;
}

PS_OUT_GLOW PS_GLOW(PS_IN In)
{
	PS_OUT_GLOW Out = (PS_OUT_GLOW)0;

	//float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//float4 vGlow = g_GlowTexture.Sample(LinearSampler, In.vTexUV) * g_bGlow;
	//float4 vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	//float fDepth = vDepth.y * 1000.f;

	//if (In.vProjPos.w > fDepth)
	//	discard;

	//if (Out.vGlow.r != 0 && Out.vGlow.g != 0 && Out.vGlow.b != 0)
	//	Out.vGlow.rgb = float3(1.f, 1.f, 1.f) /*Out.vDiffuse.rgb * Out.vGlow.r;*/;

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

	pass UI
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Effect_AlphaMask
	{
		SetRasterizerState(RS_Default_NoCull);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHAMASK();
	}

	pass Glow // 3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOW();
	}
}