#pragma once

#include "BaseTool.h"
#include "TerrainConfig.h"
#include "TextureConfig.h"

struct UpdateArea
{
	int startX = 0;
	int startY = 0;
	int width = 0;
	int height = 0;
	bool isModified = false;
};

class BaseBrushTool : public BaseTool
{
public:
	BaseBrushTool(std::string name = "BrushTool", std::string iconResourcePath = "", unsigned int radius = 0, float strength = 0.0f, float falloff = 0.0f)
		: BaseTool(name, iconResourcePath),
		radius(radius),
		strength(strength),
		falloff(falloff)
	{}

	virtual void applyBrushEffect(
		int x, int y, 
		int pixelX, int pixelY, 
		int pixelRadius, 
		int resolution,
		float heightScale,
		float deltaTime,
		std::vector<float>& image,
		bool& modified
	) = 0;

	virtual UpdateArea use(
		glm::vec3 coords, 
		TerrainConfig terConfig, 
		TextureConfig texConfig, 
		float deltaTime, 
		std::vector<float>& image
	)
	{
		float terrainPhysicalSize = static_cast<float>(terConfig.size);
		float heightScale = terConfig.heightScale;
		int imageResolution = texConfig.size;

		float pixelsPerMeter = static_cast<float>(imageResolution) / terrainPhysicalSize;
		int pixelRadius = static_cast<int>(radius * pixelsPerMeter);

		float localX = coords.x + (terrainPhysicalSize / 2.0f);
		float localZ = coords.z + (terrainPhysicalSize / 2.0f);

		float u = localX / terrainPhysicalSize;
		float v = localZ / terrainPhysicalSize;

		int pixelX = static_cast<int>(u * imageResolution);
		int pixelY = static_cast<int>(v * imageResolution);

		if (pixelX < 0 || pixelX >= imageResolution || pixelY < 0 || pixelY >= imageResolution)
		{
			UpdateArea area;
			area.isModified = false;
			return area;
		}

		int startX = std::max(0, pixelX - (int)pixelRadius);
		int startY = std::max(0, pixelY - (int)pixelRadius);
		int endX = std::min(imageResolution - 1, pixelX + (int)pixelRadius);
		int endY = std::min(imageResolution - 1, pixelY + (int)pixelRadius);

		bool modified = false;

		for (int y = startY; y <= endY; ++y)
		{
			for (int x = startX; x <= endX; ++x)
			{
				applyBrushEffect(x, y, pixelX, pixelY, (int)pixelRadius, imageResolution, heightScale, deltaTime, image, modified);
			}
		}

		UpdateArea area;
		area.isModified = modified;

		if (modified)
		{
			area.startX = startX;
			area.startY = startY;
			area.width = endX - startX + 1;
			area.height = endY - startY + 1;
		}

		return area;
	}

	unsigned int getRadius() const { return radius; }
	float getStrength() const { return strength; }
	float getFalloff() const { return falloff; }
	virtual glm::vec3 getRadiusColor() const = 0;

	void setRadius(unsigned int r) { radius = r; }
	void setStrength(float s) { strength = s; }
	void setFalloff(float f) { falloff = f; }

protected:
	unsigned int radius;	// How large is the affected area.
	float strength;			// How quickly the ground rises or falls.
	float falloff;			// How soft the edge of the brush is (whether the effect is sharp in the center and fades out at the edges).
};
