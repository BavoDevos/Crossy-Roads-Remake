#pragma once

enum class ButtonType
{
	Play,
	Exit,
	ArrowLeft,
	ArrowRight,
	Home,
	Controls,
	KeyboardArrows,
	KeyboardEmpty,
	BlackBox,
	Title
};

class MenuButton final : public GameObject
{

public:
	MenuButton(ButtonType buttonType, float posX,float posY,float scale = 1.f);
	~MenuButton() override {};

	MenuButton(const MenuButton& other) = delete;
	MenuButton(MenuButton&& other) noexcept = delete;
	MenuButton& operator=(const MenuButton& other) = delete;
	MenuButton& operator=(MenuButton&& other) noexcept = delete;

	bool IsPointInButton(POINT mousePos);
	ButtonType GetButtonType() { return m_ButtonType; };

	void SetButtonActive(bool active);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override {};
	void Draw(const SceneContext& /*sceneContext*/) override {};
private:

	XMFLOAT4 m_ButtonDimensions{};
	ButtonType m_ButtonType{};
	bool m_IsActive = false;

	float m_Scale = 1;

	void HideButton();
	void ShowButton();
};