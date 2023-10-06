

#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include "Prefabs/GamePrefab/GrassShaderEffect.h"
#include <unordered_set>

class GrassChunk final : public GameObject
{

public:
	GrassChunk(GameMaterialManager* materialManager, int sliceIndex, bool SpawnChunk=false);
	~GrassChunk() override = default;

	GrassChunk(const GrassChunk& other) = delete;
	GrassChunk(GrassChunk&& other) noexcept = delete;
	GrassChunk& operator=(const GrassChunk& other) = delete;
	GrassChunk& operator=(GrassChunk&& other) noexcept = delete;

	bool IsTileFree(int x);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override;

private:
	GameMaterialManager* m_MaterialManager = nullptr;

	std::unordered_set<int> m_ObstacleXpos{};

	int m_SliceIndex{};
	bool m_SpawnChunk = false;

	static bool m_DarkGrass;

	static int m_GrassSliceCounter;
};