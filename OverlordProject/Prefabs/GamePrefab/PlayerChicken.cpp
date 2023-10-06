#include "stdafx.h"
#include "PlayerChicken.h"
#include "Prefabs/GamePrefab/ChunkManager.h"

void PlayerChicken::Respawn()
{
	m_Dead = false;
	GetTransform()->Translate(0.f, 0.f, 0.f);
	GetTransform()->Rotate(0.f, 0.f, 0.f);
	m_TargetPosX = m_TargetPosZ = m_PrevX = m_PrevZ = 0;
	m_SquishFactor = 0.f;
	m_KeyPressed = false;

	m_CurrentChunkI = 0;
	m_CurrentLastChunk = -3;
	m_AllowedBackTrack = 3;
}

void PlayerChicken::Initialize(const SceneContext&)
{
	//auto chickenMod = AddComponent(new ModelComponent(L"Meshes/Game/Chicken.ovm"));
	//auto matId = m_MaterialManager->GetMaterialId(GameMaterials::Chicken);
	//chickenMod->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
	////chickenMod->GetTransform()->Scale(0.005f, 0.05f, 0.05f);

	//testing
	//auto test = chickenMod->GetAnimator();
	//test->SetAnimation(L"PickedAnim");
	//test->Play();
	////test->Pause();
	////GetTransform()->Scale(1, 1, 1);

	InitModel();

	//physics

	PxMaterial* ChickenPhsxMat = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);

	auto rigidBody = AddComponent(new RigidBodyComponent(false));
	rigidBody->SetKinematic(true);
	rigidBody->AddCollider(PxSphereGeometry(0.1f), *ChickenPhsxMat, true);

	SetOnTriggerCallBack([=](GameObject* /*pTrigger*/, GameObject* /*pOther*/, PxTriggerAction action)
	{
			if (!m_Dead)
			{
				if (action == PxTriggerAction::ENTER && m_PrevZ > 1)
				{
					m_Dead = true;
					SoundManager::Get()->GetSystem()->playSound(m_pDieSound, nullptr, false, &m_pChannelDie);
					m_pChannelDie->setVolume(0.7f);
					GetTransform()->Scale(1, 0.2f, 1);
					m_DeathParticle->SpawnOneShot();
				}
			}
	});

	//particles
	m_DeathParticleObject = AddChild(new GameObject());

	m_DeathParticleSettings.minEmitterRadius = 0.f;
	m_DeathParticleSettings.maxEmitterRadius = -0.f;
	m_DeathParticleSettings.minScale = 1.2f;
	m_DeathParticleSettings.maxScale = 1.2f;
	m_DeathParticleSettings.velocity = { 0.f, 0.8f, 0.f };
	m_DeathParticleSettings.minSize = 1.f;
	m_DeathParticleSettings.maxSize = 1.f;
	m_DeathParticleSettings.minEnergy = 3.f;
	m_DeathParticleSettings.maxEnergy = 3.f;
	m_DeathParticleSettings.rotateFreely = false;

	m_DeathParticle = m_DeathParticleObject->AddComponent(new ParticleEmitterComponent(L"Textures/Game/DIE.png", m_DeathParticleSettings, 1, true));

	//sound 

	FMOD::System* pFmod = SoundManager::Get()->GetSystem(); 
	pFmod->createStream("Resources/SFX/ChickDeath.wav", FMOD_DEFAULT, nullptr, &m_pDieSound);
	pFmod->createStream("Resources/SFX/ChickJump.wav", FMOD_DEFAULT, nullptr, &m_pJumpSound);
	pFmod->createStream("Resources/SFX/Splash.mp3", FMOD_DEFAULT, nullptr, &m_pSplashSound);
}

