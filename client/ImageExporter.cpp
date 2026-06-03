#include "ImageExporter.h"

#include <filesystem>
#include <iostream>

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

namespace fs = std::filesystem;

namespace terrain {

    bool ImageExporter::saveEXR(
        const std::vector<float>& image,
        const TextureConfig& config,
        std::string filePath
    ) {
        const char* err = nullptr;
        int ret = SaveEXR(image.data(), config.size, config.size, 3, 0, filePath.c_str(), &err);

        if (ret != TINYEXR_SUCCESS) {
            if (err) {
                std::cerr << "EXR Error: " << err << '\n';
                FreeEXRErrorMessage(err);
            }
            return false;
        }

        std::cout << "Image saved successfully to: " << filePath << "\n";
        return true;
    }

    std::string ImageExporter::getPerlinFileName(const TextureConfig config)
    {
        return "perlin_" + std::to_string(config.size) +
            "_f" + std::to_string(static_cast<int>(config.frequency)) +
            "_o" + std::to_string(config.octaves) +
            "_s" + std::to_string(config.seed) + ".exr";
    }

}