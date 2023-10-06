float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gDiffuseMap;
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

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//TODO: complete Vertex Shader 
	//Hint: use the previously made shaders PosNormTex3D_Shadow and PosNormTex3D_Skinned as a guide

	float4 blendedPosition;
	float3 blendedNormal;

	for (int i = 0; i < 4; i++)
	{
		int boneIndex = input.BoneIndices[i];
		if (boneIndex >= 0)
		{
			//tranform the vertex based on the relevant bone and boneWeight
			float4 boneSpacePos = mul(float4(input.pos, 1.f), gBones[boneIndex]);
			float3 boneSpaceNormal = mul(input.normal, (float3x3)gBones[boneIndex]);


			//add the position and normal to the blended Position/normal according to the weight
			blendedPosition = blendedPosition + mul(boneSpacePos, input.BoneWeights[i]);
			blendedNormal = blendedNormal + mul(boneSpaceNormal, input.BoneWeights[i]);

		}
	}

	//make sure the blended position w-component is 1
	blendedPosition.w = 1.f;

	output.pos = mul(float4(blendedPosition.xyz, 1.f), gWorldViewProj);
	output.normal = normalize(mul(blendedNormal, (float3x3)gWorld));
	output.texCoord = input.texCoord;
	output.lPos = mul(float4(blendedPosition.xyz, 1.f), gWorldViewProj_Light);
	return output;

}

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
	const float w = 1280;
	const float h = 720;

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

	lpos.z -= gShadowMapBias;

	float sum = 0;
	float x, y;

	for (y = -1.5; y <= 1.5; y += 1.0)
	{
		for (x = -1.5; x <= 1.5; x += 1.0)
		{
			sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}

	float shadowFactor = sum / 16.f;

	shadowFactor = lerp(0.3f, 1.f, shadowFactor);
	return shadowFactor;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample(samLinear,input.texCoord);
	float3 color_rgb = diffuseColor.rgb;
	float color_a = diffuseColor.a;

	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;


	return float4(color_rgb * shadowValue, color_a);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

