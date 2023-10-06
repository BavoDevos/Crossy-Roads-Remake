#include "stdafx.h"
#include "Train.h"

Train::Train(UINT materialID, float speed, int dir, bool front)
	:m_MaterialID{ materialID }, m_Speed{ speed }, m_Direction{ dir },m_Front{front}
{

}

Train::~Train()
{
	m_pTrainSound->release();
}

void Train::Initialize(const SceneContext& /*sceneContext*/)
{

	if (m_Front)
	{
		auto Train = AddComponent(new ModelComponent(L"Meshes/Game/TrainF.ovm", true));
		Train->SetMaterial(m_MaterialID);
		FMOD::System* pFmod = SoundManager::Get()->GetSystem();
		pFmod->createStream("Resources/SFX/Train_Horn.wav", FMOD_DEFAULT, nullptr, &m_pTrainSound);
	}
	else
	{
		auto Train = AddComponent(new ModelComponent(L"Meshes/Game/TrainM.ovm", true));
		Train->SetMaterial(m_MaterialID);
		FMOD::System* pFmod = SoundManager::Get()->GetSystem();
		pFmod->createStream("Resources/SFX/Train_NoHorn.wav", FMOD_DEFAULT, nullptr, &m_pTrainSound);
	}

	if (m_Direction == 1)
	{
		GetTransform()->Rotate(0.f, 0.f, 0.f, true);
	}
	else
	{
		GetTransform()->Rotate(0.f, 180.f, 0.f, true);
	}

	//collider
	m_PhysxMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	m_Collider = AddChild(new GameObject());
	m_Collider->GetTransform()->Translate(0.f, 100.f, 0.f);

	auto RigidBody = m_Collider->AddComponent(new RigidBodyComponent(false));
	m_Collider->GetTransform()->Rotate(0.f, 90.f * -m_Direction, 0.f, true);
	m_Collider->GetTransform()->Translate(0.f, 90.f, 0.f);
	RigidBody->SetKinematic(true);
	RigidBody->AddCollider(PxBoxGeometry(0.5f, 0.6f, 0.9f), *m_PhysxMat, false);
	RigidBody->GetTransform()->Translate(0.f, 90.f, 0.f);

}

void Train::Update(const SceneContext& sceneContext)
{
	//update position
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, 0.f, 0.f);

	//align collider with the car
	XMFLOAT3 pos = GetTransform()->GetWorldPosition();

	//add slight offset so hitbox matches car mesh
	if (m_Direction == 1)
	{
		m_Collider->GetTransform()->Translate(pos.x + 0.3f, pos.y, pos.z);
		if (GetTransform()->GetPosition().x > -2.f&&!m_SoundPlayed)
		{
			SoundManager::Get()->GetSystem()->playSound(m_pTrainSound, nullptr, false, &m_pTrainChannel);
			m_pTrainChannel->setVolume(0.5f);
			m_SoundPlayed = true;
		}
	}
	else if (m_Direction == -1)
	{
		m_Collider->GetTransform()->Translate(pos.x - 0.2f, pos.y, pos.z);
		if (GetTransform()->GetPosition().x < 2.f && !m_SoundPlayed)
		{
			SoundManager::Get()->GetSystem()->playSound(m_pTrainSound, nullptr, false, &m_pTrainChannel);
			m_pTrainChannel->setVolume(0.5f);
			m_SoundPlayed = true;
		}
	}
}
