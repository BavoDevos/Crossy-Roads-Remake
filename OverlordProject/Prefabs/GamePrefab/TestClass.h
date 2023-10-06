//simpe prefab to keep track of commonly used materials

#include "Prefabs/GamePrefab/GameMaterialManager.h"

class TestClass final : public GameObject
{
public:
	TestClass(GameMaterialManager* matman);
	~TestClass() override = default;
	TestClass(const TestClass& other) = delete;
	TestClass(TestClass&& other) noexcept = delete;
	TestClass& operator=(const TestClass& other) = delete;
	TestClass& operator=(TestClass&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext&) override;

private:
	GameMaterialManager* m_MaterialManager = nullptr;
};