
class Car final : public GameObject
{

public:
	Car(UINT materialID,float speed,int dir);
	~Car() override;

	Car(const Car& other) = delete;
	Car(Car&& other) noexcept = delete;
	Car& operator=(const Car& other) = delete;
	Car& operator=(Car&& other) noexcept = delete;

	//bool IsPassable(int x);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override;
	void Draw(const SceneContext& /*sceneContext*/) override {};

private:

	UINT m_MaterialID=0;

	int m_Direction = 0;
	float m_Speed = 0;

	GameObject* m_Collider{};
	PxMaterial* m_PhysxMat{};

	//sound
	FMOD::Channel* m_pCarHonkChannel{};
	FMOD::Sound* m_pCarHonkSound{};
	//sound
	FMOD::Channel* m_carEngChan{};
	FMOD::Sound* m_CarEngSound{};
	void PlaySoundHorn(int times);
	bool m_SoundPlayed = false;
};