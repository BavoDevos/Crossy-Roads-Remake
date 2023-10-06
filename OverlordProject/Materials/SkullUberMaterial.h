#pragma once
class SkullUberMaterial final : public Material<SkullUberMaterial>
{
public:
	SkullUberMaterial();
	~SkullUberMaterial() override = default;

	SkullUberMaterial(const SkullUberMaterial& other) = delete;
	SkullUberMaterial(SkullUberMaterial&& other) noexcept = delete;
	SkullUberMaterial& operator=(const SkullUberMaterial& other) = delete;
	SkullUberMaterial& operator=(SkullUberMaterial&& other) noexcept = delete;

protected:
	void InitializeEffectVariables() override;
};