#include "stdafx.h"
#include "RoadStripesShader.h"

RoadStripesShader::RoadStripesShader(int gameWidth)
	:m_GameWidth{gameWidth}
{
}

RoadStripesShader::~RoadStripesShader()
{
	SafeRelease(m_pStripesVertexBuffer);
}

void RoadStripesShader::Initialize(const SceneContext& sceneContext)
{

	//initialzie stripes shader
	m_pStripesEffect = ContentManager::Load<ID3DX11Effect>(L"Effects/CrossyRoad/RoadStripes.fx");
	m_pStripesTechnique = m_pStripesEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(sceneContext.d3dContext.pDevice, m_pStripesTechnique, &m_pStripesInputLayout);

	VertexStripe stripe{};
	stripe.WidthLength.x = m_StripeWidth;
	stripe.WidthLength.y = m_StripeLength;
	for (float i{ -2.f * m_GameWidth }; i < m_GameWidth * 2.f; i += m_StripesInterval)
	{
		stripe.color = MathHelper::HSLtoRGB(50.f / 360, 0.6f, 0.4f, 1.f);
		stripe.Position = XMFLOAT3{ i, 0.f, -0.5f };
		m_Stripes.emplace_back(stripe);
	}
}

void RoadStripesShader::Draw(const SceneContext& sceneContext)
{
	if (true)
	{
		//todo: only build the vertex buffer if it hasn't been done before!
		if (!m_BufferMade)
		{
			UINT nrStripes{ (UINT)m_Stripes.size() };

			//build buffer
			SafeRelease(m_pStripesVertexBuffer);
			D3D11_BUFFER_DESC bufferDesc{};
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = sizeof(VertexStripe) * nrStripes;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			HRESULT result = sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pStripesVertexBuffer);
			HANDLE_ERROR(result);

			//fill buffer
			if (m_pStripesVertexBuffer)
			{
				// Finally fill the  buffer. You will need to create a D3D11_MAPPED_SUBRESOURCE
				// Next you will need to use the device context to map the vertex buffer to the mapped resource
				// use memcpy to copy all our sprite vertices (m_Sprites) to the mapped resource (D3D11_MAPPED_SUBRESOURCE::pData)
				// unmap the vertex buffer

				D3D11_MAPPED_SUBRESOURCE mappedResource{};
				sceneContext.d3dContext.pDeviceContext->Map(m_pStripesVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, m_Stripes.data(), sizeof(VertexStripe) * nrStripes);
				sceneContext.d3dContext.pDeviceContext->Unmap(m_pStripesVertexBuffer, 0);
			}

			m_BufferMade = true;

		}

		//update variables
		XMMATRIX lightWVPmatrix = (XMLoadFloat4x4(&GetTransform()->GetWorld()) * XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()));
		XMFLOAT4X4 lightWVP{};
		XMStoreFloat4x4(&lightWVP, lightWVPmatrix);
		m_pStripesEffect->GetVariableByName("gWorldViewProj_Light")->AsMatrix()->SetMatrix((float*)&lightWVP);

		XMMATRIX wvpMatrix = (XMLoadFloat4x4(&GetTransform()->GetWorld()) * XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection()));
		XMFLOAT4X4 wvp{};
		XMStoreFloat4x4(&wvp, wvpMatrix);
		m_pStripesEffect->GetVariableByName("gWorldViewProj")->AsMatrix()->SetMatrix(&wvp._11);


		m_pStripesEffect->GetVariableByName("gShadowMap")->AsShaderResource()->SetResource(ShadowMapRenderer::Get()->GetShadowMap());


		//render pipeline
		const auto pDeviceContext = sceneContext.d3dContext.pDeviceContext;
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		constexpr UINT stride = sizeof(VertexStripe);
		constexpr UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pStripesVertexBuffer, &stride, &offset);
		pDeviceContext->IASetInputLayout(m_pStripesInputLayout);

		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pStripesTechnique->GetDesc(&techDesc);
		for (unsigned int i{}; i < techDesc.Passes; ++i)
		{
			m_pStripesTechnique->GetPassByIndex(i)->Apply(0, pDeviceContext);
			pDeviceContext->Draw((UINT)m_Stripes.size(), 0);
		}
	}
}
