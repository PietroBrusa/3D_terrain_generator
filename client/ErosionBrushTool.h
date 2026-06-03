#pragma once

#include "BaseBrushTool.h"

class ErosionBrushTool : public BaseBrushTool
{
public:
	static ErosionBrushTool& getInstance();

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
	ErosionBrushTool(unsigned int radius = 8, float strength = 3.0f, float falloff = 0.25f);

	ErosionBrushTool(const ErosionBrushTool&) = delete;
	void operator=(const ErosionBrushTool&) = delete;
};
