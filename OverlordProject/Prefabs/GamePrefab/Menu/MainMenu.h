
#pragma once
#include "Prefabs/GamePrefab/Menu/MenuButton.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"
class MainMenu final : public GameObject
{

public:
	MainMenu(GameMaterialManager* matman);
	~MainMenu() override {};

	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

	bool getPlayPressed() { return m_PlayPressed; };
	bool setPlayPressed(bool pressed) { m_PlayPressed = pressed; };

	//void Reset();
	Character GetSelectedCharacter() { return m_SelectedCharacter; };

protected:
	void Initialize(const SceneContext& /*sceneContext*/) override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	bool m_PlayPressed = false;

	SpriteFont* m_Font{};

	GameMaterialManager* m_MaterialManager = nullptr;

	MenuButton* m_PlayButton{};
	MenuButton* m_ExitButton{};
	MenuButton* m_ArrowLeft{};
	MenuButton* m_ArrowRight{};

	MenuButton* m_TestB{};
	MenuButton* m_TestB2{};
	MenuButton* m_TestB3{};
	MenuButton* m_TestB4{};

	std::wstring m_GameTitle1{};
	std::wstring m_GameTitle2{};

	std::wstring m_GameTitle3{};
	std::wstring m_GameTitle4{};

	GameObject* m_CurrentModelObject = nullptr;
	ModelComponent* m_CurrentModel = nullptr;

	SpriteComponent* m_TitleBanner{};

	int m_CurrentIndex = 0;

	//void ShowMainMenu(bool show);

	Character m_SelectedCharacter = Character::Chicken;

	bool m_CurrentModelIsAnimated = false;

	const float m_RotSpeed = 20;
	float m_CurrentRot = 0;
	float m_AccuRotTime = 0;

	float m_Test = 0;
	bool tesssssss = false;
	float m_SquishFactor{ 1 };
	float m_MaxSquishScale{ 0.95f };
	int m_Dir{ 1 };

	void CycleCharacters(int step);
	void SwitchCharacter();

	void HandleButtons();

	GameObject* m_Car = nullptr;

	void LoadMenuScene();

};