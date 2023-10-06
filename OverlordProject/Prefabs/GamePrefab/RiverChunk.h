
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include <unordered_set>

class RiverChunk final : public GameObject
{

public:
	RiverChunk(GameMaterialManager* materialManager, int sliceIndex);
	~RiverChunk() override;

	RiverChunk(const RiverChunk& other) = delete;
	RiverChunk(RiverChunk&& other) noexcept = delete;
	RiverChunk& operator=(const RiverChunk& other) = delete;
	RiverChunk& operator=(RiverChunk&& other) noexcept = delete;

	bool DoesTileHaveLilypad(int x);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:
	GameMaterialManager* m_MaterialManager = nullptr;

	std::unordered_set<int> m_LilypadXpos{};

	int m_SliceIndex{};

	//particles
	GameObject* m_pFoamLeftParticleObkect;
	ParticleEmitterSettings m_FoamSettings;
	ParticleEmitterComponent* m_pFoamLeft;

	GameObject* m_pFoamRightParticleObkect;
	ParticleEmitterComponent* m_PFoamRight;

	bool particlesSet=false;
};