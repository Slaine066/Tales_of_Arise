
#include "Client_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
texture2D       g_GradationTexture;
float			g_fAlpha = 1.f;
float           g_fAlpha1 = 1.f;

float           g_fBright = 0.f;

float			g_fMinRange = 100.f;
float			g_fMaxRange = 400.f;
float a = 0.f;
float g_fAngle; 
float g_fAngle1;

float g_fCurrentHp;
float g_fMaxHp;

float4			g_PlayerProjPos;
float2			g_TexUV;



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

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* �̷��� ������� �ȼ��� PS_MAIN�Լ��� ���ڷ� ������. */
/* �����ϴ� ���� Target0 == ��ġ�� 0��°�� ���ε��Ǿ��ִ� ����Ÿ��(�Ϲ������� �����)�� �׸���. */
/* �׷��� ����ۿ� ���� �׷�����. */

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a <= 0.3f)
		discard;

	return Out;
}

PS_OUT PS_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_PICKED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb += 0.1f;
	Out.vColor.rg += 0.1f;
	Out.vColor.a *= g_fAlpha;
	return Out;
}


PS_OUT PS_SCREEN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor.rgb = 0.f;
	Out.vColor.a = g_fAlpha;

	return Out;
}

PS_OUT PS_FADEOUT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_HPbar(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (In.vTexUV.y > (1.4f - In.vTexUV.x) + (In.vTexUV.y))
		discard;
	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;
	
	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	

	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float  fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


	if (Out.vColor.a<0.3f)
		discard;

	return Out;
}

PS_OUT PS_COMBOLINE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	/*if (Out.vColor.a <= 0.3f)
		discard;

	if (Out.vColor.r == 0.f&&Out.vColor.g == 0.f&& Out.vColor.b ==0.f )
		discard;

	Out.vColor.r += 0.929f;
	Out.vColor.g += 0.8f;
	Out.vColor.b += 0.486f;*/
	float3 yellowRef = normalize(float3(.69f, .62f, .42f)); //�ؽ��Ļ�


	float weight = dot(Out.vColor.rgb, yellowRef); //���ĸ����
	Out.vColor.a = lerp(0, 1, weight);

	Out.vColor.a = lerp(0, 1, saturate(weight));


	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_Golden(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*if ((g_fCurrentHp / g_fMaxHp) < In.vTexUV.x)
		discard;*/

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.r >= 0.7f && origincolor.g >= 0.7f && origincolor.b >= 0.7f)//if (origincolor.a > 0.2f)// //
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.9882f, 0.9843f, 0.9176f, 1.f), float4(0.9764f, 0.9921f, 0.8941f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	if (Out.vColor.a < 0.2f)
		discard;

	Out.vColor.a -= g_fAlpha;

	return Out;
}

PS_OUT PS_CPguage(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (1 - g_fCurrentHp / g_fMaxHp > In.vTexUV.y)
		discard;

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);



	/*if (Out.vColor.a<0.1f)
		discard;*/

	return Out;
}


PS_OUT PS_CPguageblack(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	return Out;
}

PS_OUT PS_MPGUAGE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (g_fCurrentHp / 1.f < In.vTexUV.x)
		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;
	if (Out.vColor.a <= 0.1)
		discard;
		
	return Out;
}

PS_OUT PS_BLACKCOLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0;
	Out.vColor.g = 0;
	Out.vColor.b = 0;

	return Out;
}

PS_OUT PS_PORTRAITDARK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	if (Out.vColor.a <= 0.3f)
		discard;

	Out.vColor.a -= g_fAlpha;
	Out.vColor.r -= 0.15f;
	Out.vColor.g -= 0.15f;
	Out.vColor.b -= 0.15f;

	

	return Out;
}

