
#pragma once
//Resharper Disable All

class PostOutline : public PostProcessingMaterial
{
public:
	PostOutline();
	~PostOutline() override = default;
	PostOutline(const PostOutline& other) = delete;
	PostOutline(PostOutline&& other) noexcept = delete;
	PostOutline& operator=(const PostOutline& other) = delete;
	PostOutline& operator=(PostOutline&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

