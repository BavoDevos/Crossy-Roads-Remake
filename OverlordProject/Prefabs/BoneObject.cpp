#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_pMaterial{ pMaterial }
	, m_Length{ length }
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	// The incoming bone must be translated along the X-axis so it is located next this bone
	pBone->GetTransform()->Translate(m_Length, 0.0f, 0.0f);

	// Add the incoming bone as a child to the parent (this)
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	// Calculate the inverse world matrix
	const XMMATRIX inverseWorldMatrix{ XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&GetTransform()->GetWorld())) };

	// Store the matrix in the bindpose variable
	XMStoreFloat4x4(&m_BindPose, inverseWorldMatrix);

	// Calculate the bind poses of the children
	for (BoneObject* pChild : GetChildren<BoneObject>())
	{
		pChild->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	// Create a GameObject
	GameObject* pEmpty{ new GameObject{} };
	AddChild(pEmpty);

	// Add a ModelComponent
	ModelComponent* pModel{ pEmpty->AddComponent(new ModelComponent{ L"Meshes/Bone.ovm" }) };

	// Assign the material to the model
	pModel->SetMaterial(m_pMaterial);

	// Transform the bone
	pEmpty->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
	pEmpty->GetTransform()->Scale(m_Length);
}