PS_OUT PS_POREADY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	/*if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;*/


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.rgb += a;

	Out.vColor.a -= g_fAlpha;
	Out.vColor.r = max(0.9372549019607843f, Out.vColor.r);
	Out.vColor.g = max(0.8745098039215686f, Out.vColor.g); 
	Out.vColor.b = max(0.7647058823529412f, Out.vColor.b); 


	/*if (Out.vColor.a == 0)
		discard;*/
	return Out;
}

PS_OUT PS_ALLBLUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

//	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
//		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;

	return Out;
}


PS_OUT PS_RUNECOLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
	//		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.9803921568627451f;
	Out.vColor.g = 0.9019607843137255f;
	Out.vColor.b = 0.7607843137254902f;


	if (Out.vColor.a < 0.2f)
		discard;

	Out.vColor.a -= g_fAlpha;

	return Out;
}


PS_OUT PS_REVERSELOCKON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (1-(g_fCurrentHp / 1.f )> In.vTexUV.x)
		discard;


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//Out.vColor.a -= g_fAlpha;
	Out.vColor.r = 0.0589019607843137f;
	Out.vColor.g = 0.942708f;
	Out.vColor.b = 0.83441f;

	if (Out.vColor.a <= 0.1)
	discard;

	return Out;
}

PS_OUT PS_UVROTATION(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b && 0)
		return Out;*/
	

	float2x2 rotationMatrix = float2x2(cos(g_fAngle), -sin(g_fAngle), sin(g_fAngle), cos(g_fAngle));

	if (In.vTexUV.x < 0.15f || In.vTexUV.x > 0.85f)
		discard;

	if (In.vTexUV.y < 0.2f || In.vTexUV.y > 0.8f)
		discard;

	float2 rotatedUV = mul(rotationMatrix, In.vTexUV -0.5f) + 0.5f;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, rotatedUV);

	Out.vColor.a = Out.vColor.r;

	//if (Out.vColor.r >= 0.2f)
		Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);

		


	Out.vColor.a -= 0.2f;

	return Out;
}

PS_OUT PS_INVENICON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	


	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	
	if (Out.vColor.r < 0.53f || Out.vColor.g < 0.5f || Out.vColor.b < 0.5f)
		discard;
	else
	{

		Out.vColor.r = 0.9372549019607843f;
		Out.vColor.g = 0.8745098039215686f;
		Out.vColor.b = 0.7647058823529412f;
	}

	float fGradientRadius = 0.6f;
	float fGradientStrength = 0.25f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}



	Out.vColor.rgb += fLerp;   //fLerpValue;
	
		
		
		
	Out.vColor.a -= 0.3f;
	

	return Out;
}

PS_OUT PS_INVENBACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.r;
	Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);
	//if (Out.vColor.r < 0.53f || Out.vColor.g < 0.5f || Out.vColor.b < 0.5f)
	//	discard;
	//else
	//{

	//	Out.vColor.r = 0.9372549019607843f;
	//	Out.vColor.g = 0.8745098039215686f;
	//	Out.vColor.b = 0.7647058823529412f;
	//	//	Out.vColor.r = max(0.9372549019607843f, Out.vColor.r);
	//	//	Out.vColor.g = max(0.8745098039215686f, Out.vColor.g);
	//	//	Out.vColor.b = max(0.7647058823529412f, Out.vColor.b);
	//}




	Out.vColor.a -= 0.3f;


	return Out;
}

PS_OUT PS_MENULINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

	
	
	//origincolor.a = maskcolor.a;
	//Out.vColor = origincolor;

	float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

	Out.vColor = lerpcolor;
	Out.vColor.a = maskcolor.a;
	


		if (Out.vColor.a == 0.f)
			discard;
	/*if (Out.vColor.a<0.3f)
		discard;*/

	return Out;
}


PS_OUT PS_INVENTOPBOTTOM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;
	Out.vColor.a = 1.f;

	Out.vColor.a -= 0.4f;


	return Out;
}

