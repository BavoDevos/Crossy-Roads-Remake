
#include "Prefabs/GamePrefab/GameMaterialManager.h"


#include "Prefabs/GamePrefab/ChunkManager.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include "Prefabs/GamePrefab/PlayerChicken.h"
#include "Prefabs/GamePrefab/GameCamera.h"
#include "Prefabs/GamePrefab/Menu/InGameMenu.h"
#include "Prefabs/GamePrefab/Menu/MainMenu.h"

class MainGameScene : public GameScene
{
public:
	MainGameScene() :GameScene(L"GameScene") {}
	~MainGameScene() override = default;
	MainGameScene(const MainGameScene& other) = delete;
	MainGameScene(MainGameScene&& other) noexcept = delete;
	MainGameScene& operator=(const MainGameScene& other) = delete;
	MainGameScene& operator=(MainGameScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void PostDraw() override;
private:
	GameMaterialManager* m_MaterialManager = nullptr;
	ChunkManager* m_ChunkManager = nullptr;
	PlayerChicken* m_Player = nullptr;
	GameCamera* m_Camera = nullptr;
	CameraComponent* m_MenuCam = nullptr;
	InGameMenu* m_HUD = nullptr;
	MainMenu* m_MainMenu = nullptr;
	const XMFLOAT3 m_LightDir = XMFLOAT3(0.532f, -0.689f, 0.498f);

	Character m_CurrentChar{};

	//sound
	FMOD::Channel* m_pMusicChannel{};
	FMOD::Sound* m_pMusic{};

	bool m_InMainMenu = true;

	void GoToMenu();
	void GoToGame();
	void ResetGame();
};

