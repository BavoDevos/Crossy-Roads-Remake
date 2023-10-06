#pragma once
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include <queue>
#include "Prefabs/GamePrefab/Train.h"
class LightTrain;
class RailChunk final : public GameObject
{

public:
	RailChunk(GameMaterialManager* materialManager, int sliceIndex, float gameWidth);
	~RailChunk() override;

	RailChunk(const RailChunk& other) = delete;
	RailChunk(RailChunk&& other) noexcept = delete;
	RailChunk& operator=(const RailChunk& other) = delete;
	RailChunk& operator=(RailChunk&& other) noexcept = delete;


protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:
	GameMaterialManager* m_MaterialManager;
	int m_SliceIndex{};

	LightTrain* m_TrainSign = nullptr;
	bool m_TrainSpawned = false;
	bool m_Warning = false;

	int m_Dir = 1;
	float m_CarSpeed = 2;
	float m_SpawnTimer = 2;

	float m_AccuTime = 0;

	float m_GameWidth = 0;

	std::queue<Train*> m_CarQueue{};

	void SpawnTrain();
};