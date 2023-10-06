#include "stdafx.h"
#include "MainMenu.h"
#include "Prefabs/CubePrefab.h"

#include "Materials/DiffuseMaterial.h"
#include "Prefabs/GamePrefab/GrassShaderEffect.h"
#include "Prefabs/GamePrefab/RoadStripesShader.h"
#include "Prefabs/GamePrefab/Car.h"
#include "Prefabs/GamePrefab/Menu/ControlsKeyboard.h"

MainMenu::MainMenu(GameMaterialManager* matman)
	:m_MaterialManager{matman}
{
}

void MainMenu::Initialize(const SceneContext& sceneContext)
{

	m_Font = ContentManager::Load<SpriteFont>(L"SpriteFonts/BigT3.fnt");

	m_PlayButton = AddChild(new MenuButton(ButtonType::Play, (sceneContext.windowWidth / 2) + 10.f, 580.f));
	m_ExitButton = AddChild(new MenuButton(ButtonType::Exit, (sceneContext.windowWidth / 2) - 10.f - 212.f, 580.f));


	m_ArrowLeft = AddChild(new MenuButton(ButtonType::ArrowLeft, (sceneContext.windowWidth / 2) - 180.f-(102.f*0.7f), 340.f,0.7f));
	m_ArrowRight = AddChild(new MenuButton(ButtonType::ArrowRight, (sceneContext.windowWidth / 2) + 180.f, 340.f,0.7f));


	auto chick = AddChild(new GameObject());
	auto chickenMod = chick->AddComponent(new ModelComponent(L"Meshes/Game/Chicken.ovm"));
	auto matId = m_MaterialManager->GetMaterialId(GameMaterials::Chicken);
	chickenMod->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
	chickenMod->GetTransform()->Translate(-0.f, 0.f, -1.f);
	chickenMod->GetTransform()->Rotate(0, 0, 0);
	chickenMod->GetTransform()->Scale(1.5f);
	m_CurrentModelObject = chick;
	m_CurrentModel = chickenMod;

	m_GameTitle1 = L"Crossy";
	m_GameTitle2 = L"Road";
	m_GameTitle3 = L"Crossy";
	m_GameTitle4 = L"Road";

	auto titleBannerOb = AddChild(new GameObject());
	m_TitleBanner = titleBannerOb->AddComponent(new SpriteComponent(L"Textures/Game/MenuBanner.png", { 0.f, 0.f }));
	titleBannerOb->GetTransform()->Translate(0,sceneContext.windowHeight-47.f, 0);
	
	//auto titleBannerOb2 = AddChild(new GameObject());
	//titleBannerOb2->AddComponent(new SpriteComponent(L"Textures/Game/Foam.png", { 0.f, 0.f },XMFLOAT4(Colors::CadetBlue)));
	//titleBannerOb2->GetTransform()->Translate(sceneContext.windowWidth / 2 - (64 * 3)-23.f, 10.f,0.1f);
	//titleBannerOb2->GetTransform()->Scale(1.696f, 0.6f, 1);

	LoadMenuScene();

	m_PlayButton->SetButtonActive(true);
	m_ExitButton->SetButtonActive(true);
	m_ArrowLeft->SetButtonActive(true);
	m_ArrowRight->SetButtonActive(true);
	

	//m_Car = AddChild(new GameObject);
	//auto Car = m_Car->AddComponent(new ModelComponent(L"Meshes/Game/Car.ovm", true));
	//Car->SetMaterial(m_MaterialManager->GetMaterialId(GameMaterials::CarPurple));
	//m_Car->GetTransform()->Scale(2.8f);
	//m_Car->GetTransform()->Translate(0, -0.1f, 60);


}

