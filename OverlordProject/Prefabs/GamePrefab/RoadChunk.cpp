#include "stdafx.h"
#include "RoadChunk.h"
#include <random>

#include "Prefabs/GamePrefab/ChunkManager.h"

RoadChunk::RoadChunk(GameMaterialManager* materialManager, int sliceIndex, float gameWidth)
	:m_SliceIndex{sliceIndex},m_GameWidth{gameWidth}
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	static std::uniform_real_distribution<> dis(0, 2);
	bool cartype = static_cast<int>(dis(gen));

	if (cartype)
	{
		m_MaterialID = materialManager->GetMaterialId(GameMaterials::CarPurple);
	}
	else
	{
		m_MaterialID = materialManager->GetMaterialId(GameMaterials::CarRed);
	}

	cartype = static_cast<int>(dis(gen));

	if (cartype)
	{
		m_Dir = -1;
	}
	else
	{
		m_Dir = 1;
	}

	static std::uniform_real_distribution<> dis2(2, 4);
	m_CarSpeed = static_cast<float>(dis2(gen));

	m_RoadMatID = materialManager->GetMaterialId(GameMaterials::Road);
}

RoadChunk::~RoadChunk()
{
	while (!m_CarQueue.empty())
	{
		m_CarQueue.pop();
	}
}

void RoadChunk::Initialize(const SceneContext& /*sceneContext*/)
{
	ModelComponent* mc = AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
	mc->SetMaterial(MaterialManager::Get()->GetMaterial(m_RoadMatID));

	ChunkManager* chunkManager = dynamic_cast<ChunkManager*>(GetParent());

	RoadChunk* pPrevRoad{};
	if (chunkManager)
	{
		pPrevRoad = dynamic_cast<RoadChunk*>(chunkManager->GetSlice(m_SliceIndex - 1));
	}
	if (pPrevRoad)
	{
		m_Stripes = AddChild(new RoadStripesShader(int(m_GameWidth)));
	}
}

void RoadChunk::Update(const SceneContext& sceneContext)
{
	m_AccuTime += sceneContext.pGameTime->GetElapsed();

	if (m_AccuTime >= m_SpawnTimer)
	{
		m_AccuTime = 0;
		Car* car = new Car(m_MaterialID, m_CarSpeed, m_Dir);
		m_CarQueue.push(car);
		float startpos = m_GameWidth * -2.f * m_Dir;
		car->GetTransform()->Translate(startpos, 0, GetTransform()->GetPosition().z);
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<> dis(2, 6);
		m_SpawnTimer = static_cast<float>(dis(gen));
		AddChild(car);
	}

	//if there is atleast 1 car check if it crossed the game
	if (!m_CarQueue.empty())
	{
		if (m_Dir == 1)
		{
			if (m_CarQueue.front()->GetTransform()->GetPosition().x > m_GameWidth + 3.f)
			{
				RemoveChild(m_CarQueue.front(), true);
				m_CarQueue.pop();
			}
		}
		else
		{
			if (m_CarQueue.front()->GetTransform()->GetPosition().x < -(m_GameWidth) - 3.f)
			{
				RemoveChild(m_CarQueue.front(), true);
				m_CarQueue.pop();
			}
		}
	}
}

void RoadChunk::Draw(const SceneContext& /*sceneContext*/)
{

}
