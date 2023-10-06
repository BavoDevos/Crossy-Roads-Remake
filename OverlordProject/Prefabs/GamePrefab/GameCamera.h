#pragma once

class GameCamera : public GameObject
{
public:
	GameCamera(GameObject* objToFollow, float pitch, float yaw, float distance, int zOffset, float fovDegrees = 30.f);
	~GameCamera() override = default;

	GameCamera(const GameCamera& other) = delete;
	GameCamera(GameCamera&& other) noexcept = delete;
	GameCamera& operator=(const GameCamera& other) = delete;
	GameCamera& operator=(GameCamera&& other) noexcept = delete;

	void Reset() { GetTransform()->Translate(m_OriginalTransform); };
	void SetFollowObject(GameObject* object) { m_FollowObject = object; };

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:

	GameObject* m_FollowObject{ nullptr };
	XMFLOAT3 m_OriginalTransform;
	CameraComponent* m_pCamComp = nullptr;

	float m_Pitch{};
	float m_Yaw{};
	float m_Distance{};
	float m_FovDegrees{};
	int m_zOffset{};

};

