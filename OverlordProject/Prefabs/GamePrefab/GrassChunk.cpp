#include "stdafx.h"
#include "GrassChunk.h"
#include "Prefabs/GamePrefab/Obstacle.h"

#include "Prefabs/GamePrefab/ChunkManager.h"
#include "Prefabs/GamePrefab/RiverChunk.h"

bool GrassChunk::m_DarkGrass = false;

GrassChunk::GrassChunk(GameMaterialManager* materialManager,int sliceIndex,bool SpawnChunk)
{
	m_MaterialManager = materialManager;
	m_SliceIndex = sliceIndex;
	m_SpawnChunk = SpawnChunk;
	m_DarkGrass = !m_DarkGrass;
}

bool GrassChunk::IsTileFree(int x)
{
	return !(m_ObstacleXpos.find(x) != m_ObstacleXpos.end());
}

void GrassChunk::Initialize(const SceneContext& /*sceneContext*/)
{
	ModelComponent* mc = AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
	if (m_DarkGrass)
	{
		auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass1);
		mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
	}
	else
	{
		auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass2);
		mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
	}


	XMFLOAT3 pos = GetTransform()->GetPosition();
	GetTransform()->Translate(pos.x, 0, pos.z);

	ChunkManager* chunkManager = dynamic_cast<ChunkManager*>(GetParent());

	auto gameWidth = chunkManager->getGameWidth();

	//side obstacles
	GameObject* pObstacle = AddChild(new Obstacle(m_MaterialManager,true));
	pObstacle->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(-gameWidth - 1), 0.f, 0.f });
	pObstacle = AddChild(new Obstacle(m_MaterialManager, true));
	pObstacle->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(-gameWidth - 2), 0.f, 0.f });
	pObstacle = AddChild(new Obstacle(m_MaterialManager, true));
	pObstacle->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(-gameWidth - 3), 0.f, 0.f });
	pObstacle = AddChild(new Obstacle(m_MaterialManager, true));
	pObstacle->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(gameWidth + 1), 0.f, 0.f });
	pObstacle = AddChild(new Obstacle(m_MaterialManager, true));
	pObstacle->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(gameWidth + 2), 0.f, 0.f });

	//obstacles at the back
	if (m_SliceIndex < -2)
	{
		for (int i = -gameWidth; i <= gameWidth; i++)
		{
			GameObject* pObstacle2 = AddChild(new Obstacle(m_MaterialManager, true));
			pObstacle2->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(i), 0.f, 0.f });
			m_ObstacleXpos.insert(i);
		}
	}

	if (!m_SpawnChunk)
	{
		std::vector<int> possibleSpawns;
		
		auto pSlice = chunkManager->GetSlice(m_SliceIndex - 1);

		//auto pGrasSlice = dynamic_cast<GrassChunk*>(pSlice);
		auto pRiverSlice = dynamic_cast<RiverChunk*>(pSlice);

		//dont spawn any obstacles after a river just to prevent path being blocked
		if (!pRiverSlice)
		{
			for (int i = -gameWidth; i <= gameWidth; i++)
			{
				possibleSpawns.push_back(i);
			}
		}

		int ObstacleAmount = rand() % 3;

		for (int i = 0; i <= ObstacleAmount; i++)
		{
			if (possibleSpawns.size() > 3)
			{
				auto randIndex = rand() % possibleSpawns.size();
				int posX = possibleSpawns[randIndex];
				GameObject* pObstacle3 = AddChild(new Obstacle(m_MaterialManager, false));
				pObstacle3->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(posX), 0.f, 0.f });
				m_ObstacleXpos.insert(posX);
				possibleSpawns.erase(possibleSpawns.begin() + randIndex);
			}
		}
	}

	AddChild(new GrassShaderEffect(30, float(gameWidth)));
}

void GrassChunk::Update(const SceneContext&)
{
}

void GrassChunk::Draw(const SceneContext& /*sceneContext*/)
{

}
