#include "pch.h"
#include <gtest/gtest.h>
#include "TerrainGenerator.h"
#include "engine.h"

class TerrainGeneratorTest : public ::testing::Test {
protected:
    TerrainConfig config;

    void SetUp() override {
        config.size = 128;
        config.vertexSpacing = 2.0f;
        config.heightScale = 1.0f;
    }

    void TearDown() override {
    }
};

TEST_F(TerrainGeneratorTest, GenerateCalculatesCorrectGridAndReturnsMeshes) {
    TerrainGenerator generator(config);
    int gridX = 0;
    int gridZ = 0;

    std::vector<Eng::Mesh*> chunks = generator.generate(gridX, gridZ, nullptr);

    EXPECT_GT(gridX, 0);
    EXPECT_GT(gridZ, 0);
    EXPECT_EQ(chunks.size(), gridX * gridZ);

    for (auto* chunk : chunks) {
        delete chunk;
    }
}

TEST_F(TerrainGeneratorTest, ChunkContainsValidVerticesAndUVs) {
    TerrainGenerator generator(config);

    int chunkSize = 32;
    int globalSize = 64;
    Eng::Mesh* chunk = generator.generateChunk(0, 0, chunkSize, globalSize);

    ASSERT_NE(chunk, nullptr);

    auto vertices = chunk->getVertexes();
    auto uvs = chunk->getTextureCoordinates();

    int expectedVertices = (chunkSize + 1) * (chunkSize + 1);
    EXPECT_EQ(vertices.size(), expectedVertices);
    EXPECT_EQ(uvs.size(), expectedVertices);

    EXPECT_FLOAT_EQ(uvs.front().x, 0.0f);
    EXPECT_FLOAT_EQ(uvs.front().y, 0.0f);

    EXPECT_FLOAT_EQ(uvs.back().x, (float)(chunkSize) / globalSize);
    EXPECT_FLOAT_EQ(uvs.back().y, (float)(chunkSize) / globalSize);

    delete chunk;
}

TEST_F(TerrainGeneratorTest, ProgressIsUpdated) {
    TerrainGenerator generator(config);
    int gridX, gridZ;
    std::atomic<float> progress(0.0f);

    auto chunks = generator.generate(gridX, gridZ, &progress);

    EXPECT_FLOAT_EQ(progress.load(), 1.0f);

    for (auto* chunk : chunks) { delete chunk; }
}