PS_OUT PS_INVENTOPBOTTOMALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;




	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	Out.vColor.r = 0.f;
	Out.vColor.g = 0.f;
	Out.vColor.b = 0.f;
	Out.vColor.a = 0.6f;

	Out.vColor.a -= g_fAlpha1;


	return Out;
}

PS_OUT PS_GALDBACK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a -= Out.vColor.r + 0.2f;

	Out.vColor.rgb -= 0.2f;
	/*if (Out.vColor.a <= 1.f)
		discard;*/

	return Out;
}

PS_OUT PS_UVROTATIONSTRONG(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	/*Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (Out.vColor.r == 0 && Out.vColor.g == 0 && Out.vColor.b && 0)
	return Out;*/


	float2x2 rotationMatrix = float2x2(cos(g_fAngle), -sin(g_fAngle1), sin(g_fAngle1), cos(g_fAngle1));

	if (In.vTexUV.x < 0.15f || In.vTexUV.x > 0.85f)
		discard;

	if (In.vTexUV.y < 0.2f || In.vTexUV.y > 0.8f)
		discard;

	float2 rotatedUV = mul(rotationMatrix, In.vTexUV - 0.5f) + 0.5f;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, rotatedUV);

	Out.vColor.a = Out.vColor.r;

	//if (Out.vColor.r >= 0.2f)
	Out.vColor.rgb = float3(0.6f, 0.5f, 0.3f);//(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);




	Out.vColor.a += 0.15f;

	return Out;


}

PS_OUT PS_USINGITEMPORTRAIT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;



	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	////Out.vColor.a = Out.vColor.r;

	////if (Out.vColor.r >= 0.2f)
	//Out.vColor.rgb = (0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f);

	float4 lerpcolor = lerp(float4(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f, 1.f), float4(0.9f, 0.8352f, 0.9f, 1.f), Out.vColor);


	
	Out.vColor = lerpcolor;
	Out.vColor.a -= 0.5f;
	return Out;


}

PS_OUT PS_LIGHTEFFECT(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;



	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.g;

	Out.vColor.rgb = float3(0.5882352941176471f, 0.5372549019607843f, 0.5f);
	//float4 lerpcolor = lerp(float4(0.5882352941176471f, 0.5372549019607843f, 0.3686274509803922f, 1.f), float4(0.9f, 0.8352f, 0.9f, 1.f), Out.vColor);

	Out.vColor.a -= 0.25f;
	
	//Out.vColor.rgb = lerpcolor.rgb;
	
	return Out;


}

PS_OUT PS_Bright(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.3f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}

	Out.vColor.rgb += fLerp;   //fLerpValue;


	if (Out.vColor.a<0.3f)
		discard;

	Out.vColor.a *= g_fAlpha;

	return Out;

	
}

PS_OUT PS_HPbarBLUE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (g_fCurrentHp / g_fMaxHp < In.vTexUV.x)
		discard;

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (origincolor.a > 0.4f)
	{
		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;
	}
	else
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	float fGradientRadius = 0.5f;
	float fGradientStrength = 0.2f;
	float fLerp = 0.f;

	/* First Lerp */
	if (In.vTexUV.x > g_fBright - fGradientRadius && In.vTexUV.x < g_fBright)
	{
		float fStart = g_fBright - fGradientRadius;
		float fEnd = g_fBright;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(0, fGradientStrength, fInterpFactor);
	}
	/* Second Lerp */
	else if (In.vTexUV.x > g_fBright && In.vTexUV.x < g_fBright + fGradientRadius)
	{
		float fStart = g_fBright;
		float fEnd = g_fBright + fGradientRadius;
		float fInterpFactor = (In.vTexUV.x - fStart) / (fEnd - fStart);
		fLerp = lerp(fGradientStrength, 0, fInterpFactor);
	}
	Out.vColor.b = 1.f;
	Out.vColor.r -= 0.13f;
	Out.vColor.g += 0.05f;
	//Out.vColor.g += 0.2f;
	Out.vColor.rgb += fLerp;   //fLerpValue;


	if (Out.vColor.a<0.3f)
		discard;

	return Out;
}