void PlayerChicken::Update(const SceneContext& sceneContext)
{
	if (m_Dead)return;
	m_DeathParticleObject->GetTransform()->Translate(GetTransform()->GetPosition());
	//rotation handling on pressing movement keys
	if (sceneContext.pInput->IsActionTriggered(PressForward))
	{
		SetTargetRot(180.f);
		m_KeyPressed = true;
	}
	else if (sceneContext.pInput->IsActionTriggered(PressBackward))
	{
		SetTargetRot(0.f);
		m_KeyPressed = true;
	}
	else if (sceneContext.pInput->IsActionTriggered(PressLeft))
	{
		SetTargetRot(90.f);
		m_KeyPressed = true;
	}
	else if (sceneContext.pInput->IsActionTriggered(PressRight))
	{
		SetTargetRot(270.f);
		m_KeyPressed = true;
	}


	if (sceneContext.pInput->IsActionTriggered(ReleaseForward)
		|| sceneContext.pInput->IsActionTriggered(ReleaseBackward)
		|| sceneContext.pInput->IsActionTriggered(ReleaseRight)
		|| sceneContext.pInput->IsActionTriggered(ReleaseLeft))
		m_KeyPressed = false;

	if (m_JumpTimer <= 0.f)
	{
		m_PrevX = m_TargetPosX;
		m_PrevZ = m_TargetPosZ;

		bool jumped = false;

		//handle movement (and rotation) on key release
		if (sceneContext.pInput->IsActionTriggered(ReleaseForward))
		{
			if (m_ChunkManager->IsTileFree(m_CurrentChunkI+1,m_TargetPosX))
			{
				++m_CurrentChunkI;
				if (m_CurrentChunkI - m_AllowedBackTrack > m_CurrentLastChunk)
				{
					++m_CurrentLastChunk;
				}
				m_ChunkManager->PlayerMove(m_CurrentChunkI);
				++m_TargetPosZ;

				jumped = true;
			}
			SetTargetRot(180.f);
		}
		else if (sceneContext.pInput->IsActionTriggered(ReleaseBackward))
		{
			if (m_ChunkManager->IsTileFree(m_CurrentChunkI - 1, m_TargetPosX))
			{
				if (m_CurrentChunkI - 1 > m_CurrentLastChunk)
				{
					--m_CurrentChunkI;
					--m_TargetPosZ;
					jumped = true;
				}
			}
			SetTargetRot(0.f);

		}
		else if (sceneContext.pInput->IsActionTriggered(ReleaseLeft))
		{
			if (abs(m_TargetPosX - 1) <= m_GameWidth && m_ChunkManager->IsTileFree(m_CurrentChunkI, m_TargetPosX-1))
			{
				--m_TargetPosX;

				jumped = true;
			}
			SetTargetRot(90.f);
		}
		else if (sceneContext.pInput->IsActionTriggered(ReleaseRight))
		{
			if (abs(m_TargetPosX + 1) <= m_GameWidth && m_ChunkManager->IsTileFree(m_CurrentChunkI, m_TargetPosX + 1))
			{
				++m_TargetPosX;
				jumped = true;
			}
			SetTargetRot(270.f);
		}



		if (jumped)
		{
			m_JumpTimer = m_JumpTime;
			SoundManager::Get()->GetSystem()->playSound(m_pJumpSound, nullptr, false, &m_pChannelJump);
			m_pChannelJump->setVolume(0.5f);
		}
	}

	if (m_JumpTimer >= 0.f)
	{
		m_JumpTimer -= sceneContext.pGameTime->GetElapsed();
		MathHelper::Clamp(m_JumpTimer, m_JumpTime, 0.f);
	}
	if (m_KeyPressed)
	{
		m_SquishFactor = std::lerp(m_SquishFactor, 1.f, 0.2f);
	}
	else
	{
		m_SquishFactor = std::lerp(m_SquishFactor, 0.f, 0.2f);
	}

	//lerp transform
	float jumpProgress = 1.f - (m_JumpTimer / m_JumpTime);
	if (jumpProgress < 0.5f)
	{
		//raise
		m_CurrentHeight = std::lerp(0.f, m_JumpHeight, jumpProgress * 2);
	}
	else
	{
		float targetHeight = 0.f;

		if (m_ChunkManager->DidChickenDie(m_CurrentChunkI, m_TargetPosX))
		{
			m_Dead = true;
			m_DeathParticle->SpawnOneShot();
			SoundManager::Get()->GetSystem()->playSound(m_pSplashSound, nullptr, false, &m_pChannelDie);
			m_pChannelDie->setVolume(0.7f);
		}
		m_CurrentHeight = std::lerp(m_JumpHeight, targetHeight, (jumpProgress - 0.5f) * 2);
	}
	MathHelper::Clamp(jumpProgress, 1.f, 0.f);
	float xPos = std::lerp(static_cast<float>(m_PrevX), static_cast<float>(m_TargetPosX), jumpProgress);
	float zPos = std::lerp(static_cast<float>(m_PrevZ), static_cast<float>(m_TargetPosZ), jumpProgress);
	GetTransform()->Translate(xPos, m_CurrentHeight, zPos);


	//lerp rotate
	m_RotTimer -= sceneContext.pGameTime->GetElapsed();
	float rotLerp{ 1 - (m_RotTimer / m_RotTime) };
	MathHelper::Clamp(rotLerp, 1.f, 0.f);

	float currentRotation = std::lerp(m_PrevRotY, m_TargetRotY, rotLerp);
	GetTransform()->Rotate(0.f, currentRotation, 0.f);

	//lerp squish
	GetTransform()->Scale(1.f, std::lerp(1.f, m_MaxSquishScale, m_SquishFactor), 1.f);

	if (m_Dead)
	{
		GetTransform()->Translate(xPos, -1.f, zPos);
	}
}


