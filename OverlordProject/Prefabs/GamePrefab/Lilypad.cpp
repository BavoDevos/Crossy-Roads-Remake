#include "stdafx.h"
#include "Lilypad.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include <random>
Lilypad::Lilypad(GameMaterialManager* materialManager)
	:m_MaterialManager{ materialManager }
{

	static std::random_device rd;
	static std::mt19937 gen(rd());

	static std::uniform_real_distribution<> dis(-2,2);
	int rotDir = static_cast<int>(dis(gen));
	static std::uniform_real_distribution<> disT(3.0, 5.0);
	float rotT = static_cast<float>(disT(gen));

	static std::uniform_real_distribution<> disR(0.0, 360.0);
	float Startrot = static_cast<float>(disR(gen));
	m_CurrentRot = Startrot;
	GetTransform()->Rotate(0, Startrot, 0);
	GetTransform()->Translate(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y - 0.4f, GetTransform()->GetPosition().z);
	m_RotationDir = rotDir;
	m_RotTime = rotT;
}

void Lilypad::Initialize(const SceneContext& /*sceneContext*/)
{
	auto Lilypad = AddComponent(new ModelComponent(L"Meshes/Game/Lilypad.ovm", true));
	auto matId = m_MaterialManager->GetMaterialId(GameMaterials::LilyPad);
	Lilypad->SetMaterial(matId);
}

void Lilypad::Update(const SceneContext& sceneContext)
{
	m_CurrentRot += sceneContext.pGameTime->GetElapsed() * m_RotSpeed * m_RotationDir;
	m_AccuRotTime += sceneContext.pGameTime->GetElapsed();
	GetTransform()->Rotate(0, m_CurrentRot, 0);
	if (m_AccuRotTime > m_RotTime)
	{
		if (m_RotationDir >= 0)
		{
			m_RotationDir = -1;
		}
		else
		{
			m_RotationDir = 1;
		}
		//m_RotTime = static_cast<float>(rand() % 10 + 3);
		m_AccuRotTime = 0;
	}


}
