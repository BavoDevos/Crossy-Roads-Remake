#include "stdafx.h"
#include "BricksUberMaterial.h"

BricksUberMaterial::BricksUberMaterial()
	: Material<BricksUberMaterial>{ L"Effects/UberShader.fx" }
{
}

void BricksUberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", XMFLOAT3{ 0.577f, 0.577f, -0.577f });

	SetVariable_Scalar(L"gUseTextureDiffuse", true);
	SetVariable_Scalar(L"gUseEnvironmentMapping", true);
	SetVariable_Scalar(L"gUseFresnelFalloff", true);
	SetVariable_Scalar(L"gUseTextureNormal", true);
	SetVariable_Scalar(L"gUseSpecularPhong", true);

	SetVariable_Vector(L"gColorAmbient", XMFLOAT4{ 0.5f, 0.6f, 0.9f, 1.0f });

	SetVariable_Scalar(L"gReflectionStrength", 0.7f);
	SetVariable_Scalar(L"gRefractionStrength", 0.2f);
	SetVariable_Scalar(L"gRefractionIndex", 1.0f);
	SetVariable_Scalar(L"gShininess", 22);

	SetVariable_Texture(L"gTextureDiffuse", ContentManager::Load<TextureData>(L"Textures/Wall_DiffuseMap.jpg"));
	SetVariable_Texture(L"gTextureSpecularIntensity", ContentManager::Load<TextureData>(L"Textures/Wall_HeightMap.jpg"));
	SetVariable_Texture(L"gTextureOpacity", ContentManager::Load<TextureData>(L"Textures/Wall_HeightMap.jpg"));
	SetVariable_Texture(L"gTextureNormal", ContentManager::Load<TextureData>(L"Textures/Wall_NormalMap.jpg"));
	SetVariable_Texture(L"gCubeEnvironment", ContentManager::Load<TextureData>(L"Textures/Sunol_Cubemap.dds"));
}
