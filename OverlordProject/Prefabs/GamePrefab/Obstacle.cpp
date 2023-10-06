#include "stdafx.h"
#include "Obstacle.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"
Obstacle::Obstacle(GameMaterialManager* materialManager, bool edgeObstacle=false)
	:m_MaterialManager{materialManager}
{
	if (edgeObstacle)
	{
		m_ObstacleType = 0;
	}
	else
	{
		m_ObstacleType = rand() % 4;
	}
}

void Obstacle::Initialize(const SceneContext& /*sceneContext*/)
{
	ModelComponent* ObstacleModel{ };
	UINT matId{};
	switch (m_ObstacleType)
	{
	case 0:
		ObstacleModel = AddComponent(new ModelComponent(L"Meshes/Game/Tree2.ovm", true));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Tree2);
		ObstacleModel->SetMaterial(matId);
		break;
	case 1:
		ObstacleModel = AddComponent(new ModelComponent(L"Meshes/Game/Tree1.ovm", true));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Tree1);
		ObstacleModel->SetMaterial(matId);
		break;
	case 2:
		ObstacleModel = AddComponent(new ModelComponent(L"Meshes/Game/Tree0.ovm", true));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Tree0);
		ObstacleModel->SetMaterial(matId);
		break;
	case 3 :
		ObstacleModel = AddComponent(new ModelComponent(L"Meshes/Game/Boulder.ovm", true));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
		ObstacleModel->SetMaterial(matId);
		break;
	default:
		ObstacleModel = AddComponent(new ModelComponent(L"Meshes/Game/Boulder.ovm", true));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
		ObstacleModel->SetMaterial(matId);
		break;
	}
}
