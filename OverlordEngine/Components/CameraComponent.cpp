#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{

	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active ? this : nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Parteng Game Object is nullptr");

	if (!pGameObject) return nullptr;
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Parent Scene is nullptr");
	
	//convert mouse pos to NDC
	POINT mousePos = InputManager::GetMousePosition();
	XMFLOAT2 halfScreenSize = XMFLOAT2(pScene->GetSceneContext().windowWidth / 2, pScene->GetSceneContext().windowHeight / 2);
	XMFLOAT3 ndc{ (mousePos.x - halfScreenSize.x) / halfScreenSize.x , (halfScreenSize.y - mousePos.y) / halfScreenSize.y , 0.f };

	//near point
	XMVECTOR point = XMLoadFloat3(&ndc);
	XMMATRIX viewProjInv = XMLoadFloat4x4(&m_ViewProjectionInverse);
	XMVECTOR nearPos = XMVector3TransformCoord(point, viewProjInv);

	//far point
	ndc.z = 1.f;
	point = XMLoadFloat3(&ndc);
	XMVECTOR farPos = XMVector3TransformCoord(point, viewProjInv);
	XMVECTOR rayDir = farPos - nearPos;
	rayDir = XMVector3Normalize(rayDir);

	XMFLOAT3 rayOrigin{};
	XMStoreFloat3(&rayOrigin, nearPos);
	XMFLOAT3 rayDirection{};
	XMStoreFloat3(&rayDirection, rayDir);


	PxQueryFilterData filterData{};
	filterData.data.word0 = UINT(ignoreGroups);
	PxRaycastBuffer hit{};
	if (pScene->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(rayOrigin), PhysxHelper::ToPxVec3(rayDirection), PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		RigidBodyComponent* pRigidBodyComponent = reinterpret_cast<RigidBodyComponent*>(hit.block.actor->userData);
		if (pRigidBodyComponent)
		{
			return pRigidBodyComponent->GetGameObject();
		}
	}
	return nullptr;
	
}