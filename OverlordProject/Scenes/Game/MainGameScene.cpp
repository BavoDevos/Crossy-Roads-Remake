#include "stdafx.h"
#include "MainGameScene.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Prefabs/GamePrefab/PlayerChicken.h"

#include "Materials/Post/PostCellShading.h"
#include "Materials/Post/PostOutline.h"

void MainGameScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;

	m_SceneContext.pLights->SetDirectionalLight({ -5.f, 10.f, 5.f },m_LightDir);
	ShadowMapRenderer::Get()->SetProjectionSize(30.f);

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	m_MaterialManager = AddChild(new GameMaterialManager());

	m_MainMenu = AddChild(new MainMenu(m_MaterialManager));

	//CAMERA
	GameObject* pCam = AddChild(new GameObject());
	m_MenuCam = pCam->AddComponent(new CameraComponent());
	m_MenuCam->SetOrthoSize(8.f);


	if (false)
	{
		//for testing
		auto freec = new FreeCamera();
		AddChild(freec);
		m_MenuCam = freec->GetComponent<CameraComponent>();
		m_MainMenu->GetTransform()->Scale(1.1f);

	}
	else
	{


		float distance = 10.f;
		//float distanceY = 12.f;
		float pitch{ 10.f };
		float yaw{ 0.f };
		pCam->GetTransform()->Rotate(pitch, yaw, 0.f);

		//lookat translation
		XMFLOAT3 forward{ sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch)) ,
						sin(XMConvertToRadians(-pitch)),
						cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch))
		};

		//XMFLOAT3 transform = XMFLOAT3(-forward.x * distance, -forward.y * distanceY, -forward.z * distance);
		XMFLOAT3 transform = XMFLOAT3(-forward.x * distance, 2.5f, -forward.z * distance);
		pCam->GetTransform()->Translate(transform);

		m_MainMenu->GetTransform()->Scale(1.1f);
		//activate
		m_MenuCam->SetActive(true);
	}





	//inputs
	auto inputAction = InputAction(PressForward, InputState::pressed, VK_UP);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(PressBackward, InputState::pressed, VK_DOWN);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(PressLeft, InputState::pressed, VK_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(PressRight, InputState::pressed, VK_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(ReleaseForward, InputState::released, VK_UP);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(ReleaseBackward, InputState::released, VK_DOWN);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(ReleaseLeft, InputState::released, VK_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(ReleaseRight, InputState::released, VK_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);
	inputAction = InputAction(Respawn, InputState::pressed, VK_SPACE);
	m_SceneContext.pInput->AddInputAction(inputAction);
	


	//post 

	AddPostProcessingEffect(MaterialManager::Get()->CreateMaterial<PostCellShading>());
	AddPostProcessingEffect(MaterialManager::Get()->CreateMaterial<PostOutline>());




	FMOD::System* pFmod = SoundManager::Get()->GetSystem();
	pFmod->createStream("Resources/SFX/music.wav", FMOD_DEFAULT, nullptr, &m_pMusic);

	SoundManager::Get()->GetSystem()->playSound(m_pMusic, nullptr, false, &m_pMusicChannel);
	m_pMusicChannel->setVolume(0.3f);
	m_pMusicChannel->setLoopCount(-1);
}

void MainGameScene::OnGUI()
{
}

void MainGameScene::Update()
{
	if (!m_InMainMenu)
	{
		auto currentPos = m_ChunkManager->CurrentSlice();
		m_SceneContext.pLights->SetDirectionalLight({ -5.f, 5.f, currentPos - 3.5f }, m_LightDir);
		m_HUD->SetScore(currentPos);

		if (m_Player->IsDead() && !m_HUD->getGameOver())
		{
			m_HUD->SetGameOver(true);
		}

		if (m_Player->IsDead() && m_HUD->getPlayPressed())
		{
			ResetGame();
		}
		else if (m_Player->IsDead() && m_HUD->GetToMenuPressed())
		{
			GoToMenu();
		}
	}
	else
	{
		if (m_MainMenu->getPlayPressed())
		{
			GoToGame();
		}
	}
}

void MainGameScene::PostDraw()
{

}

void MainGameScene::GoToMenu()
{
	RemoveChild(m_Player, true);
	RemoveChild(m_ChunkManager, true);
	RemoveChild(m_HUD, true);
	m_Camera->GetComponent<CameraComponent>()->SetActive(false);
	RemoveChild(m_Camera, true);

	//activate
	m_MenuCam->SetActive(true);

	m_MainMenu = AddChild(new MainMenu(m_MaterialManager));
	m_InMainMenu = true;

	m_MainMenu->GetTransform()->Scale(1.1f);
	m_SceneContext.pLights->SetDirectionalLight({ -5.f, 10.f, 5.f }, m_LightDir);
	ShadowMapRenderer::Get()->SetProjectionSize(30.f);

}

void MainGameScene::GoToGame()
{
	m_CurrentChar =  m_MainMenu->GetSelectedCharacter();
	
	RemoveChild(m_MainMenu, true);

	m_MenuCam->SetActive(false);

	m_ChunkManager = AddChild(new ChunkManager(m_MaterialManager));

	m_Player = AddChild(new PlayerChicken(4,m_ChunkManager, m_MaterialManager, m_CurrentChar));

	m_Camera = AddChild(new GameCamera(m_Player, 50.f, -15.f, 600.f, 2, 8.f));

	m_HUD = AddChild(new InGameMenu());

	m_InMainMenu = false;

	m_SceneContext.pLights->SetDirectionalLight({ 0.f, 0.f, -5.f }, m_LightDir);
	ShadowMapRenderer::Get()->SetProjectionSize(15.f);

}

void MainGameScene::ResetGame()
{
	RemoveChild(m_Player, true);
	RemoveChild(m_ChunkManager, true);
	RemoveChild(m_HUD, true);
	m_Camera->GetComponent<CameraComponent>()->SetActive(false);
	RemoveChild(m_Camera, true);

	m_ChunkManager = AddChild(new ChunkManager(m_MaterialManager));

	m_Player = AddChild(new PlayerChicken(4, m_ChunkManager, m_MaterialManager, m_CurrentChar));

	m_Camera = AddChild(new GameCamera(m_Player, 50.f, -15.f, 600.f, 2, 8.f));

	m_HUD = AddChild(new InGameMenu());
}
