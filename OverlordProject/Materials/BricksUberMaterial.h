#pragma once
class BricksUberMaterial final : public Material<BricksUberMaterial>
{
public:
	BricksUberMaterial();
	~BricksUberMaterial() override = default;

	BricksUberMaterial(const BricksUberMaterial& other) = delete;
	BricksUberMaterial(BricksUberMaterial&& other) noexcept = delete;
	BricksUberMaterial& operator=(const BricksUberMaterial& other) = delete;
	BricksUberMaterial& operator=(BricksUberMaterial&& other) noexcept = delete;

protected:
	void InitializeEffectVariables() override;
};