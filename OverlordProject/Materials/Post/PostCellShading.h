
#pragma once
//Resharper Disable All

class PostCellShading : public PostProcessingMaterial
{
public:
	PostCellShading();
	~PostCellShading() override = default;
	PostCellShading(const PostCellShading& other) = delete;
	PostCellShading(PostCellShading&& other) noexcept = delete;
	PostCellShading& operator=(const PostCellShading& other) = delete;
	PostCellShading& operator=(PostCellShading&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

