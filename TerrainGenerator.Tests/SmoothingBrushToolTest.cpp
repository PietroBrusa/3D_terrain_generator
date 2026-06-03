#include "pch.h"
#include <gtest/gtest.h>
#include "SmoothingBrushTool.h"

class SmoothingBrushToolTest : public ::testing::Test {
protected:
    std::vector<float> image;
    int res = 10;
    float hScale = 10.0f;
    float dt = 1.0f;

	SmoothingBrushTool& brush = SmoothingBrushTool::getInstance();

    void SetUp() override {
        image.assign(res * res * 3, 0.5f);
    }
};

TEST_F(SmoothingBrushToolTest, AveragesHeightsAndFlattensSpikes) {
    brush.setRadius(2);
    brush.setStrength(10.0f);
    brush.setFalloff(0.0f);

    int centerIndex = (5 * res + 5) * 3;
    image[centerIndex] = 1.0f;
    image[centerIndex + 1] = 1.0f;
    image[centerIndex + 2] = 1.0f;

    bool modified = false;

    brush.applyBrushEffect(5, 5, 5, 5, 2, res, hScale, dt, image, modified);

    EXPECT_TRUE(modified);

    EXPECT_LT(image[centerIndex], 1.0f);
    EXPECT_GT(image[centerIndex], 0.5f);
}
