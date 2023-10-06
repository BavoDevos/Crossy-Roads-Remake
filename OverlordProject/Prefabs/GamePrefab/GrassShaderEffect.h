class GameMaterialManager;

class GrassShaderEffect final : public GameObject
{

public:
	GrassShaderEffect(unsigned int grassAmount,float gameWidth,float SpreadAmount = 0.48f);
	~GrassShaderEffect() override;

	GrassShaderEffect(const GrassShaderEffect& other) = delete;
	GrassShaderEffect(GrassShaderEffect&& other) noexcept = delete;
	GrassShaderEffect& operator=(const GrassShaderEffect& other) = delete;
	GrassShaderEffect& operator=(GrassShaderEffect&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override {};
	void Draw(const SceneContext& sceneContext) override;
private:
	float m_GameWidth=0;

	float m_CurrentWindOffset = 0;

	float m_Movement = -0.03f;

	float m_MaxWindOffset = 0.04f;
	float m_MinWindOffset = -0.04f;

	int dir = -1;

	float m_SpreadAmount = 0.49f;

	//grass shader
	ID3DX11Effect* m_pGrassEffect{};
	ID3DX11EffectTechnique* m_pGrassTechnique{};
	ID3D11InputLayout* m_pGrassInputLayout{};

	bool m_BufferMade{};

	ID3D11Buffer* m_pGrassVertexBuffer{};
	unsigned int m_GrassAmount{ 30 };
	std::vector<VertexGrass> m_GrassPoints{};
};