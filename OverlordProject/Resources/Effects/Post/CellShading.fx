//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Mirror;
	AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState depthWrite
{
	DEPTHWRITEMASK = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState backCull
{
	CullMode = back;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = float4(input.Position, 1.f);
	output.TexCoord = input.TexCoord;

	return output;
}



//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Step 1: Sample the texture
	float3 sampledColor = gTexture.Sample(samPoint, input.TexCoord).rgb;
	// Check if the color is pure white or pure black
	bool isPureWhite = all(sampledColor.rgb == float3(1.0, 1.0, 1.0));
	bool isPureBlack = all(sampledColor.rgb == float3(0.0, 0.0, 0.0));

	if (isPureWhite || isPureBlack)
	{
		// Return the sampled color as is
		return float4(sampledColor, 1.0);
	}
	// Step 2: Calculate the grayscale intensity
	float intensity = (sampledColor.r + sampledColor.g + sampledColor.b) / 3;
	//float intensity = dot(sampledColor.rgb, float3(0.299, 0.587, 0.114));
	// Step 3: Define the shade colors based on intensity levels
	float4 shadeColors[5];
	shadeColors[0] = float4(sampledColor * 0.9, 1.0);    // Darkest shade
	shadeColors[1] = float4(sampledColor , 1.0);
	shadeColors[2] = float4(sampledColor*1.1, 1.0);          // Middle shade
	shadeColors[3] = float4(sampledColor * 1.2, 1.0);    // Slightly brighter lightest shade
	shadeColors[4] = float4(sampledColor * 1.3, 1.0);
	// Step 4: Determine the shading level
	int numLevels = 5;
	int shadingLevel = int(intensity * numLevels);

	// Step 5: Apply the shade color based on shading level
	float4 celShadeColor = shadeColors[shadingLevel];

	// Step 6: Return the cel-shaded color
	return celShadeColor;
}


////PIXEL SHADER
////------------
//float4 PS(PS_INPUT input) : SV_Target
//{
//	// Step 1: sample the texture
//	float3 sampledColor = gTexture.Sample(samPoint, input.TexCoord);
//	// Step 2: calculate the mean value
//	float colorValue = (sampledColor.r + sampledColor.g + sampledColor.b) / 3;
//	// Step 3: return the color
//
//	return float4(colorValue, colorValue, colorValue, 1.0f);
//}





//TECHNIQUE
//---------
technique11 Outline
{
	pass P0
	{
		// Set states...
		SetRasterizerState(backCull);
		SetDepthStencilState(depthWrite, 0);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}