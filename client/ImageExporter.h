#pragma once
#include "TextureConfig.h"
#include <vector>
#include <string>

namespace terrain
{

    class ImageExporter
    {
    public:
        static bool saveEXR(const std::vector<float> &image, const TextureConfig &config, std::string filePath);

        static std::string getPerlinFileName(const TextureConfig config);
    };

}