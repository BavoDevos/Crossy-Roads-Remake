
class GameMaterialManager;

class RoadStripesShader final : public GameObject
{

public:
	RoadStripesShader(int gameWidth);
	~RoadStripesShader() override;

	RoadStripesShader(const RoadStripesShader& other) = delete;
	RoadStripesShader(RoadStripesShader&& other) noexcept = delete;
	RoadStripesShader& operator=(const RoadStripesShader& other) = delete;
	RoadStripesShader& operator=(RoadStripesShader&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& /*sceneContext*/) override {};
	void Draw(const SceneContext& sceneContext) override;
private:
	int m_GameWidth = 0;
	//road stripes
	ID3DX11Effect* m_pStripesEffect{};
	ID3DX11EffectTechnique* m_pStripesTechnique{};
	ID3D11InputLayout* m_pStripesInputLayout{};
	ID3D11Buffer* m_pStripesVertexBuffer{};

	bool m_BufferMade{};

	const float m_StripeLength{ 0.7f };
	const float m_StripeWidth{ 0.08f };
	const float m_StripesInterval{ 1.5f };

	std::vector<VertexStripe> m_Stripes;
};