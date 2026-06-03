#include "pch.h"
#include <gtest/gtest.h>
#include "BaseBrushTool.h"

class MockBrushTool : public BaseBrushTool {
public:
    int applyCalls = 0;

    MockBrushTool(unsigned int radius) : BaseBrushTool("Mock", "", radius, 1.0f, 0.0f) {}

    void applyBrushEffect(int x, int y, int pixelX, int pixelY, int pixelRadius, int resolution, float heightScale, float deltaTime, std::vector<float>& image, bool& modified) override
    {
        applyCalls++;
        modified = true;
    }
    glm::vec3 getRadiusColor() const override { return glm::vec3(0.0f); }
};

class BaseBrushToolTest : public ::testing::Test {
protected:
    TerrainConfig terCfg;
    TextureConfig texCfg;
    std::vector<float> dummyImage;

    void SetUp() override {
        terCfg.size = 100;
        terCfg.heightScale = 10.0f;
        texCfg.size = 100;
        dummyImage.assign(100 * 100 * 3, 0.0f);
    }
};

TEST_F(BaseBrushToolTest, BoundingBox_1to1_Scale) {
    terCfg.size = 100;
    texCfg.size = 100;

    MockBrushTool brush(10); 

    glm::vec3 clickPos(0.0f, 0.0f, 0.0f);

    UpdateArea area = brush.use(clickPos, terCfg, texCfg, 1.0f, dummyImage);

    EXPECT_TRUE(area.isModified);
    EXPECT_EQ(area.startX, 40);
    EXPECT_EQ(area.startY, 40);
    EXPECT_EQ(area.width, 21);
    EXPECT_EQ(area.height, 21);
    EXPECT_EQ(brush.applyCalls, 21 * 21);
}

TEST_F(BaseBrushToolTest, BoundingBox_LowRes_Scale) {
    terCfg.size = 100;
    texCfg.size = 10;

    MockBrushTool brush(10);

    glm::vec3 clickPos(-50.0f, 0.0f, -50.0f);

    UpdateArea area = brush.use(clickPos, terCfg, texCfg, 1.0f, dummyImage);

    EXPECT_TRUE(area.isModified);
    EXPECT_EQ(area.startX, 0);
    EXPECT_EQ(area.startY, 0);
    EXPECT_EQ(area.width, 2);
    EXPECT_EQ(area.height, 2);
    EXPECT_EQ(brush.applyCalls, 4);
}

TEST_F(BaseBrushToolTest, BoundingBox_HighRes_Scale) {
    terCfg.size = 50;
    texCfg.size = 200;

    std::vector<float> bigImage(200 * 200 * 3, 0.0f);

    MockBrushTool brush(10); 

    glm::vec3 clickPos(0.0f, 0.0f, 0.0f);

    UpdateArea area = brush.use(clickPos, terCfg, texCfg, 1.0f, bigImage);

    EXPECT_TRUE(area.isModified);
    EXPECT_EQ(area.startX, 60);
    EXPECT_EQ(area.startY, 60);
    EXPECT_EQ(area.width, 81);
    EXPECT_EQ(area.height, 81);
}

TEST_F(BaseBrushToolTest, UseIgnoresOutOfBoundsClick) {
    terCfg.size = 100;
    texCfg.size = 100;
    MockBrushTool brush(10);

    glm::vec3 clickPos(1000.0f, 0.0f, 1000.0f);

    UpdateArea area = brush.use(clickPos, terCfg, texCfg, 1.0f, dummyImage);

    EXPECT_FALSE(area.isModified);
    EXPECT_EQ(brush.applyCalls, 0);
}