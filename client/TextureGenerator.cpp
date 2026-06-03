#include "TextureGenerator.h"

#include <algorithm>
#include <cmath>

TextureGenerator::TextureGenerator(const TextureConfig& config)
	: m_config(config), m_perlin(config.seed)
{
}

std::vector<float> TextureGenerator::generate(std::atomic<float>* progress) const
{
	const int width = m_config.size;
	const int height = m_config.size;
	std::vector<float> image(width * height * 3);

	const double macroFrequency = m_config.frequency;
	const double microFrequency = m_config.frequency * 4.0;

	const double macroWeight = 0.85;
	const double microWeight = 0.15;

	int totalPixels = width * height;
	int completedPixels = 0;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			double nx = static_cast<double>(x) / width;
			double ny = static_cast<double>(y) / height;

			double macro01 = m_perlin.noise2D_01(nx * macroFrequency, ny * macroFrequency);
			double micro01 = m_perlin.normalizedOctave2D_01(nx * microFrequency, ny * microFrequency, m_config.octaves);

			double noiseValue = macro01 * macroWeight + micro01 * microWeight;

			noiseValue = std::pow(noiseValue, 1.5);

			int pixelIndex = (y * width + x) * 3;
			image[pixelIndex + 0] = static_cast<float>(noiseValue);
			image[pixelIndex + 1] = static_cast<float>(noiseValue);
			image[pixelIndex + 2] = static_cast<float>(noiseValue);

			completedPixels++;

			if (progress)
			{
				progress->store(static_cast<float>(completedPixels) / static_cast<float>(totalPixels));
			}
		}
	}

	return image;
}

