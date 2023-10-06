#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera(GameObject* objToFollow, float pitch, float yaw, float distance, int zOffset, float fovDegrees)
	: m_FollowObject{ objToFollow }
	, m_Pitch{ pitch }
	, m_Yaw{ yaw }
	, m_Distance{ distance }
	, m_FovDegrees{ fovDegrees }
	, m_zOffset{ zOffset }
	, m_OriginalTransform{}

{


}

void GameCamera::Initialize(const SceneContext&)
{
	m_pCamComp = AddComponent(new CameraComponent());
	m_pCamComp->UseOrthographicProjection();
	m_pCamComp->SetOrthoSize(8.f);
	m_pCamComp->SetFieldOfView(XMConvertToRadians(m_FovDegrees));
	//rotation
	GetTransform()->Rotate(m_Pitch, m_Yaw, 0.f);

	//lookat translation
	XMFLOAT3 forward{ sin(XMConvertToRadians(m_Yaw)) * cos(XMConvertToRadians(m_Pitch)) ,
					sin(XMConvertToRadians(-m_Pitch)),
					cos(XMConvertToRadians(m_Yaw)) * cos(XMConvertToRadians(m_Pitch))
	};

	m_OriginalTransform = XMFLOAT3(-forward.x * m_Distance, -forward.y * m_Distance, -forward.z * m_Distance + m_zOffset);
	GetTransform()->Translate(m_OriginalTransform);

	//activate
	m_pCamComp->SetActive(true);
}

void GameCamera::Update(const SceneContext& sceneContext)
{
	//follow the player in the z direction (forward game direction)
	float lerp{ 2.f * sceneContext.pGameTime->GetElapsed() };
	MathHelper::Clamp(lerp, 1.f, 0.f);
	GetTransform()->Translate(m_OriginalTransform.x, m_OriginalTransform.y, m_OriginalTransform.z + std::lerp((GetTransform()->GetPosition().z - m_OriginalTransform.z), m_FollowObject->GetTransform()->GetWorldPosition().z, lerp));


}
