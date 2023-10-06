float4x4 gWorld;
float4x4 gLightViewProj;
float4x4 gBones[70];
 
DepthStencilState depthStencilState
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState rasterizerState
{
	FillMode = SOLID;
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader [STATIC]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS(float3 position:POSITION) :SV_POSITION
{
	//TODO: return the position of the vertex in correct space (hint: seen from the view of the light)
	float4 output = (float4)0;
	output = mul(float4(position, 1.f), mul(gWorld, gLightViewProj));
	return output;
}

//--------------------------------------------------------------------------------------
// Vertex Shader [SKINNED]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS_Skinned(float3 position:POSITION, float4 BoneIndices : BLENDINDICES, float4 BoneWeights : BLENDWEIGHTS) : SV_POSITION
{
	float4 output = (float4)0;

	float4 blendedPosition;
	for (int i = 0; i < 4; i++)
	{
		int boneIndex = BoneIndices[i];
		if (boneIndex >= 0)
		{
			//tranform the position based on the relevant bone and boneWeight
			float4 boneSpacePos = mul(float4(position, 1.f), gBones[boneIndex]);


			//add the position to the blended Position/normal according to the weight
			blendedPosition = blendedPosition + mul(boneSpacePos, BoneWeights[i]);
		}
	}

	//make sure the blended position w-component is 1
	blendedPosition.w = 1.f;

	output = mul(float4(blendedPosition.xyz, 1.f), mul(gWorld, gLightViewProj));

	return output;
}
 
//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void ShadowMapPS_VOID(float4 position:SV_POSITION){}

technique11 GenerateShadows
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
	    SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}

technique11 GenerateShadows_Skinned
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
		SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS_Skinned()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}