void PlayerChicken::SetTargetRot(float rot)
{
	m_PrevRotY = m_TargetRotY;
	m_TargetRotY = rot;

	if (fmod(m_PrevRotY, 360.f) < 0.f)
	{
		m_PrevRotY = fmod(m_PrevRotY, 360.f) + 360.f;
	}

	float delta = (m_TargetRotY - m_PrevRotY);

	if (abs(delta) > 180.f)
	{
		if (delta < 0.f)
		{
			m_PrevRotY -= 360.f;
		}
		else
		{
			m_PrevRotY += 360.f;
		}
	}

	m_RotTimer = m_RotTime;
}

void PlayerChicken::InitModel()
{
	ModelComponent* NewModel = nullptr;
	UINT matId;
	ModelAnimator* modelAnimator;

	switch (m_CurrentChar)
	{
	case Character::Chicken:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Chicken.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Chicken);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		break;
	case Character::Rambo:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Man/Rambo.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"Game");
		modelAnimator->Pause();

		break;
	case Character::Zombie:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Man/Zombie.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"Game");
		modelAnimator->Pause();
		break;
	case Character::SquidGuy:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Man/Squid.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"Game");
		modelAnimator->Pause();
		break;
	case Character::Demon:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Man/Demon.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"Game");
		modelAnimator->Pause();

		break;
	case Character::MuricaMan:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Man/MuricaMan.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::AnTest);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		modelAnimator = NewModel->GetAnimator();
		modelAnimator->SetAnimation(L"Game");
		modelAnimator->Pause();
		break;
	case Character::Cat:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Cat.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Cat);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));

		break;
	case Character::Doge:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Doge.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Doge);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));

		break;
	case Character::Penguin:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Pengy.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Penguin);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));

		break;
	case Character::Pig:
		NewModel =AddComponent(new ModelComponent(L"Meshes/Game/Pig.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Pig);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		break;
	case Character::Frog:
		NewModel = AddComponent(new ModelComponent(L"Meshes/Game/Frog.ovm"));
		matId = m_MaterialManager->GetMaterialId(GameMaterials::Frog);
		NewModel->SetMaterial(MaterialManager::Get()->GetMaterial(matId));
		break;
	case Character::count:
	default:
		std::cout << "SOMETHING WENT WRONG\n";
		break;
	}
}
