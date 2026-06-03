#include "pch.h"
#include <gtest/gtest.h>

#include "TerrainModel.h"
#include "engine.h"

class TerrainModelTest : public ::testing::Test {
protected:
    TerrainConfig* validTerrainCfg = nullptr;
    TextureConfig* validTextureCfg = nullptr;
    Eng::Camera* dummyCamera = nullptr;
    Eng::Node* dummyRootNode = nullptr;

    void SetUp() override {
        validTerrainCfg = new TerrainConfig();
        validTerrainCfg->size = 64;
        validTerrainCfg->vertexSpacing = 2.0f;
        validTerrainCfg->heightScale = 1.0f;

        validTextureCfg = new TextureConfig();
        validTextureCfg->size = 64;
        validTextureCfg->frequency = 1.0f;
        validTextureCfg->octaves = 1;
        validTextureCfg->seed = 123;

        dummyCamera = new Eng::Camera("TestCamera", glm::mat4(1.0f));
        Eng::Base::getInstance().setActiveCamera(dummyCamera);
    }

    void TearDown() override {
        Eng::Base::getInstance().setActiveCamera(nullptr);
        delete dummyCamera;
        delete validTerrainCfg;
        delete validTextureCfg;
    }
};

TEST_F(TerrainModelTest, GenerateTerrainReturnsNullOnInvalidConfig) {
    TerrainConfig invalidCfg;
    invalidCfg.size = -1;

    TerrainModel* model = TerrainModel::generateTerrain(invalidCfg, *validTextureCfg, nullptr);

    EXPECT_EQ(model, nullptr);
}

TEST_F(TerrainModelTest, GenerateTerrainCreatesChunksAndImage) {
    std::atomic<float> progress(0.0f);

    TerrainModel* model = TerrainModel::generateTerrain(*validTerrainCfg, *validTextureCfg, &progress);

    ASSERT_NE(model, nullptr);
    EXPECT_FALSE(model->getTerrainChunks().empty());
    EXPECT_FALSE(model->getTerrainImage().empty());

    EXPECT_FLOAT_EQ(progress.load(), 1.0f);

    delete model;
}

TEST_F(TerrainModelTest, UpdateCalculatesLODWithoutCrashing) {
    TerrainModel* model = TerrainModel::generateTerrain(*validTerrainCfg, *validTextureCfg, nullptr);
    ASSERT_NE(model, nullptr);

    glm::mat4 farMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(10000.0f, 90.0f, 10000.0f));
    dummyCamera->setMatrix(farMatrix);

    EXPECT_NO_THROW({
        model->update();
        });

    delete model;
}
