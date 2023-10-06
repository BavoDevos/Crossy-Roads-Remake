#pragma once
#include "Prefabs/GamePrefab/Menu/MenuButton.h"
#include "Prefabs/GamePrefab/Menu/ControlsKeyboard.h"

class InGameMenu final : public GameObject
{

public:
	InGameMenu();
	~InGameMenu() override {};

	InGameMenu(const InGameMenu& other) = delete;
	InGameMenu(InGameMenu&& other) noexcept = delete;
	InGameMenu& operator=(const InGameMenu& other) = delete;
	InGameMenu& operator=(InGameMenu&& other) noexcept = delete;

	void SetScore(int score);
	bool getGameOver() { return m_GameOver; };
	void SetGameOver(bool gameOver) { m_GameOver = gameOver; }
	bool getPlayPressed() { return m_PlayPressed; };
	bool setPlayPressed(bool pressed) { m_PlayPressed = pressed; };
	bool GetToMenuPressed() { return m_ToMenuPressed; };
	void Reset();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	int m_Score{ 0 };
	bool m_GameOver{ false };
	bool m_ScoreFlag{ false };
	bool m_GameOverFlag{ false };

	bool m_PlayPressed = false;
	bool m_ToMenuPressed = false;

	SpriteFont* m_Font{};
	SpriteFont* m_BigFont{};

	std::wstring m_ScoreText{};

	const std::wstring m_GameOverText{};
	std::wstring m_ScoreComment{};

	//std::wstring m_ScoreText{};

	MenuButton* m_PlayButton{};
	//MenuButton* m_PauseButton{};
	MenuButton* m_HomeButton{};
	MenuButton* m_ExitButton{};

	ControlsKeyboard* m_Controls = nullptr;

	MenuButton* m_TitleCard{};

	bool m_ShowControls = true;

	float m_Offset = 0;

	void ShowDeathMenu(bool show);
	void ShowSpawnScreen(bool show);

	void UpdateDeathMenu(const SceneContext& sceneContext);
	void UpdateSpawnMenu(const SceneContext& sceneContext);
	void UpdateInGame(const SceneContext& sceneContext);
};