void MainMenu::Update(const SceneContext& sceneContext)
{
	m_CurrentRot += sceneContext.pGameTime->GetElapsed() * m_RotSpeed;
	m_AccuRotTime += sceneContext.pGameTime->GetElapsed();
	m_Test+= sceneContext.pGameTime->GetElapsed();

	m_CurrentModelObject->GetTransform()->Rotate(0, m_CurrentRot, 0);

	TextRenderer::Get()->DrawText(m_Font, m_GameTitle3, { sceneContext.windowWidth / 2 - (64*3) + 5.f, 20.f}, {XMFLOAT4{Colors::Black}});
	TextRenderer::Get()->DrawText(m_Font, m_GameTitle4, { sceneContext.windowWidth / 2 - (64 *2) + 5.f, 80.f }, { XMFLOAT4{Colors::Black} });

	//TextRenderer::Get()->DrawText(m_Font, m_GameTitle3, { sceneContext.windowWidth / 2 - (64 * 3)+5.f, 17.f }, { XMFLOAT4{Colors::Black} });
	//TextRenderer::Get()->DrawText(m_Font, m_GameTitle4, { sceneContext.windowWidth / 2 - (64 * 2)+5.f, 77.f }, { XMFLOAT4{Colors::Black} });

	//TextRenderer::Get()->DrawText(m_Font, m_GameTitle3, { sceneContext.windowWidth / 2 - (64 * 3), 15.f }, { XMFLOAT4{Colors::Black} });
	//TextRenderer::Get()->DrawText(m_Font, m_GameTitle4, { sceneContext.windowWidth / 2 - (64 * 2), 75.f }, { XMFLOAT4{Colors::Black} });

	TextRenderer::Get()->DrawText(m_Font, m_GameTitle1, { sceneContext.windowWidth / 2 - (64 * 3), 20.f });
	TextRenderer::Get()->DrawText(m_Font, m_GameTitle2, { sceneContext.windowWidth / 2 - (64 * 2), 80.f });

	HandleButtons();

	if (!m_CurrentModelIsAnimated)
	{

		if (m_Dir == 1)
		{
			m_SquishFactor = std::lerp(m_SquishFactor, 0.8f, 0.015f);
			if (m_SquishFactor <= 0.8f + 0.05f)  // Add a small threshold for bouncing
			{
				m_Dir = -1;
			}
		}
		else
		{
			m_SquishFactor = std::lerp(m_SquishFactor, 1.0f, 0.015f);
			if (m_SquishFactor >= 1.0f - 0.05f)  // Add a small threshold for bouncing
			{
				m_Dir = 1;
			}
		}

		// Apply squish using interpolated m_SquishFactor
		m_CurrentModelObject->GetTransform()->Scale(1.5f, m_SquishFactor * m_MaxSquishScale+0.5f, 1.5f);

	}
}

void MainMenu::CycleCharacters(int step)
{
	int current = static_cast<int>(m_SelectedCharacter);
	int numCharacters = static_cast<int>(Character::count);//get the last but remove 1 cause count should not be considered
	int next = (current + step + numCharacters) % numCharacters;
	m_SelectedCharacter = static_cast<Character>(next);

	SwitchCharacter();
}

void MainMenu::SwitchCharacter()
{
	RemoveChild(m_CurrentModelObject, true);
	auto NewModelObject = AddChild(new GameObject());
	ModelComponent* NewModel = nullptr;
	UINT matId;
	ModelAnimator* modelAnimator;

	
	
	switch (m_SelectedCharacter)
	{
	case Character::Chicken:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Chicken.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Chicken);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::Rambo:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Man/Rambo.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.5f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"PickedAnim");
		modelAnimator->Play();
		m_CurrentModelIsAnimated = true;
		break;
	case Character::Zombie:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Man/Zombie.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.35f);
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"PickedAnim");
		modelAnimator->Play();
		m_CurrentModelIsAnimated = true;
		break;
	case Character::SquidGuy:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Man/Squid.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.35f);
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"PickedAnim");
		modelAnimator->Play();
		m_CurrentModelIsAnimated = true;
		break;
	case Character::Demon:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Man/Demon.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.35f);
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"PickedAnim");
		modelAnimator->Play();
		m_CurrentModelIsAnimated = true;
		break;
	case Character::MuricaMan:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Man/MuricaMan.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.35f);
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"PickedAnim");
		modelAnimator->Play();
		m_CurrentModelIsAnimated = true;
		break;
	case Character::Cat:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Cat.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Cat);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::Doge:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Doge.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Doge);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::Penguin:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Pengy.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Penguin);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::Pig:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Pig.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Pig);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::Frog:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Frog.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Frog);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	case Character::count:
		NewModel = NewModelObject->AddComponent(new ModelComponent(L"Meshes/Game/Doge.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Doge);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
		m_CurrentModelIsAnimated = false;
		break;
	default:
		break;
	}

	if (m_CurrentModelIsAnimated)
	{
		NewModel->GetTransform()->Translate(-0.f, -0.f, -1.5f);
		NewModel->GetTransform()->Rotate(0, 0, 0);
		NewModel->GetTransform()->Scale(1.5f);
	}

	m_CurrentModelObject = NewModelObject;
	m_CurrentModel = NewModel;
}

void MainMenu::HandleButtons()
{
	const bool mousePressed{ InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON) };

	if (mousePressed)
	{
		const POINT mousePos{ InputManager::GetMousePosition() };

		if (m_PlayButton->IsPointInButton(mousePos))
		{
			m_PlayPressed = true;
			std::cout << "Pressed Play\n";
		}
		else if (m_ExitButton->IsPointInButton(mousePos))
		{

			PostQuitMessage(0);
		}
		else if(m_ArrowLeft->IsPointInButton(mousePos))
		{
			std::cout << "Pressed arrowL\n";
			CycleCharacters(-1);
		}
		else if (m_ArrowRight->IsPointInButton(mousePos))
		{
			std::cout << "Pressed arrowR\n";
			CycleCharacters(1);
		}
	}
}

