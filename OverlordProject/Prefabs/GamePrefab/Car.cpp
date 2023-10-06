#include "stdafx.h"
#include "Car.h"
#include <random>

Car::Car(UINT materialID, float speed,int dir)
	:m_MaterialID{materialID},m_Speed{speed},m_Direction{dir}
{
}

Car::~Car()
{
	m_PhysxMat->release();
	m_CarEngSound->release();
	m_pCarHonkSound->release();
}

void Car::Initialize(const SceneContext& /*sceneContext*/)
{
	auto Car = AddComponent(new ModelComponent(L"Meshes/Game/Car.ovm", true));
	Car->SetMaterial(m_MaterialID);

	GetTransform()->Rotate(0.f, 90.f * -m_Direction, 0.f, true);

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

	FMOD::System* pFmod = SoundManager::Get()->GetSystem();
	pFmod->createStream("Resources/SFX/Car_Toot.mp3", FMOD_DEFAULT, nullptr, &m_pCarHonkSound);
	pFmod->createStream("Resources/SFX/Car_Engine.wav", FMOD_DEFAULT, nullptr, &m_CarEngSound);


	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis2(0, 20);
	int hornrand = static_cast<int>(dis2(gen));

	if (hornrand == 6)
	{
		SoundManager::Get()->GetSystem()->playSound(m_CarEngSound, nullptr, false, &m_carEngChan);
		m_carEngChan->setVolume(0.2f);
		m_carEngChan->setLoopCount(-1);
		std::cout << "ENGINE PLAYING\n";
	}

}

void Car::Update(const SceneContext& sceneContext)
{
	//update position
	float XMovement = m_Speed * sceneContext.pGameTime->GetElapsed() * m_Direction;
	GetTransform()->Translate(GetTransform()->GetPosition().x + XMovement, 0.f, 0.f);

	//align collider with the car
	XMFLOAT3 pos = GetTransform()->GetWorldPosition();

	if (!m_SoundPlayed)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis2(0, 600);
		int hornrand = static_cast<int>(dis2(gen));
		std::uniform_real_distribution<> dis3(1,3);
		int horntimes = static_cast<int>(dis3(gen));

		if (hornrand == 5)
		{
			PlaySoundHorn(horntimes);
			std::cout << "HORN SOUND\n";
		}
	}

	//add slight offset so hitbox matches car mesh
	if (m_Direction == 1)
	{
		m_Collider->GetTransform()->Translate(pos.x + 0.3f, pos.y, pos.z);
	}
	else if (m_Direction == -1)
	{
		m_Collider->GetTransform()->Translate(pos.x - 0.2f, pos.y, pos.z);
	}
}

void Car::PlaySoundHorn(int times)
{
	SoundManager::Get()->GetSystem()->playSound(m_pCarHonkSound, nullptr, false, &m_pCarHonkChannel);
	m_pCarHonkChannel->setVolume(0.5f);
	m_pCarHonkChannel->setLoopCount(times);
	m_SoundPlayed = true;
}
