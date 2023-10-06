#pragma once
class GameMaterialManager;

class LightTrain final : public GameObject
{

public:
	LightTrain(GameMaterialManager* materialManager,float posx);
	~LightTrain() override;

	LightTrain(const LightTrain& other) = delete;
	LightTrain(LightTrain&& other) noexcept = delete;
	LightTrain& operator=(const LightTrain& other) = delete;
	LightTrain& operator=(LightTrain&& other) noexcept = delete;

	void SetLightOn();
	void SetLightOff();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	GameMaterialManager* m_MaterialManager = nullptr;
	ModelComponent* m_TrainSign = nullptr;
	float m_PosX = -1;

	float m_LightSwitchTime = 0.2f;

	float m_AccuTime = 0;

	bool m_Light1 = false;
	bool m_LightOn = false;

	//sound
	FMOD::Channel* m_pBellChannel{};
	FMOD::Sound* m_pBellSound{};
};