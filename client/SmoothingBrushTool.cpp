#include "SmoothingBrushTool.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

SmoothingBrushTool::SmoothingBrushTool(unsigned int radius, float strength, float falloff)
	: BaseBrushTool("Smoothing", "./res/smooth.png", radius, strength, falloff)
{}

SmoothingBrushTool& SmoothingBrushTool::getInstance()
{
	static SmoothingBrushTool instance;
	return instance;
}

void SmoothingBrushTool::applyBrushEffect(
    int x, int y,
    int pixelX, int pixelY,
    int pixelRadius,
    int resolution,
    float heightScale,
    float deltaTime,
    std::vector<float>& image,
    bool& modified
)
{
    float distance = glm::distance(glm::vec2(x, y), glm::vec2(pixelX, pixelY));

    if (distance > static_cast<float>(pixelRadius)) {
        return;
    }

    float heightSum = 0.0f;
    int count = 0;

    for (int ny = -1; ny <= 1; ++ny) {
        for (int nx = -1; nx <= 1; ++nx) {
            int checkX = std::clamp(x + nx, 0, resolution - 1);
            int checkY = std::clamp(y + ny, 0, resolution - 1);
            heightSum += image[(checkY * resolution + checkX) * 3];
            count++;
        }
    }

    float average = heightSum / static_cast<float>(count);

    float t = distance / static_cast<float>(pixelRadius);
    float smooth = 1.0f - glm::smoothstep(0.0f, 1.0f, t);
    float influence = glm::mix(1.0f, smooth, this->falloff);

    float maxMoveAmount = (this->strength / heightScale) * deltaTime * influence;

    int index = (y * resolution + x) * 3;
    float currentHeightValue = image[index];

    float difference = average - currentHeightValue;

    float moveAmount = std::clamp(difference, -maxMoveAmount, maxMoveAmount);
    float smoothedVal = currentHeightValue + moveAmount;

    if (std::abs(smoothedVal - currentHeightValue) > 0.00001f) {
        modified = true;
    }

    image[index] = smoothedVal;
    image[index + 1] = smoothedVal;
    image[index + 2] = smoothedVal;
}

glm::vec3 SmoothingBrushTool::getRadiusColor() const
{
    return glm::vec3(0.490, 0.663, 0.780);
}
