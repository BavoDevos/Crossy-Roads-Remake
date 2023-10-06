#pragma once
#include "Prefabs/GamePrefab/Menu/MenuButton.h"
class ControlsKeyboard final : public GameObject
{

public:
	ControlsKeyboard(float x,float y);
	~ControlsKeyboard() override {};

	ControlsKeyboard(const ControlsKeyboard& other) = delete;
	ControlsKeyboard(ControlsKeyboard&& other) noexcept = delete;
	ControlsKeyboard& operator=(const ControlsKeyboard& other) = delete;
	ControlsKeyboard& operator=(ControlsKeyboard&& other) noexcept = delete;

	//void SetButtonActive(bool active);
	void SetHide(bool hide);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};
private:

	MenuButton* m_ArrowUp{};
	MenuButton* m_ArrowUp2{};
	MenuButton* m_ArrowLeft{};
	MenuButton* m_ArrowRight{};
	MenuButton* m_ArrowDown{};

	SpriteComponent* m_BackDrop{};

	std::vector<MenuButton*> m_BlankButtons{};

	XMFLOAT2 m_Pos{};
	float accu = 0;
	bool up = true;
	bool m_Active = true;
};