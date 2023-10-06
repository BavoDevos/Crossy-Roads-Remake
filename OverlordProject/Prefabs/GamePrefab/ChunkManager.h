
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include <random>

class ChunkManager final : public GameObject
{

public:
	ChunkManager(GameMaterialManager* materialManager);
	~ChunkManager() override = default;

	ChunkManager(const ChunkManager& other) = delete;
	ChunkManager(ChunkManager&& other) noexcept = delete;
	ChunkManager& operator=(const ChunkManager& other) = delete;
	ChunkManager& operator=(ChunkManager&& other) noexcept = delete;

	//bool IsPassable(int x);
	int getGameWidth() { return m_GameWidth; };
	int CurrentSlice() { return m_CurrentPlayerSlice; };

	void PlayerMove(int playerSlice);
	GameObject* GetSlice(int sliceNumber);
	bool IsTileFree(int sliceNumber, int tile);

	bool DidChickenDie(int slice, int tile);

	void Reset();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:
	
	GameMaterialManager* m_MaterialManager = nullptr;

	int m_GameWidth = 4;
	//chunk handling
	std::unordered_map<int, GameObject*> m_Chunks;

	int m_NewestSlice = 10;
	int m_CurrentPlayerSlice = 0;
	int m_OldestSlice = -8;

	bool m_RiverSliceSpawn = false;
	int m_ConsecutiveDanger = 0;

	void SpawnNextSlice();
	void MakeRandSlice();
	int NextSliceType();
};