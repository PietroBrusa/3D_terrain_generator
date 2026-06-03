#pragma once

#include <cstdint>

struct TerrainConfig
{
	int size = 512;
	float heightScale = 100.0f;
	float vertexSpacing = 1.0f;

	bool isValid() const
	{
		return size > 0 && heightScale >= 0 && vertexSpacing >= 0;
	}
};