void MainMenu::LoadMenuScene()
{
	//grass tiles left
	bool Color = false;
	for (float i = -12; i <= -4; i += 2)
	{
		auto grass = AddChild(new GameObject());
		ModelComponent* mc = grass->AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
		if (Color)
		{
			auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass1);
			mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
		}
		else
		{
			auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass2);
			mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
		}
		auto grassBlades = AddChild(new GrassShaderEffect(80, 0.2f, 10));
		grass->GetTransform()->Translate(i, 0, 0);
		grass->GetTransform()->Scale(20, 1, 2);
		grass->GetTransform()->Rotate(0, 90, 0);
		grassBlades->GetTransform()->Translate(i, 0, 0);
		grassBlades->GetTransform()->Scale(1.5f);
		//grassBlades->GetTransform()->Rotate(0, 90, 0);
		Color = !Color;
	}


	//road tiles middle
	for (float i = -4; i <= 3; i += 2)
	{
		auto grass = AddChild(new GameObject());
		ModelComponent* mc = grass->AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
		auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Road);
		mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
		grass->GetTransform()->Translate(i, 0, 0);
		grass->GetTransform()->Scale(20, 1, 2);
		grass->GetTransform()->Rotate(0, 90, 0);
	}


	//stripes
	auto roadStripes = AddChild(new GameObject());
	roadStripes->AddChild(new RoadStripesShader(int(3)));
	roadStripes->GetTransform()->Rotate(0, 90, 0);
	roadStripes->GetTransform()->Translate(1.2f, 0, 9.2f);
	roadStripes->GetTransform()->Scale(2.4f);
	

	//grass tiles right
	for (float i = 4; i <= 12; i += 2)
	{
		auto grass = AddChild(new GameObject());
		ModelComponent* mc = grass->AddComponent(new ModelComponent(L"Meshes/Game/Chunk.ovm", false));
		if (Color)
		{
			auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass1);
			mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
		}
		else
		{
			auto MatId = m_MaterialManager->GetMaterialId(GameMaterials::Grass2);
			mc->SetMaterial(MaterialManager::Get()->GetMaterial(MatId));
		}
		auto grassBlades = AddChild(new GrassShaderEffect(80, 0.2f, 10));
		grass->GetTransform()->Translate(i, 0, 0);
		grass->GetTransform()->Scale(20, 1, 2);
		grass->GetTransform()->Rotate(0, 90, 0);
		grassBlades->GetTransform()->Translate(i, 0, 0);
		grassBlades->GetTransform()->Scale(1.5f);
		//grassBlades->GetTransform()->Rotate(0, 90, 0);
		Color = !Color;
	}

	//rocks
	auto obstace = AddChild(new GameObject());
	auto ObstacleModel = obstace->AddComponent(new ModelComponent(L"Meshes/Game/BigRock.ovm", true));
	auto matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
	ObstacleModel->SetMaterial(matId);
	obstace->GetTransform()->Translate(-12.8f, 0, 24);
	obstace->GetTransform()->Rotate(0, 180, 0);
	obstace->GetTransform()->Scale(5.8f, 5.5f, 5.7f);

	auto obstace2 = AddChild(new GameObject());
	auto ObstacleModel2 = obstace2->AddComponent(new ModelComponent(L"Meshes/Game/BigRock.ovm", true));
	matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
	ObstacleModel2->SetMaterial(matId);
	obstace2->GetTransform()->Translate(10.2f, -0.5f, 24);
	obstace2->GetTransform()->Rotate(0, 270, 0);
	obstace2->GetTransform()->Scale(5.27f);


	auto obstace3 = AddChild(new GameObject());
	auto ObstacleModel3 = obstace3->AddComponent(new ModelComponent(L"Meshes/Game/BigRock2.ovm", true));
	matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
	ObstacleModel3->SetMaterial(matId);
	obstace3->GetTransform()->Translate(5.f, 8.8f, 26);
	obstace3->GetTransform()->Rotate(0, 0, 90);
	obstace3->GetTransform()->Scale(3.8f);


	auto obstace4 = AddChild(new GameObject());
	auto ObstacleModel4 = obstace4->AddComponent(new ModelComponent(L"Meshes/Game/BigRock2.ovm", true));
	matId = m_MaterialManager->GetMaterialId(GameMaterials::Boulder);
	ObstacleModel4->SetMaterial(matId);
	obstace4->GetTransform()->Translate(-5.f, 8.8f, 24);
	obstace4->GetTransform()->Rotate(0, 0, -90);
	obstace4->GetTransform()->Scale(3.8f);


	auto obstace5 = AddChild(new GameObject());
	auto ObstacleModel5 = obstace5->AddComponent(new ModelComponent(L"Meshes/UnitPlane.ovm", true));
	matId = m_MaterialManager->GetMaterialId(GameMaterials::TunnelExit);
	ObstacleModel5->SetMaterial(matId);
	obstace5->GetTransform()->Translate(0.2f, 0.f, 30);
	obstace5->GetTransform()->Rotate(-90, 0, 0);
	obstace5->GetTransform()->Scale(1.2f);

}

