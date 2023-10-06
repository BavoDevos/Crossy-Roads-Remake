#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial()
	: Material<SpikyMaterial>{ L"Effects/SpikyShader.fx" }
{
}

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });
}
