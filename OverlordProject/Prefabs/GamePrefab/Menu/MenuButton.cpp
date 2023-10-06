#include "stdafx.h"
#include "MenuButton.h"

MenuButton::MenuButton(ButtonType buttonType, float posX, float posY,float scale)
	:m_ButtonType(buttonType),m_Scale{scale}
{
	m_ButtonDimensions.x = posX;
	m_ButtonDimensions.y = posY;
}

bool MenuButton::IsPointInButton(POINT mousePos)
{
	if (!m_IsActive)return false;

	const bool isInsideX{ m_ButtonDimensions.x <= mousePos.x && mousePos.x <= m_ButtonDimensions.z };
	const bool isInsideY{ m_ButtonDimensions.y <= mousePos.y && mousePos.y <= m_ButtonDimensions.w };

	return isInsideX && isInsideY;
}

void MenuButton::SetButtonActive(bool active)
{
	if (active)
	{
		ShowButton();
	}
	else
	{
		HideButton();
	}
}

void MenuButton::Initialize(const SceneContext& /*sceneContext*/)
{
	switch (m_ButtonType)
	{
	case ButtonType::Play:	
		AddComponent(new SpriteComponent(L"Textures/Game/Play.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x,m_ButtonDimensions.y,0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (212.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (112.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::Exit:
		AddComponent(new SpriteComponent(L"Textures/Game/Exit.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (212.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (112.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::ArrowLeft:
		AddComponent(new SpriteComponent(L"Textures/Game/ArrowL.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (102.f*m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (102.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::ArrowRight:
		AddComponent(new SpriteComponent(L"Textures/Game/ArrowR.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (102.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (102.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::Home:
		AddComponent(new SpriteComponent(L"Textures/Game/Home.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (212.f*m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (112.f*m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::Controls:
		AddComponent(new SpriteComponent(L"Textures/Game/Controller.png", { 0.f, 0.f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (132 * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (106.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::KeyboardArrows:
		AddComponent(new SpriteComponent(L"Textures/Game/ControlsUp.png", { 0.5f, 0.5f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (39.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (39.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::KeyboardEmpty:
		AddComponent(new SpriteComponent(L"Textures/Game/EmptyBox.png", { 0.5f, 0.5f },{1,1,1,0.3f}));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0.2f);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (39.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (39.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::BlackBox:
		AddComponent(new SpriteComponent(L"Textures/Game/ControlsUp2.png", { 0.5f, 0.5f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (36.f * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (36.f * m_Scale) + m_ButtonDimensions.y;
		break;
	case ButtonType::Title:
		AddComponent(new SpriteComponent(L"Textures/Game/title.png", { 0.5f, 0.5f }));
		GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0.2f);
		GetTransform()->Scale(m_Scale);
		m_ButtonDimensions.z = (800 * m_Scale) + m_ButtonDimensions.x;
		m_ButtonDimensions.w = (464.f * m_Scale) + m_ButtonDimensions.y;
		break;
	}
}

void MenuButton::HideButton()
{
	GetTransform()->Translate(-100, -100, -100);
	m_IsActive = false;
}

void MenuButton::ShowButton()
{
	GetTransform()->Translate(m_ButtonDimensions.x, m_ButtonDimensions.y, 0);
	m_IsActive = true;
}
