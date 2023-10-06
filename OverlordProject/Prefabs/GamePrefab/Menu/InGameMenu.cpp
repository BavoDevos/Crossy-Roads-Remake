#include "stdafx.h"
#include "InGameMenu.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"
#include "Materials/Post/Postblur.h"

InGameMenu::InGameMenu()
{
}

void InGameMenu::SetScore(int score)
{
	if (score <= m_Score)return;

	m_Score = score;
	m_ScoreFlag = true;
}

void InGameMenu::Reset()
{
	m_GameOver = false;
	m_GameOverFlag = false;
	m_PlayPressed = false;
	m_Score = 0;
	m_ScoreFlag = true;

	ShowDeathMenu(false);
	ShowSpawnScreen(true);
}

void InGameMenu::Initialize(const SceneContext& sceneContext)
{
	m_Font = ContentManager::Load<SpriteFont>(L"SpriteFonts/Ret.fnt");
	m_BigFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/BigT3.fnt");
	m_ScoreText = L"SCORE " + std::to_wstring(0);

	m_PlayButton = AddChild(new MenuButton(ButtonType::Play, (sceneContext.windowWidth / 2) + 10.f,580.f));
	m_HomeButton = AddChild(new MenuButton(ButtonType::Home,(sceneContext.windowWidth / 2) - 10.f-212.f,580.f));
	m_ExitButton = AddChild(new MenuButton(ButtonType::Exit, sceneContext.windowWidth-126.f,20.f,0.5f));
	m_PlayButton->SetButtonActive(false);
	m_HomeButton->SetButtonActive(false);
	m_ExitButton->SetButtonActive(false);
	/*m_PauseButton = AddChild(new MenuButton(ButtonType::Controls, sceneContext.windowWidth - 66.f - 20.f, 20.f, 0.5f));*/
	m_ExitButton->SetButtonActive(false);
	//m_Controls = AddChild(new ControlsKeyboard(500, 745));

	m_Controls = AddChild(new ControlsKeyboard(700, 620));
	m_TitleCard = new MenuButton(ButtonType::Title, sceneContext.windowWidth/2, (sceneContext.windowHeight / 2)-100, 0.8f);
	//m_TitleCard->GetTransform()->Translate(400, 600, 0.1f);
	m_TitleCard->SetButtonActive(true);

	AddChild(m_TitleCard);
}

void InGameMenu::Update(const SceneContext& sceneContext)
{
	if (m_ShowControls)
	{
		UpdateSpawnMenu(sceneContext);
	}
	else if (m_GameOver)
	{
		if (!m_GameOverFlag)
		{
			ShowDeathMenu(true);
		}

		UpdateDeathMenu(sceneContext);
	}
	else
	{
		UpdateInGame(sceneContext);
	}

}

void InGameMenu::ShowDeathMenu(bool show)
{
	m_PlayButton->SetButtonActive(show);
	m_HomeButton->SetButtonActive(show);
	m_ExitButton->SetButtonActive(show);
	m_GameOverFlag = show;
	m_ScoreFlag = true;
}

void InGameMenu::ShowSpawnScreen(bool show)
{
	m_TitleCard->SetButtonActive(show);
	m_ShowControls = show;
	m_Controls->SetHide(!show);
}

void InGameMenu::UpdateDeathMenu(const SceneContext& sceneContext)
{
	if (m_ScoreFlag)
	{
		m_ScoreText = std::to_wstring(m_Score);

		m_Offset = 0;

		for (wchar_t charr : m_ScoreText)
		{
			m_Offset += m_BigFont->GetMetric(charr).width;
		}

		m_Offset = m_Offset / 2;
	}

	TextRenderer::Get()->DrawText(m_BigFont, L"SCORE", { sceneContext.windowWidth / 2 - (64 * 2.5f) + 5.f, 150 }, { 0,0,0,1 });
	TextRenderer::Get()->DrawText(m_BigFont, m_ScoreText, { sceneContext.windowWidth / 2 - m_Offset + 5.f, 150 + 64 + 10 }, { 0,0,0,1 });

	TextRenderer::Get()->DrawText(m_BigFont, L"SCORE", {sceneContext.windowWidth / 2 -(64*2.5f), 150});
	TextRenderer::Get()->DrawText(m_BigFont, m_ScoreText, {sceneContext.windowWidth / 2- m_Offset, 150+64+10});



	const bool mousePressed{ InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON) };

	if (mousePressed)
	{
		const POINT mousePos{ InputManager::GetMousePosition() };

		if (m_PlayButton->IsPointInButton(mousePos))
		{
			m_PlayPressed = true;
		}
		else if (m_HomeButton->IsPointInButton(mousePos))
		{
			m_ToMenuPressed = true;
		}
		else if (m_ExitButton->IsPointInButton(mousePos))
		{
			PostQuitMessage(0);
		}
	}
}

void InGameMenu::UpdateSpawnMenu(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsActionTriggered(ReleaseForward)
		|| sceneContext.pInput->IsActionTriggered(ReleaseBackward)
		|| sceneContext.pInput->IsActionTriggered(ReleaseRight)
		|| sceneContext.pInput->IsActionTriggered(ReleaseLeft))
	{
		ShowSpawnScreen(false);
	}
}

void InGameMenu::UpdateInGame(const SceneContext& sceneContext)
{
	if (m_ScoreFlag)
	{
		m_ScoreText = L"SCORE " + std::to_wstring(m_Score);
		m_ScoreFlag = false;
	}

	TextRenderer::Get()->DrawText(m_Font, m_ScoreText, { sceneContext.windowWidth / 2 - 90.f, 20.f });
}
