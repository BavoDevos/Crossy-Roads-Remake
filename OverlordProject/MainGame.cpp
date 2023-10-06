#include "stdafx.h"
#include "MainGame.h"


#define Game

#pragma region Lab/Milestone Includes
#ifdef Game
#include "Scenes/Game/MainGameScene.h"
#endif


#pragma endregion

//Game is preparing
void MainGame::OnGamePreparing(GameContext& gameContext)
{
	gameContext.windowWidth = 1000;
	gameContext.windowHeight = 800;

	gameContext.windowTitle = L"GP2 - Crossy Roads (2023) | (2DAE07) Devos Bavo";
}

void MainGame::Initialize()
{
#ifdef Game
	SceneManager::Get()->AddGameScene(new MainGameScene());
#endif

}

LRESULT MainGame::WindowProcedureHook(HWND /*hWnd*/, UINT message, WPARAM wParam, LPARAM lParam)
{

	if(message == WM_KEYUP)
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		//[F1] Toggle Scene Info Overlay
		if(wParam == VK_F1)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetSceneSettings().Toggle_ShowInfoOverlay();
		}

		//[F2] Toggle Debug Renderer (Global)
		if (wParam == VK_F2)
		{
			DebugRenderer::ToggleDebugRenderer();
			return 0;

		}

		//[F3] Previous Scene
		if (wParam == VK_F3)
		{
			SceneManager::Get()->PreviousScene();
			return 0;

		}

		//[F4] Next Scene
		if (wParam == VK_F4)
		{
			SceneManager::Get()->NextScene();
			return 0;
		}

		//[F5] If PhysX Framestepping is enables > Next Frame	
		if (wParam == VK_F6)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetPhysxProxy()->NextPhysXFrame();
		}
	}
	

	return -1;
}
