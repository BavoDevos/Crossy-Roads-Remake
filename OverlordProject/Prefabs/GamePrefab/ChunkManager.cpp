#include "stdafx.h"
#include "ChunkManager.h"

//chunk types
#include "Prefabs/GamePrefab/GrassChunk.h"
#include "Prefabs/GamePrefab/RiverChunk.h"
#include "Prefabs/GamePrefab/RoadChunk.h"
#include "Prefabs/GamePrefab/RailChunk.h"

ChunkManager::ChunkManager(GameMaterialManager* materialManager)
{
	m_MaterialManager = materialManager;
}

void ChunkManager::PlayerMove(int playerSlice)
{
	if (playerSlice > m_CurrentPlayerSlice)
	{
		m_CurrentPlayerSlice = playerSlice;
		SpawnNextSlice();
	}
}

GameObject* ChunkManager::GetSlice(int sliceNumber)
{
	auto it = m_Chunks.find(sliceNumber);
	if (it != m_Chunks.end())
	{
		return it->second;
	}

	return nullptr;
}

bool ChunkManager::IsTileFree(int sliceNumber, int tile)
{
	auto slice = GetSlice(sliceNumber);
	if (!slice)return false;

	auto grassSlice = dynamic_cast<GrassChunk*>(slice);

	if (!grassSlice)return true;

	return grassSlice->IsTileFree(tile);
}

bool ChunkManager::DidChickenDie(int SliceNumber, int tile)
{
	auto slice = GetSlice(SliceNumber);
	if (!slice)return false;

	auto RiverSlice = dynamic_cast<RiverChunk*>(slice);

	if (!RiverSlice)return false;

	return !(RiverSlice->DoesTileHaveLilypad(tile));
}

void ChunkManager::Reset()
{
	m_Chunks.clear();
	m_CurrentPlayerSlice = 0;
	m_NewestSlice = 8;
	m_CurrentPlayerSlice = 0;
	m_OldestSlice = -8;
	m_RiverSliceSpawn = false;

	for (int i = m_OldestSlice; i <= m_NewestSlice; i++)
	{
		auto Chunk = AddChild(new GrassChunk(m_MaterialManager, i, (i < 2)));
		Chunk->GetTransform()->Translate(0.f, 0.f, float(i));
		m_Chunks.insert(std::pair<int, GameObject*>(i, Chunk));
	}
}

void ChunkManager::Initialize(const SceneContext& /*sceneContext*/)
{

	for (int i = m_OldestSlice; i <= m_NewestSlice; i++)
	{
		GameObject* Chunk = AddChild(new GrassChunk(m_MaterialManager,i,(i<2)));
		Chunk->GetTransform()->Translate(0.f, 0.f, float(i));
		m_Chunks.insert(std::pair<int, GameObject*>(i, Chunk));
	}
}

void ChunkManager::Update(const SceneContext& /*sceneContext*/)
{
}

void ChunkManager::SpawnNextSlice()
{
	m_NewestSlice++;
	MakeRandSlice();

	//delete oldest slice
	auto Iterator = m_Chunks.find(m_OldestSlice);
	if (Iterator != m_Chunks.end())
	{
		RemoveChild(Iterator->second, true);
		m_Chunks.erase(m_OldestSlice);
	}

	m_OldestSlice++;
}

void ChunkManager::MakeRandSlice()
{
	auto next = NextSliceType();

	GameObject* nextChunk{};

	switch (next)
	{
	case 0:
		nextChunk = AddChild( new GrassChunk(m_MaterialManager, m_NewestSlice));
		nextChunk->GetTransform()->Translate(0.f, 0.f, float(m_NewestSlice));
		m_Chunks.insert(std::pair<int, GameObject*>(m_NewestSlice, nextChunk));
		break;
	case 1 :
		nextChunk = AddChild(new RiverChunk(m_MaterialManager, m_NewestSlice));
		nextChunk->GetTransform()->Translate(0.f, 0.1f, float(m_NewestSlice));
		m_Chunks.insert(std::pair<int, GameObject*>(m_NewestSlice, nextChunk));
		break;
	case 2:
		nextChunk = AddChild( new RoadChunk(m_MaterialManager,m_NewestSlice,float(m_GameWidth)));
		nextChunk->GetTransform()->Translate(0.f, 0.f, float(m_NewestSlice));
		m_Chunks.insert(std::pair<int, GameObject*>(m_NewestSlice, nextChunk));
		break;
	case 3:
		nextChunk = AddChild(new RailChunk(m_MaterialManager, m_NewestSlice, float(m_GameWidth)));
		nextChunk->GetTransform()->Translate(0.f, -0.2f, float(m_NewestSlice));
		m_Chunks.insert(std::pair<int, GameObject*>(m_NewestSlice, nextChunk));
		break;
	default:
		break;
	}

}

int ChunkManager::NextSliceType()
{
	float GrassChance = 0.4f;
	float RiverChance = 0;
	float RoadChance = 0.2f;
	float RailChance = 0.1f;

	if (!m_RiverSliceSpawn)
	{
		RiverChance = 0.3f;
	}

	float LevelMultiplier = 1.0f + 0.01f * m_CurrentPlayerSlice;

	if (m_ConsecutiveDanger != 0)
	{
		RoadChance *= (LevelMultiplier / m_ConsecutiveDanger);
		RailChance *= (LevelMultiplier / m_ConsecutiveDanger);
	}
	else
	{
		RoadChance *= (LevelMultiplier);
		RailChance *= (LevelMultiplier);
	}

	float accu = GrassChance + RiverChance+RoadChance+RailChance;

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.f, accu);

	double randomValue = dis(gen);

	if (randomValue < GrassChance)
	{
		m_RiverSliceSpawn = false;
		m_ConsecutiveDanger = 0;
		return 0;
	}
	else if (randomValue < (GrassChance + RiverChance))
	{
		m_RiverSliceSpawn = true;
		m_ConsecutiveDanger = 0;
		return 1;
	}
	else if (randomValue < (GrassChance + RiverChance+RoadChance))
	{
		m_RiverSliceSpawn = false;
		m_ConsecutiveDanger++;
		return 2;
	}
	else
	{
		m_RiverSliceSpawn = false;
		m_ConsecutiveDanger++;
		return 3;
	}
}
