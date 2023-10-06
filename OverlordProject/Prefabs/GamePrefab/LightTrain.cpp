#include "stdafx.h"
#include "LightTrain.h"
#include "Prefabs/GamePrefab/GameMaterialManager.h"

LightTrain::LightTrain(GameMaterialManager* materialManager,float posx)
	:m_MaterialManager(materialManager),m_PosX{posx}
{
}

LightTrain::~LightTrain()
{
	m_pBellSound->release();
}

void LightTrain::SetLightOn()
{
	if (!m_LightOn)
	{
		m_LightOn = true;
		SoundManager::Get()->GetSystem()->playSound(m_pBellSound, nullptr, false, &m_pBellChannel);
		m_pBellChannel->setVolume(0.5f);
		m_pBellChannel->setLoopCount(-1);
	}
}

void LightTrain::SetLightOff()
{
	m_LightOn = false;
	m_TrainSign->SetMaterial(m_MaterialManager->GetMaterialId(GameMaterials::LightOff));
	m_AccuTime = 0;
	m_pBellChannel->stop();
}

void LightTrain::Initialize(const SceneContext& /*sceneContext*/)
{
	m_TrainSign = AddComponent(new ModelComponent(L"Meshes/Game/TrainLight.ovm"));
	m_TrainSign->SetMaterial(m_MaterialManager->GetMaterialId(GameMaterials::LightOff));
	XMFLOAT3 pos = GetTransform()->GetPosition();
	GetTransform()->Translate(m_PosX,pos.y, pos.z-0.6f);

	FMOD::System* pFmod = SoundManager::Get()->GetSystem();
	pFmod->createStream("Resources/SFX/Train_Alarm.wav", FMOD_DEFAULT, nullptr, &m_pBellSound);
}

void LightTrain::Update(const SceneContext& sceneContext)
{
	if (!m_LightOn)return;

	m_AccuTime += sceneContext.pGameTime->GetElapsed();

	if (m_AccuTime < m_LightSwitchTime)return;

	if (m_Light1)
	{
		m_TrainSign->SetMaterial(m_MaterialManager->GetMaterialId(GameMaterials::LightOn2));
		m_Light1 = false;
	}
	else
	{
		m_TrainSign->SetMaterial(m_MaterialManager->GetMaterialId(GameMaterials::LightOn1));
		m_Light1 = true;
	}

	m_AccuTime = 0;
}
