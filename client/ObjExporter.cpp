#include "ObjExporter.h"

#include <iostream>
#include <fstream>

void ObjExporter::exportToObj(const std::string& fileName, const std::vector<Eng::Mesh*> chunks, const std::vector<float>& image, int size, float heightScale, int activeLod, std::atomic<float>* progress)
{
    fs::path filePath(fileName);
    fs::path dir = filePath.parent_path();

    if (!dir.empty() && !fs::exists(dir)) {
        try {
            fs::create_directories(dir);
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << "\n";
            return;
        }
    }

    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open the file in write mode.\n";
        return;
    }

    size_t totalOps = 0;
    for (const auto* chunk : chunks) {
        if (!chunk) continue;
        totalOps += chunk->getVertexes().size() + chunk->getTextureCoordinates().size() * 2 + chunk->getFaces().size();
    }

    size_t completedOps = 0;

    auto updateProgress = [&]() {
        completedOps++;
        if (progress && (completedOps % 5000 == 0 || completedOps == totalOps)) {
            progress->store(static_cast<float>(completedOps) / static_cast<float>(totalOps));
        }
        };

    // --- START EXPORTING --- //
    file << "# Exported Terrain Mesh\n";

    auto getH = [&](float u, float v_coord)
        {
            int x = std::clamp(int(u * (size - 1)), 0, size - 1);
            int y = std::clamp(int(v_coord * (size - 1)), 0, size - 1);

            return image[(y * size + x) * 3] * heightScale;
        };

    file << "# Exported Terrain Mesh\n";

    unsigned int globalVertexOffset = 0;

    for (int c = 0; c < chunks.size(); c++)
    {
        Eng::Mesh* chunk = chunks[c];
        if (!chunk) continue;

        const auto& vertexes = chunk->getVertexes();
        const auto& texCoords = chunk->getTextureCoordinates();
        const auto& faces = chunk->getFacesForLOD(activeLod);

        file << "\no Chunk_" << c << "\n";

        // --- WRITING VERTEXES --- //
        for (size_t i = 0; i < vertexes.size(); ++i)
        {
            float h = getH(texCoords[i].x, texCoords[i].y);
            file << "v " << vertexes[i].x << " " << h << " " << vertexes[i].z << "\n";
            updateProgress();
        }

        // --- WRITING UV COORDINATES --- //
        for (const auto& vt : texCoords)
        {
            file << "vt " << vt.x << " " << (1.0f - vt.y) << "\n";
            updateProgress();
        }

        // --- WRITING NORMALS --- //
        float texelSize = 1.0f / (float)size;
        for (const auto& vt : texCoords)
        {
            float hL = getH(vt.x - texelSize, vt.y);
            float hR = getH(vt.x + texelSize, vt.y);
            float hD = getH(vt.x, vt.y - texelSize);
            float hU = getH(vt.x, vt.y + texelSize);

            float nx = hL - hR;
            float nz = hD - hU;
            float ny = 2.0f * texelSize * heightScale;

            float length = std::sqrt(nx * nx + ny * ny + nz * nz);
            file << "vn " << nx / length << " " << ny / length << " " << nz / length << "\n";

            updateProgress();
        }

        // --- WRITING FACES --- //
        for (const auto& face : faces)
        {
            unsigned int i1 = face.x + 1 + globalVertexOffset;
            unsigned int i2 = face.y + 1 + globalVertexOffset;
            unsigned int i3 = face.z + 1 + globalVertexOffset;

            file << "f " << i1 << "/" << i1 << "/" << i1 << " "
                << i2 << "/" << i2 << "/" << i2 << " "
                << i3 << "/" << i3 << "/" << i3 << "\n";

            updateProgress();
        }

        globalVertexOffset += vertexes.size();
    }

    file.close();
    std::cout << "OBJ file saved successfully to: " << fileName << "\n";
}
