
#include <queue>
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include "Prefabs/GamePrefab/Car.h"
#include "Prefabs/GamePrefab/RoadStripesShader.h"

class RoadChunk final : public GameObject
{

public:
	RoadChunk(GameMaterialManager* materialManager, int sliceIndex,float gameWidth);
	~RoadChunk() override;

	RoadChunk(const RoadChunk& other) = delete;
	RoadChunk(RoadChunk&& other) noexcept = delete;
	RoadChunk& operator=(const RoadChunk& other) = delete;
	RoadChunk& operator=(RoadChunk&& other) noexcept = delete;


protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& sceneContext) override;

private:
	UINT m_MaterialID = 0;
	UINT m_RoadMatID = 0;
	int m_SliceIndex{};

	int m_Dir = 0;
	float m_CarSpeed = 0;
	float m_SpawnTimer = 0;

	float m_AccuTime = 0;

	float m_GameWidth = 0;

	std::queue<Car*> m_CarQueue{};
	RoadStripesShader* m_Stripes = nullptr;
};