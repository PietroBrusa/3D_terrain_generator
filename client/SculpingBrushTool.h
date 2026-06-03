#pragma once

#include "BaseBrushTool.h"

class SculptingBrushTool : public BaseBrushTool
{
public:
	static SculptingBrushTool& getInstance();

	void applyBrushEffect(
		int x, int y, 
		int pixelX, int pixelY, 
		int pixelRadius, 
		int resolution, 
		float heightScale, 
		float deltaTime,
		std::vector<float>& image, 
		bool& modified
	) override;

	glm::vec3 getRadiusColor() const override;

private:
	SculptingBrushTool(unsigned int radius = 15, float strength = 5.0f, float falloff = 0.5f);

	SculptingBrushTool(const SculptingBrushTool&) = delete;
	void operator=(const SculptingBrushTool&) = delete;
};
