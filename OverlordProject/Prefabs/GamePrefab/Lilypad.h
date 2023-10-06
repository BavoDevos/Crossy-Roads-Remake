class GameMaterialManager;

class Lilypad final : public GameObject
{

public:
	Lilypad(GameMaterialManager* materialManager);
	~Lilypad() override = default;

	Lilypad(const Lilypad& other) = delete;
	Lilypad(Lilypad&& other) noexcept = delete;
	Lilypad& operator=(const Lilypad& other) = delete;
	Lilypad& operator=(Lilypad&& other) noexcept = delete;

	//bool IsPassable(int x);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	GameMaterialManager* m_MaterialManager = nullptr;

	int m_RotationDir = 0;
	float m_CurrentRot = 0;
	const float m_RotSpeed = 10;

	float m_AccuRotTime = 0;

	float m_RotTime = 0;
};