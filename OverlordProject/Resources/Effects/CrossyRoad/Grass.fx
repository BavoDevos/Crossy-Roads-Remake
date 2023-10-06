float4x4 gViewProj;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorldViewProj_Light;
float gShadowMapBias = 0.0001f;
float gWindOffset;
Texture2D gShadowMap;

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
	DepthEnable = FALSE;
};

RasterizerState BackCulling
{
	CullMode = NONE;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float4 lPos : TEXCOORD1;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col)
{

	//Geometry Vertex Output
	GS_DATA geomData = (GS_DATA)0;
	geomData.Position = mul(float4(pos, 1.0f), gWorldViewProj);
	geomData.Color = col;
	geomData.lPos = mul(float4(pos, 1.f), gWorldViewProj_Light);
	triStream.Append(geomData);
}

[maxvertexcount(3)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	float3 p = vertex[0].Position; //Extract the position data from the VS_DATA vertex struct

	float width = 0.03f;
	float height = 0.3f;

	float4 col = vertex[0].Color;

	//VERTEX 1 [LT]
	CreateVertex(triStream, float3(p.x - width, p.y, p.z), col); //Change the color data too!

	//VERTEX 2 [RT]
	CreateVertex(triStream, float3(p.x + gWindOffset, p.y + height, p.z), col); //Change the color data too!

	//VERTEX 3 [LB]
	CreateVertex(triStream, float3(p.x + width, p.y, p.z), col); //Change the color data too!

}

//PIXEL SHADER
//************

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
	const float w = 1280.f;
	const float h = 720.f;

	return float2(u * 1.0f / w, v * 1.0f / h);
}

float EvaluateShadowMap(float4 lpos)
{
	//TODO: complete

	const float shadowVal = 0.5f;
	const float lightVal = 1.f;

	//re-homogenize
	lpos.xyz /= lpos.w;

	//check if position is visible
	if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f) return shadowVal;


	lpos.x = lpos.x / 2 + 0.5;
	lpos.y = lpos.y / -2 + 0.5;

	lpos.z = lpos.z - gShadowMapBias;


	float sum = 0;
	float x, y;


	for (y = -1.5f; y <= 1.5f; y += 1.0f)
	{
		for (x = -1.5f; x <= 1.5f; x += 1.0f)
		{
			sum = sum + gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}

	float shadowFactor = sum / 16.f;


	//float shadowFactor = gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);

	shadowFactor = lerp(shadowVal, lightVal, shadowFactor);

	return shadowFactor;

}


float4 MainPS(GS_DATA input) : SV_TARGET
{
	float shadow = EvaluateShadowMap(input.lPos);

	float4 finalColor = float4(input.Color.rgb * shadow, input.Color.a);
	return finalColor;
}

// Default Technique
technique10 Default
{
	pass p0
	{
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetDepthStencilState(NoDepth,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
