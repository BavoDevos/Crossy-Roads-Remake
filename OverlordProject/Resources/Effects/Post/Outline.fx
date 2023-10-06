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

float3 rgbToHSV(float3 rgb)
{
	float3 hsv;

	float cMax = max(max(rgb.r, rgb.g), rgb.b);
	float cMin = min(min(rgb.r, rgb.g), rgb.b);
	float delta = cMax - cMin;

	// Calculate hue
	if (delta == 0)
	{
		hsv.x = 0;
	}
	else if (cMax == rgb.r)
	{
		hsv.x = 60 * ((rgb.g - rgb.b) / delta + 6);
	}
	else if (cMax == rgb.g)
	{
		hsv.x = 60 * ((rgb.b - rgb.r) / delta + 2);
	}
	else
	{
		hsv.x = 60 * ((rgb.r - rgb.g) / delta + 4);
	}

	// Calculate saturation
	if (cMax == 0)
	{
		hsv.y = 0;
	}
	else
	{
		hsv.y = delta / cMax;
	}

	// Calculate value
	hsv.z = cMax;

	return hsv;
}

////PIXEL SHADER
////------------
//float4 PS(PS_INPUT input) : SV_Target
//{
//    float4 outlineColor = float4(0.0, 0.0, 0.0, 1.0);
//    float4 imageColor = gTexture.Sample(samPoint, input.TexCoord);
//
//    // Detect edges
//    uint width, height, levels;
//    gTexture.GetDimensions(0, width, height, levels);
//    float2 texelSize = 1.0 / float2(width, height);
//    float edgeFactor = 0.0;
//    for (int x = -1; x <= 1; ++x)
//    {
//        for (int y = -1; y <= 1; ++y)
//        {
//            edgeFactor += length(imageColor - gTexture.Sample(samPoint, input.TexCoord + float2(x, y) * texelSize));
//        }
//    }
//
//    edgeFactor /= 9.0; // Normalize
//
//    // Apply black outline to edges
//    float outlineThreshold = 0.1; // Adjust this threshold as needed
//    if (edgeFactor > outlineThreshold)
//    {
//        return outlineColor;
//    }
//    else
//    {
//        return imageColor;
//    }
//}

//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 outlineColor = float4(0.0, 0.0, 0.0, 1.0);
	float4 imageColor = gTexture.Sample(samPoint, input.TexCoord);

	// Detect edges
	uint width, height, levels;
	gTexture.GetDimensions(0, width, height, levels);
	float2 texelSize = 1.0 / float2(width, height);
	float edgeFactor = 0.0;

	// Increase the range of neighboring pixels
	float outlineRange = 1.2; // Increase this for thicker outlines

	for (int x = -outlineRange; x <= outlineRange; ++x)
	{
		for (int y = -outlineRange; y <= outlineRange; ++y)
		{
			float4 neighborColor = gTexture.Sample(samPoint, input.TexCoord + float2(x, y) * texelSize);
			edgeFactor += length(imageColor.rgb - neighborColor.rgb);
		}
	}

	edgeFactor /= float((outlineRange * 2 + 1) * (outlineRange * 2 + 1)); // Normalize

	// Adjust the outline threshold and blend amount for the desired effect
	float outlineThreshold = 0.02; // Adjust this threshold as needed
	float outlineBlendAmount = saturate(5.f * (edgeFactor - outlineThreshold) / (1.0 - outlineThreshold));

	// Blend the outline color with the original image color
	float4 blendedColor = lerp(imageColor, outlineColor, outlineBlendAmount);

	return blendedColor;
}





////PIXEL SHADER
////------------
//float4 PS(PS_INPUT input) : SV_Target
//{
//	float4 outlineColor = float4(0.0, 0.0, 0.0, 1.0);
//	float4 imageColor = gTexture.Sample(samPoint, input.TexCoord);
//
//	// Calculate the pixel's saturation
//	float3 pixelHSV = rgbToHSV(imageColor.rgb);
//	float saturation = pixelHSV.y;
//
//	// Define the shadow saturation threshold
//	float shadowSaturationThreshold = 0.4;
//
//	// Check if the pixel is a shadow based on saturation
//	bool isShadow = saturation < shadowSaturationThreshold;
//
//	if (isShadow)
//	{
//		// Return the original pixel color without applying the outline effect
//		return imageColor;
//	}
//	else
//	{
//		    // Detect edges
//		    uint width, height, levels;
//		    gTexture.GetDimensions(0, width, height, levels);
//		    float2 texelSize = 1.0 / float2(width, height);
//		    float edgeFactor = 0.0;
//		
//		    // Increase the range of neighboring pixels
//		    float outlineRange = 1.2; // Increase this for thicker outlines
//		
//		    for (int x = -outlineRange; x <= outlineRange; ++x)
//		    {
//		        for (int y = -outlineRange; y <= outlineRange; ++y)
//		        {
//		            float4 neighborColor = gTexture.Sample(samPoint, input.TexCoord + float2(x, y) * texelSize);
//		            edgeFactor += length(imageColor.rgb - neighborColor.rgb);
//		        }
//		    }
//		
//		    edgeFactor /= float((outlineRange * 2 + 1) * (outlineRange * 2 + 1)); // Normalize
//		
//		    // Adjust the outline threshold for thicker outlines
//		    float outlineThreshold = 0.015; // Adjust this threshold as needed
//		    if (edgeFactor > outlineThreshold)
//		    {
//		        return outlineColor;
//		    }
//		    else
//		    {
//		        return imageColor;
//		    }
//	}
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