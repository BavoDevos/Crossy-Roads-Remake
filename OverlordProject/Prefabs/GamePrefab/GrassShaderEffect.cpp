#include "stdafx.h"
#include "GrassShaderEffect.h"
#include <random>
GrassShaderEffect::GrassShaderEffect(unsigned int grassAmount,float gameWidth,float spreadAmount)
	:m_GrassAmount{grassAmount},m_GameWidth{gameWidth},m_SpreadAmount{spreadAmount}
{
}

GrassShaderEffect::~GrassShaderEffect()
{
	SafeRelease(m_pGrassVertexBuffer);
}

void GrassShaderEffect::Initialize(const SceneContext& sceneContext)
{
	//grass effect setup
	m_pGrassEffect = ContentManager::Load<ID3DX11Effect>(L"Effects/CrossyRoad/Grass.fx");
	m_pGrassTechnique = m_pGrassEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(sceneContext.d3dContext.pDevice, m_pGrassTechnique, &m_pGrassInputLayout);



	const std::vector<XMFLOAT4> grassShades = {
		{ 0.0f, 0.66f, 0.33f, 1.0f },
		{ 0.0f, 0.75f, 0.3f, 1.0f }, 
		{ 0.0f, 0.61f, 0.35f, 1.0f }, 
		{ 0.0f, 0.72f, 0.28f, 1.0f },
		{ 0.0f, 0.58f, 0.38f, 1.0f },
		{ 0.0f, 0.69f, 0.25f, 1.0f },
		{ 0.0f, 0.54f, 0.41f, 1.0f },
		{ 0.1f, 0.66f, 0.33f, 1.0f },
		{ 0.1f, 0.75f, 0.3f, 1.0f },
		{ 0.1f, 0.61f, 0.35f, 1.0f },
		{ 0.1f, 0.72f, 0.28f, 1.0f },
		{ 0.1f, 0.58f, 0.38f, 1.0f },
		{ 0.1f, 0.69f, 0.25f, 1.0f },
	};


	// Initialize a random number generator
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<size_t> dist(0, grassShades.size() - 1);
	static std::uniform_real_distribution<float> dist2(-0.02f,0.02f);
	float randomOffset = dist2(gen);
	m_CurrentWindOffset = randomOffset;
	//fill the grass vector with random points
	VertexGrass vertex{};
	for (unsigned int i{}; i < m_GrassAmount; ++i)
	{
		vertex.Position = XMFLOAT3{ MathHelper::randF(-m_GameWidth - 0.4f, m_GameWidth + .4f), 0.f, MathHelper::randF(-m_SpreadAmount, m_SpreadAmount) };

		//vertex.color = XMFLOAT4{ 0,0.66f,0.33f,1 };

		size_t randomIndex = dist(gen);
		vertex.color = grassShades[randomIndex];
		m_GrassPoints.emplace_back(vertex);
	}
}

void GrassShaderEffect::Draw(const SceneContext& sceneContext)
{
	if (m_GrassAmount != 0)
	{
		//todo: only build the vertex buffer if it hasn't been done before!
		if (!m_BufferMade)
		{
			//build buffer
			SafeRelease(m_pGrassVertexBuffer);
			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = sizeof(VertexGrass) * m_GrassAmount;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			HRESULT result = sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pGrassVertexBuffer);
			HANDLE_ERROR(result);

			//fill buffer
			if (m_pGrassVertexBuffer)
			{
				// Finally fill the  buffer. You will need to create a D3D11_MAPPED_SUBRESOURCE
				// Next you will need to use the device context to map the vertex buffer to the mapped resource
				// use memcpy to copy all our sprite vertices (m_Sprites) to the mapped resource (D3D11_MAPPED_SUBRESOURCE::pData)
				// unmap the vertex buffer

				D3D11_MAPPED_SUBRESOURCE mappedResource{};
				sceneContext.d3dContext.pDeviceContext->Map(m_pGrassVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, m_GrassPoints.data(), sizeof(VertexGrass) * m_GrassAmount);
				sceneContext.d3dContext.pDeviceContext->Unmap(m_pGrassVertexBuffer, 0);
			}

			m_BufferMade = true;

		}

		auto check = m_CurrentWindOffset * 100.f;
		auto min = m_MinWindOffset * 100.f;
		auto max = m_MaxWindOffset * 100.f;

		if (dir == -1)
		{
			if (check < min)
			{
				dir = 1;
				m_Movement = 0.03f;
			}
		}
		else
		{
			if (check > max)
			{
				dir = -1;
				m_Movement = -0.03f;
			}
		}

		m_CurrentWindOffset += sceneContext.pGameTime->GetElapsed() * m_Movement;
		m_pGrassEffect->GetVariableByName("gWindOffset")->AsScalar()->SetFloat(m_CurrentWindOffset);
		//update variables
		XMMATRIX lightWVPmatrix = (XMLoadFloat4x4(&GetTransform()->GetWorld()) * XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()));
		XMFLOAT4X4 lightWVP{};
		XMStoreFloat4x4(&lightWVP, lightWVPmatrix);
		m_pGrassEffect->GetVariableByName("gWorldViewProj_Light")->AsMatrix()->SetMatrix((float*)&lightWVP);

		XMMATRIX wvpMatrix = (XMLoadFloat4x4(&GetTransform()->GetWorld()) * XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection()));
		XMFLOAT4X4 wvp{};
		XMStoreFloat4x4(&wvp, wvpMatrix);
		m_pGrassEffect->GetVariableByName("gWorldViewProj")->AsMatrix()->SetMatrix(&wvp._11);


		m_pGrassEffect->GetVariableByName("gShadowMap")->AsShaderResource()->SetResource(ShadowMapRenderer::Get()->GetShadowMap());


		//render pipeline
		const auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		constexpr UINT stride = sizeof(VertexGrass);
		constexpr UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pGrassVertexBuffer, &stride, &offset);
		pDeviceContext->IASetInputLayout(m_pGrassInputLayout);

		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pGrassTechnique->GetDesc(&techDesc);
		for (unsigned int i{}; i < techDesc.Passes; ++i)
		{
			m_pGrassTechnique->GetPassByIndex(i)->Apply(0, pDeviceContext);
			pDeviceContext->Draw(m_GrassAmount, 0);
		}

	}
}
