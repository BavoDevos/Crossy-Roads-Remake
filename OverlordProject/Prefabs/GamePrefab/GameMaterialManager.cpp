#include "stdafx.h"
#include "GameMaterialManager.h"

#include <stdexcept>


#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_skinned.h"
//exception for when material is not found 
class MaterialNotFoundException : public std::exception 
{
public:
    const char* what() const noexcept override 
    {
        return "Material not found.";
    }
};


GameMaterialManager::GameMaterialManager()
{
}

UINT GameMaterialManager::GetMaterialId(GameMaterials material) const
{
    auto it = m_GameMaterials.find(material);

    if (it != m_GameMaterials.end()) 
    {
        return it->second;
    }

    throw MaterialNotFoundException();
}

void GameMaterialManager::Initialize(const SceneContext& sceneContext)
{
#pragma region RoadChunkTextures

    // car purple
    auto Car1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Car1->SetDiffuseTexture(L"Textures/Game/CarPurple.png");
    Car1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::CarPurple] = Car1->GetMaterialId();

    // car Red
    auto Car2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Car2->SetDiffuseTexture(L"Textures/Game/CarRed.png");
    Car2->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::CarRed] = Car2->GetMaterialId();

    // Truck
    auto Truck1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Truck1->SetDiffuseTexture(L"Textures/Game/TruckRed.png");
    Truck1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::TruckRed] = Truck1->GetMaterialId();

    // road
    DiffuseMaterial_Shadow* Road = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Road->SetDiffuseTexture(L"Textures/Game/Road.png");
    Road->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Road] = Road->GetMaterialId();

#pragma endregion

#pragma region RiverChunkTextures

    // River
    DiffuseMaterial_Shadow* River = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    River->SetDiffuseTexture(L"Textures/Game/River.png");
    River->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::River] = River->GetMaterialId();

    // LilyPad
    DiffuseMaterial_Shadow* LilyPad = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    LilyPad->SetDiffuseTexture(L"Textures/Game/LilyPad.png");
    LilyPad->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::LilyPad] = LilyPad->GetMaterialId();

#pragma endregion

#pragma region RailChunkTextures

    // Rail
    DiffuseMaterial_Shadow* Rail = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Rail->SetDiffuseTexture(L"Textures/Game/Rail.png");
    Rail->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Rail] = Rail->GetMaterialId();

    //train doesnt need shadow as it passes fast and is almost not noticeable

    // TrainFront
    DiffuseMaterial* TrainFront = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    TrainFront->SetDiffuseTexture(L"Textures/Game/TrainF.png");
    TrainFront->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::TrainFront] = TrainFront->GetMaterialId();

    // TrainMiddle
    DiffuseMaterial* TrainMiddle = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    TrainMiddle->SetDiffuseTexture(L"Textures/Game/TrainM.png");
    TrainMiddle->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::TrainMiddle] = TrainMiddle->GetMaterialId();


    // light off
    DiffuseMaterial_Shadow* LightOff = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    LightOff->SetDiffuseTexture(L"Textures/Game/LightOff.png");
    LightOff->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::LightOff] = LightOff->GetMaterialId();

    // LightOn1
    DiffuseMaterial_Shadow* LightOn1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    LightOn1->SetDiffuseTexture(L"Textures/Game/LightOn1.png");
    LightOn1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::LightOn1] = LightOn1->GetMaterialId();

    // LightOn2
    DiffuseMaterial_Shadow* LightOn2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    LightOn2->SetDiffuseTexture(L"Textures/Game/LightOn2.png");
    LightOn2->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::LightOn2] = LightOn2->GetMaterialId();


#pragma endregion

#pragma region GrassChunkTextures

    // Grass1
    DiffuseMaterial_Shadow* Grass1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Grass1->SetDiffuseTexture(L"Textures/Game/Grass1.png");
    Grass1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Grass1] = Grass1->GetMaterialId();

    // Grass2
    DiffuseMaterial_Shadow* Grass2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Grass2->SetDiffuseTexture(L"Textures/Game/Grass2.png");
    Grass2->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Grass2] = Grass2->GetMaterialId();

    // Tree0
    DiffuseMaterial_Shadow* Tree0 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Tree0->SetDiffuseTexture(L"Textures/Game/Tree0.png");
    Tree0->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Tree0] = Tree0->GetMaterialId();

    // Tree1
    DiffuseMaterial_Shadow* Tree1 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Tree1->SetDiffuseTexture(L"Textures/Game/Tree1.png");
    Tree1->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Tree1] = Tree1->GetMaterialId();

    // Tree2
    DiffuseMaterial_Shadow* Tree2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Tree2->SetDiffuseTexture(L"Textures/Game/Tree2.png");
    Tree2->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Tree2] = Tree2->GetMaterialId();

    // Boulder
    DiffuseMaterial_Shadow* Boulder = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
    Boulder->SetDiffuseTexture(L"Textures/Game/boulder.png");
    Boulder->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Boulder] = Boulder->GetMaterialId();

#pragma endregion

#pragma region ChickenTexture

    //no shadow so the player always clearly sees the character

    // Chicken
    DiffuseMaterial* Chicken = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    Chicken->SetDiffuseTexture(L"Textures/Game/Chicken.png");
    Chicken->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Chicken] = Chicken->GetMaterialId();

    // Chicken
    DiffuseMaterial_Skinned* Chicken2 = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
    Chicken2->SetDiffuseTexture(L"Textures/Game/Man1.png");
    Chicken2->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::AnTest] = Chicken2->GetMaterialId();

#pragma endregion


    // Chicken
    ColorMaterial* BlackMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
    BlackMat->SetColor({ 0.1f,0.1f,0.1f,1.f });
    BlackMat->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::TunnelExit] = BlackMat->GetMaterialId();

    // Chicken
    DiffuseMaterial* OtherChars = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    OtherChars->SetDiffuseTexture(L"Textures/Game/Cat.png");
    OtherChars->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Cat] = OtherChars->GetMaterialId();

    OtherChars = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    OtherChars->SetDiffuseTexture(L"Textures/Game/Doge.png");
    OtherChars->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Doge] = OtherChars->GetMaterialId();

    OtherChars = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    OtherChars->SetDiffuseTexture(L"Textures/Game/Pengy.png");
    OtherChars->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Penguin] = OtherChars->GetMaterialId();


    OtherChars = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    OtherChars->SetDiffuseTexture(L"Textures/Game/Pig.png");
    OtherChars->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Pig] = OtherChars->GetMaterialId();

    OtherChars = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
    OtherChars->SetDiffuseTexture(L"Textures/Game/Frog.png");
    OtherChars->SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
    m_GameMaterials[GameMaterials::Frog] = OtherChars->GetMaterialId();

}
