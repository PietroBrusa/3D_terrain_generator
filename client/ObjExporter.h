#pragma once

#include <string>
#include <filesystem>

#include <atomic>

#include "engine.h"

namespace fs = std::filesystem;

class ObjExporter
{
public:
    static void exportToObj(
        const std::string& fileName,
        const std::vector<Eng::Mesh*> chunks,
        const std::vector<float>& image,
        int size,
        float heightScale,
        int activeLod = 0,
        std::atomic<float>* progress = nullptr);
};
