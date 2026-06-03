#include "TerrainGenerator.h"

#include <vector>

#include <glm/glm.hpp>

#include "configuration.h"

TerrainGenerator::TerrainGenerator(const TerrainConfig& config)
	: m_config(config)
{
}

std::vector<Eng::Mesh*> TerrainGenerator::generate(int& gridDimX, int& gridDimZ, std::atomic<float>* progress) const
{
    int totalVertices = m_config.size / m_config.vertexSpacing;
    int actualChunkSize = std::min(totalVertices, (int)TERRAIN_CHUNK_SIZE);

	gridDimX = totalVertices / actualChunkSize;
	gridDimZ = totalVertices / actualChunkSize;

	std::vector<Eng::Mesh*> chunks;

    int totalChunks = gridDimX * gridDimZ;
    int completedChunks = 0;

	for (int z = 0; z < gridDimZ; z++)
	{
		for (int x = 0; x < gridDimX; x++)
		{
			int startX = x * actualChunkSize;
			int startZ = z * actualChunkSize;

            Eng::Mesh* chunk = generateChunk(startX, startZ, actualChunkSize, totalVertices);
            chunks.push_back(chunk);

            completedChunks++;

            if (progress)
            {
                progress->store(static_cast<float>(completedChunks) / static_cast<float>(totalChunks));
            }
		}
	}

	return chunks;
}

Eng::Mesh* TerrainGenerator::generateChunk(int startX, int startZ, int chunkSize, int globalSize) const
{
    std::vector<glm::vec3> vertexes;
    std::vector<std::vector<glm::uvec3>> lodFaces(TERRAIN_LODS_COUNT);
    std::vector<glm::vec2> textureCoordinates;

    int vertexCount = chunkSize + 1;
    float physicalGlobalSize = globalSize * m_config.vertexSpacing;

    for (int z = 0; z < vertexCount; ++z) {
        for (int x = 0; x < vertexCount; ++x) {
            float xPos = ((startX + x) * m_config.vertexSpacing) - (physicalGlobalSize / 2.0f);
            float zPos = ((startZ + z) * m_config.vertexSpacing) - (physicalGlobalSize / 2.0f);

            vertexes.push_back(glm::vec3(xPos, 0.0f, zPos));

            float u = (float)(startX + x) / globalSize;
            float v = (float)(startZ + z) / globalSize;
            textureCoordinates.push_back(glm::vec2(u, v));
        }
    }

    for (int lod = 0; lod < TERRAIN_LODS_COUNT; lod++) {
        int step = 1 << lod;
        if (step >= chunkSize) break;

        for (int z = 0; z < chunkSize; z += step) {
            for (int x = 0; x < chunkSize; x += step) {
                unsigned int topLeft = z * vertexCount + x;
                unsigned int topRight = z * vertexCount + (x + step);
                unsigned int bottomLeft = (z + step) * vertexCount + x;
                unsigned int bottomRight = (z + step) * vertexCount + (x + step);

                lodFaces[lod].push_back(glm::uvec3(topLeft, bottomLeft, topRight));
                lodFaces[lod].push_back(glm::uvec3(topRight, bottomLeft, bottomRight));
            }
        }
    }

    std::string meshName = "GridMesh_" + std::to_string(startX) + "_" + std::to_string(startZ);
    return new Eng::Mesh(meshName, glm::mat4(1.0f), vertexes, lodFaces, {}, textureCoordinates);
}
