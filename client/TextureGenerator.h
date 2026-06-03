#pragma once
#include "TextureConfig.h"
#include "PerlinNoise.hpp"
#include <vector>

class TextureGenerator
{
public:
	explicit TextureGenerator(const TextureConfig& config);

	std::vector<float> generate(std::atomic<float>* progress = nullptr) const;

private:
	TextureConfig m_config;
	siv::PerlinNoise m_perlin;
};

