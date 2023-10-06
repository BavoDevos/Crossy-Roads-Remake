#include "stdafx.h"
#include "RailChunk.h"
#include "Prefabs/GamePrefab/LightTrain.h"
#include "Prefabs/GamePrefab/ChunkManager.h"
#include <random>

RailChunk::RailChunk(GameMaterialManager* materialManager, int sliceIndex, float gameWidth)
	:m_MaterialManager{materialManager},m_SliceIndex(sliceIndex),m_GameWidth{gameWidth}
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	static std::uniform_real_distribution<> dis(0, 2);
	bool cartype = static_cast<int>(dis(gen));

	if (cartype)
	{
		m_Dir = -1;
	}
	else
	{
		m_Dir = 1;
	}

	static std::uniform_real_distribution<> dis2(20, 40);
	m_CarSpeed = static_cast<float>(dis2(gen));

	static std::uniform_real_distribution<> dis3(2, 6);
	m_SpawnTimer = static_cast<float>(dis3(gen));
}

RailChunk::~RailChunk()
{
	while (!m_CarQueue.empty())
	{
		m_CarQueue.pop();
	}
}

void RailChunk::Initialize(const SceneContext& /*sceneContext*/)
{
	XMFLOAT3 pos = GetTransform()->GetPosition();
	GetTransform()->Translate(-100,100,100);
	ModelComponent* mc = AddComponent(new ModelComponent(L"Meshes/Game/Rail.ovm", false));
	auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Rail);
	mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));


	bool trainLightSet = false;
	ChunkManager* chunkManager = dynamic_cast<ChunkManager*>(GetParent());
	if (chunkManager->IsTileFree(m_SliceIndex - 1, -1))
	{
		m_TrainSign = AddChild(new LightTrain(m_MaterialManager,-1));
		trainLightSet = true;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (chunkManager->IsTileFree(m_SliceIndex - 1, -i))
			{
				m_TrainSign = AddChild(new LightTrain(m_MaterialManager, static_cast<float>(-i)));
				trainLightSet = true;
				break;
			}
			else if (chunkManager->IsTileFree(m_SliceIndex - 1, i))
			{
				m_TrainSign = AddChild(new LightTrain(m_MaterialManager, static_cast<float>(i)));
				trainLightSet = true;
				break;
			}
		}
	}

	//if somehow no tile was free just set pos to 0
	if (!trainLightSet)
	{
		m_TrainSign = AddChild(new LightTrain(m_MaterialManager, 0));
	}

	//m_TrainSign->SetLightOn();
}

void RailChunk::Update(const SceneContext& sceneContext)
{
	if (!m_TrainSpawned)
	{
		m_AccuTime += sceneContext.pGameTime->GetElapsed();

		if (m_AccuTime + 1.f >= m_SpawnTimer)
		{
			m_TrainSign->SetLightOn();
		}

		if (m_AccuTime >= m_SpawnTimer)
		{
			SpawnTrain();
			m_TrainSpawned = true;
			m_AccuTime = 0;
		}


	}
	//if there is atleast 1 car check if it crossed the game
	if (!m_CarQueue.empty())
	{
		if (m_Dir == 1)
		{
			if (m_CarQueue.front()->GetTransform()->GetPosition().x > m_GameWidth + 50.2f)
			{
				RemoveChild(m_CarQueue.front(), true);
				m_CarQueue.pop();

				m_TrainSign->SetLightOff();
			}
		}
		else
		{
			if (m_CarQueue.front()->GetTransform()->GetPosition().x < -(m_GameWidth)-50.2f)
			{
				RemoveChild(m_CarQueue.front(), true);
				m_CarQueue.pop();

				m_TrainSign->SetLightOff();
			}
		}
	}
	else
	{
		m_TrainSpawned = false;
	}
}

void RailChunk::SpawnTrain()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis2(3,12);
	int randomWagonAmount = static_cast<int>(dis2(gen));

	float WagonSize = 5.2f;
	for (int i = 0; i <= randomWagonAmount; i++)
	{
		if (m_Dir == -1)
		{
			if (i == 0)
			{
				Train* car = new Train(m_MaterialManager->GetMaterialId(GameMaterials::TrainFront), m_CarSpeed, m_Dir, true);
				m_CarQueue.push(car);
				float startpos = -((-m_GameWidth * -2.f * m_Dir) + (0 * 1));
				car->GetTransform()->Translate(startpos, 0, GetTransform()->GetPosition().z);
				AddChild(car);
			}
			else
			{
				Train* car2 = new Train(m_MaterialManager->GetMaterialId(GameMaterials::TrainMiddle), m_CarSpeed, m_Dir);
				m_CarQueue.push(car2);
				float startpos2 = -((-m_GameWidth * -2.f * m_Dir) - (WagonSize * i));
				car2->GetTransform()->Translate(startpos2, 0, GetTransform()->GetPosition().z);
				AddChild(car2);
			}
		}
		else
		{
			if (i == 0)
			{
				Train* car = new Train(m_MaterialManager->GetMaterialId(GameMaterials::TrainFront), m_CarSpeed, m_Dir, true);
				m_CarQueue.push(car);
				float startpos = ((m_GameWidth * -2.f * m_Dir) + (0 * 1));
				car->GetTransform()->Translate(startpos, 0, GetTransform()->GetPosition().z);
				AddChild(car);
			}
			else
			{
				Train* car2 = new Train(m_MaterialManager->GetMaterialId(GameMaterials::TrainMiddle), m_CarSpeed, m_Dir);
				m_CarQueue.push(car2);
				float startpos2 = ((m_GameWidth * -2.f * m_Dir) - (WagonSize * i));
				car2->GetTransform()->Translate(startpos2, 0, GetTransform()->GetPosition().z);
				AddChild(car2);
			}
		}
	}
	static std::uniform_real_distribution<> dis(4, 10);
	m_SpawnTimer = static_cast<float>(dis(gen));


}
