#include "stdafx.h"
#include "ControlsKeyboard.h"

ControlsKeyboard::ControlsKeyboard(float x, float y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void ControlsKeyboard::SetHide(bool hide)
{
	if (m_Active == !hide)return;

	m_Active = !hide;
	
	m_ArrowDown->SetButtonActive(m_Active);
	m_ArrowLeft->SetButtonActive(m_Active);
	m_ArrowRight->SetButtonActive(m_Active);
	m_ArrowUp->SetButtonActive(m_Active);
	m_ArrowUp2->SetButtonActive(m_Active);

	for (MenuButton* button : m_BlankButtons)
	{
		button->SetButtonActive(m_Active);
	}
}

void ControlsKeyboard::Initialize(const SceneContext& /*sceneContext*/)
{
	auto x = m_Pos.x;
	auto y = m_Pos.y;

	float scale = 1.2f;

	float dim = 39.f*scale;
	float halfDim =dim/2;

	m_ArrowDown = new MenuButton(ButtonType::KeyboardArrows, x, y + halfDim, scale);
	m_ArrowDown->GetTransform()->Rotate(0, 0, 180);
	m_ArrowDown->SetButtonActive(true);
	AddChild(m_ArrowDown);

	m_ArrowUp = new MenuButton(ButtonType::KeyboardArrows, x, y- halfDim, scale);
	m_ArrowUp->GetTransform()->Rotate(0, 0, 0);
	m_ArrowUp->SetButtonActive(true);
	AddChild(m_ArrowUp);

	m_ArrowUp2 = new MenuButton(ButtonType::BlackBox, x, y - halfDim+3, scale);
	m_ArrowUp2->GetTransform()->Rotate(0, 0, 0);
	m_ArrowUp2->SetButtonActive(false);
	AddChild(m_ArrowUp2);

	//m_ArrowUp->GetTransform()->Scale(1.f);


	m_ArrowLeft = new MenuButton(ButtonType::KeyboardArrows, x- dim, y + halfDim, scale);
	m_ArrowLeft->GetTransform()->Rotate(0, 0, -90);
	m_ArrowLeft->SetButtonActive(true);
	AddChild(m_ArrowLeft);

	m_ArrowRight = new MenuButton(ButtonType::KeyboardArrows, x+ dim, y + halfDim, scale);
	m_ArrowRight->GetTransform()->Rotate(0, 0, 90);
	m_ArrowRight->SetButtonActive(true);
	AddChild(m_ArrowRight);


	auto box = new MenuButton(ButtonType::KeyboardEmpty, x - dim-dim, y + halfDim, scale);
	box->GetTransform()->Rotate(0, 0, 0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);

	box = new MenuButton(ButtonType::KeyboardEmpty, x + dim+dim, y + halfDim, scale);
	box->GetTransform()->Rotate(0, 0,0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);

	box = new MenuButton(ButtonType::KeyboardEmpty, x - dim - dim, y - halfDim, scale);
	box->GetTransform()->Rotate(0, 0, 0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);

	box = new MenuButton(ButtonType::KeyboardEmpty, x + dim + dim, y - halfDim, scale);
	box->GetTransform()->Rotate(0, 0, 0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);

	box = new MenuButton(ButtonType::KeyboardEmpty, x - dim, y - halfDim, scale);
	box->GetTransform()->Rotate(0, 0, 0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);

	box = new MenuButton(ButtonType::KeyboardEmpty, x + dim, y - halfDim, scale);
	box->GetTransform()->Rotate(0, 0, 0);
	box->SetButtonActive(true);
	m_BlankButtons.push_back(box);
	AddChild(box);


	//m_ArrowUp->GetTransform()->Scale(1.f);
	//m_ArrowLeft->GetTransform()->Scale(1.f);
	//m_ArrowRight->GetTransform()->Scale(1.f);
	//m_ArrowDown->GetTransform()->Scale(1.f);

	//auto box = new MenuButton(ButtonType::KeyboardEmpty, x + dim, y - 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);

	//box = new MenuButton(ButtonType::KeyboardEmpty, x - dim, y - 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);

	//box = new MenuButton(ButtonType::KeyboardEmpty, x - dim - dim, y - 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);

	//box = new MenuButton(ButtonType::KeyboardEmpty, x + 42 + 42, y - 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);

	//box = new MenuButton(ButtonType::KeyboardEmpty, x - 42 - 42, y + 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);

	//box = new MenuButton(ButtonType::KeyboardEmpty, x + 42 + 42, y + 21.f, 1.f);
	//box->SetButtonActive(true);
	//AddChild(box);
}

void ControlsKeyboard::Update(const SceneContext& sceneContext)
{

	if (!m_Active)return;
	accu += sceneContext.pGameTime->GetElapsed();

	if (accu > 0.4f)
	{
		if (up)
		{
			m_ArrowUp->SetButtonActive(false);
			m_ArrowUp2->SetButtonActive(true);
			up = false;
		}
		else
		{
			m_ArrowUp->SetButtonActive(true);
			m_ArrowUp2->SetButtonActive(false);
			up = true;
		}
		accu = 0;
	}
}
