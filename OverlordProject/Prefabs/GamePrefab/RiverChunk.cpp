#include "stdafx.h"
#include "RiverChunk.h"
#include "Prefabs/GamePrefab/Lilypad.h"
#include "Prefabs/GamePrefab/GrassChunk.h"
#include "Prefabs/GamePrefab/ChunkManager.h"

RiverChunk::RiverChunk(GameMaterialManager* materialManager, int sliceIndex)
{
    m_MaterialManager = materialManager;
    m_SliceIndex = sliceIndex;
}

RiverChunk::~RiverChunk()
{
}

bool RiverChunk::DoesTileHaveLilypad(int x)
{
    return (m_LilypadXpos.find(x) != m_LilypadXpos.end());
}

void RiverChunk::Initialize(const SceneContext& /*sceneContext*/)
{
	ModelComponent* mc = AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
	auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::River);
	mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));

	XMFLOAT3 pos = GetTransform()->GetPosition();
	GetTransform()->Translate(pos.x, 0, pos.z);

	//spawn lilypads

	ChunkManager* chunkManager = dynamic_cast<ChunkManager*>(GetParent());

	auto gameWidth = chunkManager->getGameWidth();

	std::vector<int> possibleSpawns;
	std::vector<int> ForcedSpawn;

	auto pSlice = chunkManager->GetSlice(m_SliceIndex-1);

	auto pGrasSlice = dynamic_cast<GrassChunk*>(pSlice);
	auto pRiverSlice = dynamic_cast<RiverChunk*>(pSlice);

	bool ForceSpawm = false;
	int forceAnount = 0;
	if (pGrasSlice)
	{
		for (int i = -gameWidth; i <= gameWidth; i++)
		{
			if (pGrasSlice->IsTileFree(i))
			{
				possibleSpawns.push_back(i);
			}
		}
	}
	else if (pRiverSlice)
	{
		for (int i = -gameWidth; i <= gameWidth; i++)
		{
			if (pRiverSlice->DoesTileHaveLilypad(i))
			{
				ForcedSpawn.push_back(i);
				ForceSpawm = true;
				forceAnount++;
			}
			else
			{
				possibleSpawns.push_back(i);
			}
		}
	}
	else
	{
		for (int i = -gameWidth; i <= gameWidth; i++)
		{
			possibleSpawns.push_back(i);
		}
	}


	static std::random_device rd;
	static std::mt19937 gen(rd());

	if (ForceSpawm)
	{
		for (int posX : ForcedSpawn)
		{
			GameObject* pObstacle3 = AddChild(new Lilypad(m_MaterialManager));
			pObstacle3->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(posX), -0.05f, 0.f });
			m_LilypadXpos.insert(posX);

		}
	}
	static std::uniform_real_distribution<> dis(2,5);
	int ObstacleAmount = static_cast<int>(dis(gen));

	for (int i = 0; i <= ObstacleAmount; i++)
	{
		if (possibleSpawns.size() > 3)
		{
			std::uniform_real_distribution<> dis2(0, double(possibleSpawns.size()));
			int randIndex = static_cast<int>(dis2(gen));
			int posX = possibleSpawns[randIndex];
			GameObject* pObstacle3 = AddChild(new Lilypad(m_MaterialManager));
			pObstacle3->GetTransform()->Translate(XMFLOAT3{ static_cast<float>(posX), -0.05f, 0.f });
			m_LilypadXpos.insert(posX);
			possibleSpawns.erase(possibleSpawns.begin() + randIndex);
		}
	}

	//particles

		//	//particles
	m_pFoamLeftParticleObkect = AddChild(new GameObject());

	m_FoamSettings.minEmitterRadius = -0.3f;
	m_FoamSettings.maxEmitterRadius = 0.3f;
	m_FoamSettings.minScale = 1.f;
	m_FoamSettings.maxScale = 1.f;

	m_FoamSettings.minSize = 0.4f;
	m_FoamSettings.maxSize = 0.7f;
	m_FoamSettings.minEnergy = .7f;
	m_FoamSettings.maxEnergy = 1.5f;
	//m_FoamSettings.color = MathHelper::HSLtoRGB(0.9f,0.95f,0.98f, 1.f);
	m_FoamSettings.color = MathHelper::HSLtoRGB(1, 1, 1, 1.f);
	m_FoamSettings.faceCamera = false;
	m_FoamSettings.rotateFreely = false;

	XMFLOAT4X4 facingAxisMatrix{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	};


	m_FoamSettings.facingAxisMatrix = facingAxisMatrix;


	m_FoamSettings.velocity = { 0.5f, 0.f, 0.f };
	m_pFoamLeft = m_pFoamLeftParticleObkect->AddComponent(new ParticleEmitterComponent(L"Textures/Game/Foam.png", m_FoamSettings, 20, false));


	m_pFoamRightParticleObkect = AddChild(new GameObject());
	m_FoamSettings.velocity = { -0.5f, 0.f, 0.f };
	m_PFoamRight = m_pFoamRightParticleObkect->AddComponent(new ParticleEmitterComponent(L"Textures/Game/Foam.png", m_FoamSettings, 20, false));

	//m_pFoamLeftParticleObkect->GetTransform()->Translate(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y, GetTransform()->GetPosition().z);

	m_pFoamRightParticleObkect->GetTransform()->Translate(GetTransform()->GetPosition().x + 4.7f, GetTransform()->GetPosition().y + 1.f, GetTransform()->GetPosition().z);

	m_pFoamLeftParticleObkect;
}

void RiverChunk::Update(const SceneContext&)
{
	if (!particlesSet)
	{
		m_pFoamLeftParticleObkect->GetTransform()->Translate(GetTransform()->GetPosition().x-5.f, GetTransform()->GetPosition().y+0.f, GetTransform()->GetPosition().z);
		m_pFoamRightParticleObkect->GetTransform()->Translate(GetTransform()->GetPosition().x + 4.7f, GetTransform()->GetPosition().y + 0.f, GetTransform()->GetPosition().z);
	}
}
