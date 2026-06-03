#pragma once
#include <cstdint>


struct TextureConfig
{
	int size = 512;
	float frequency = 4.0f;
	int octaves = 6.0f;
	uint32_t seed = 123456;

	bool isValid() const
	{
		return size > 0 && octaves > 0 && frequency > 0.0f;
	}
};
