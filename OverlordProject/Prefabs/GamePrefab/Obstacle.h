
class GameMaterialManager;

class Obstacle final : public GameObject
{

public:
	Obstacle(GameMaterialManager* materialManager,bool edgeObstacle);
	~Obstacle() override = default;

	Obstacle(const Obstacle& other) = delete;
	Obstacle(Obstacle&& other) noexcept = delete;
	Obstacle& operator=(const Obstacle& other) = delete;
	Obstacle& operator=(Obstacle&& other) noexcept = delete;


protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override {};
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	GameMaterialManager* m_MaterialManager = nullptr;

	int m_ObstacleType = 0;
};