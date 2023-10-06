#include "stdafx.h"
#include "TestClass.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

TestClass::TestClass(GameMaterialManager* matman)
{
	m_MaterialManager = matman;
}

void TestClass::Initialize(const SceneContext& sceneContext)
{
	m_MaterialManager;

	auto model = AddComponent(new ModelComponent(L"Meshes/Game/Car.ovm",true));
	
	//auto materialId = m_MaterialManager->GetMaterialId(GameMaterials::CarPurple);

	//model->SetMaterial(MaterialManager::Get()->GetMaterial(materialId));

	auto Car1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	Car1->SetDiffuseTexture(L"Textures/Game/CarPurple.png");
	Car1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	model->SetMaterial(Car1);
}
