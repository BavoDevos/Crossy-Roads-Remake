#pragma once

enum class GameMaterials
{
	CarPurple,
	CarRed,
	TruckRed,
	Road,
	River,
	LilyPad,
	Rail,
	TrainFront,
	TrainMiddle,
	LightOff,
	LightOn1,
	LightOn2,
	Grass1,
	Grass2,
	Tree0,
	Tree1,
	Tree2,
	Boulder,
	Chicken,
	AnTest,
	TunnelExit,
	TunnelExit2,
	Cat,
	Doge,
	Penguin,
	Pig,
	Frog
};

enum InputIds
{
	PressForward,
	PressBackward,
	PressLeft,
	PressRight,
	ReleaseForward,
	ReleaseBackward,
	ReleaseLeft,
	ReleaseRight,
	Respawn
};

enum class Character
{
	Chicken,
	Rambo,
	Zombie,
	SquidGuy,
	Demon,
	MuricaMan,
	Cat,
	Doge,
	Penguin,
	Pig,
	Frog,
	count
};


//simpe prefab to keep track of commonly used materials
class GameMaterialManager final : public GameObject
{
public:
	GameMaterialManager();
	~GameMaterialManager() override = default;
	GameMaterialManager(const GameMaterialManager& other) = delete;
	GameMaterialManager(GameMaterialManager&& other) noexcept = delete;
	GameMaterialManager& operator=(const GameMaterialManager& other) = delete;
	GameMaterialManager& operator=(GameMaterialManager&& other) noexcept = delete;

	UINT GetMaterialId(GameMaterials material) const;

protected:
	void Initialize(const SceneContext&) override;

private:
	std::unordered_map<GameMaterials, UINT> m_GameMaterials;
};

