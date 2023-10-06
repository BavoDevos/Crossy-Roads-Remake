#pragma once

class ChunkManager;
#include "Prefabs/GamePrefab/GameMaterialManager.h"

class PlayerChicken : public GameObject
{
public:
	PlayerChicken(int gameWidth,ChunkManager* chunkManager,GameMaterialManager* materialManager,Character currentChar)
		: m_GameWidth{ gameWidth },m_ChunkManager{chunkManager},m_MaterialManager{materialManager},m_CurrentChar{currentChar}
	{};
	~PlayerChicken() override = default;

	PlayerChicken(const PlayerChicken& other) = delete;
	PlayerChicken(PlayerChicken&& other) noexcept = delete;
	PlayerChicken& operator=(const PlayerChicken& other) = delete;
	PlayerChicken& operator=(PlayerChicken&& other) noexcept = delete;

	bool IsDead() { return m_Dead; };
	void setChunkManager(ChunkManager* chunk) { m_ChunkManager = chunk; };
	void Respawn();

protected:
	void Initialize(const SceneContext& /*sceneContext*/) override;
	void Update(const SceneContext& sceneContext) override;

private:


	int m_GameWidth{};
	int m_CurrentChunkI = 0;
	int m_CurrentLastChunk = -5;
	int m_AllowedBackTrack = 5;

	//chunkManager
	ChunkManager* m_ChunkManager = nullptr;
	GameMaterialManager* m_MaterialManager = nullptr;

	//movement vars
	int m_TargetPosX{};
	int m_TargetPosZ{};
	int m_PrevX{};
	int m_PrevZ{};
	const float m_JumpHeight{ 0.3f };
	float m_CurrentHeight{};

	//rotation vars
	float m_PrevRotY{};
	float m_TargetRotY{};
	float m_RotTimer{};
	const float m_RotTime{ 0.08f };

	const float m_JumpTime{ 0.1f };
	float m_JumpTimer{ 0.f };

	GameObject* m_ModelChild{};

	float m_SquishFactor{ 0 };
	float m_MaxSquishScale{ 0.7f };
	bool m_KeyPressed{ false };

	bool m_Dead = false;

	void SetTargetRot(float rot);

	Character m_CurrentChar{};

	//particle
	GameObject* m_DeathParticleObject=nullptr;
	ParticleEmitterSettings m_DeathParticleSettings{};
	ParticleEmitterComponent* m_DeathParticle = nullptr;

	//sound
	FMOD::Channel* m_pChannelJump{};
	FMOD::Sound* m_pJumpSound{};
	FMOD::Channel* m_pChannelDie{};
	FMOD::Sound* m_pDieSound{};
	FMOD::Sound* m_pSplashSound{};

	void InitModel();
};

