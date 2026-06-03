#include "pch.h"
#include <gtest/gtest.h>
#include "ErosionBrushTool.h"

class ErosionBrushToolTest : public ::testing::Test {
protected:
    std::vector<float> image;
    int res = 10;
    float hScale = 10.0f;
    float dt = 1.0f;
    
	ErosionBrushTool& brush = ErosionBrushTool::getInstance();

    void SetUp() override {
        image.assign(res * res * 3, 0.5f);
    }
};

TEST_F(ErosionBrushToolTest, DecreasesHeight) {
    brush.setRadius(2);
    brush.setStrength(2.0f);
    brush.setFalloff(0.0f);

    (2, 2.0f, 0.0f);
    bool modified = false;

    brush.applyBrushEffect(5, 5, 5, 5, 2, res, hScale, dt, image, modified);

    int index = (5 * res + 5) * 3;
    EXPECT_TRUE(modified);
    EXPECT_LT(image[index], 0.5f);
}

TEST_F(ErosionBrushToolTest, RespectsMinimumClamp) {
    brush.setRadius(2);
    brush.setStrength(100.0f);
    brush.setFalloff(0.0f);

    bool modified = false;

    brush.applyBrushEffect(5, 5, 5, 5, 2, res, hScale, dt, image, modified);

    int index = (5 * res + 5) * 3;
    EXPECT_TRUE(modified);
    EXPECT_FLOAT_EQ(image[index], 0.0f);
}
