
class Train final : public GameObject
{

public:
	Train(UINT materialID, float speed, int dir,bool front=false);
	~Train() override;

	Train(const Train& other) = delete;
	Train(Train&& other) noexcept = delete;
	Train& operator=(const Train& other) = delete;
	Train& operator=(Train&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	UINT m_MaterialID = 0;

	int m_Direction = 0;
	float m_Speed = 0;
	bool m_Front = false;
	GameObject* m_Collider{};
	PxMaterial* m_PhysxMat{};

	bool m_SoundPlayed = false;
	//sound
	FMOD::Channel* m_pTrainChannel{};
	FMOD::Sound* m_pTrainSound{};
};