PS_OUT PS_Dialoguebox(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.g;

	if (Out.vColor.r == 0)
		discard;
	

	Out.vColor.rgb = 0.f;
	Out.vColor.a *= g_fAlpha;


	return Out;


}
PS_OUT PS_DialogueLINE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a = Out.vColor.r;

	if (Out.vColor.r == 0)
		discard;

    float4 lerpcolor = lerp(float4(0.9019607843137255f, 0.8235294117647059f, 0.6588235294117647f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), Out.vColor);

	Out.vColor.rgb = lerpcolor.rgb;
	Out.vColor.a *= g_fAlpha;

	return Out;
}

PS_OUT PS_DIALOGUECURSOR(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;

	if (1 - g_fCurrentHp / g_fMaxHp > In.vTexUV.y)
		discard;

	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (origincolor.r == 0)
		discard;

		float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

		float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

		Out.vColor = lerpcolor;

		Out.vColor.a *= g_fAlpha;

	return Out;
}
PS_OUT PS_DIALOGUECURSORNOTMOVE(PS_IN In)
{
	PS_OUT      Out = (PS_OUT)0;


	float4 origincolor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (origincolor.r == 0)
		discard;

	float4 maskcolor = g_GradationTexture.Sample(LinearSampler, In.vTexUV);

	float4 lerpcolor = lerp(float4(0.701f, 0.784f, 0.545f, 1.f), float4(0.7882f, 0.8352f, 0.647f, 1.f), maskcolor);

	Out.vColor = lerpcolor;

	Out.vColor.a *= g_fAlpha;

	return Out;
}


technique11 DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Alpha_Blending
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Picked
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PICKED();
	}

	pass Alpha_Set
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA();
	}

	pass Alpha_Screen
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SCREEN();
	}

	pass FADEOUT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FADEOUT();
	}

	pass HPbar
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPbar();
	}

	pass COMBOLINE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBOLINE();
	}

	pass Golden
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Golden();
	}

	pass CPguage
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CPguage();
	}

	pass CPguageblack
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CPguageblack();
	}

	pass MPGUAGE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MPGUAGE();
	}

	pass BLACKCOLOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLACKCOLOR();
	}

	pass PORTRAITDARK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_PORTRAITDARK();
	}

	pass POREADY
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_POREADY();
	}

	pass PS_ALLBLUE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALLBLUE();
	}

	pass RUNECOLOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RUNECOLOR();
	}


	pass REVERSELOCKON
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_REVERSELOCKON();
	}

	pass UVROT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UVROTATION();
	}

	pass INVENICON
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENICON();
	}

	pass INVENBACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENBACK();
	}

	pass MENULINE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MENULINE();
	}

	pass INVENTOPBOTTOM
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENTOPBOTTOM();
	}

	pass INVENTOPBOTTOMALPHA
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_INVENTOPBOTTOMALPHA();
	}

	pass GALDBACK
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GALDBACK();
	}
	
	pass UVROTATIONSTRONG
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UVROTATIONSTRONG();
	}
	
	pass USINGITEMPORTRAIT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USINGITEMPORTRAIT();
	}

	pass LIGHTEFFECT
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LIGHTEFFECT();
	}

	pass Bright
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Bright();
	}

	pass BLUEHPBAR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPbarBLUE();                //29
	}

	pass Dialoguebox
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Dialoguebox();                //30
	}

	pass Dialogueline
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DialogueLINE();                //31
	}
	
	pass PS_DIALOGUECURSOR
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DIALOGUECURSOR();                //32
	}

	pass PS_DIALOGUECURSORNONMOVE
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Priority, 0);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DIALOGUECURSORNOTMOVE();                //33
	}
	
	
	
	
	
}


