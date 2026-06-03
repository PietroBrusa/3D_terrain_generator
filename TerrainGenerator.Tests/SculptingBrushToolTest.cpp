#include "pch.h"
#include <gtest/gtest.h>
#include "SculpingBrushTool.h"

class SculptingBrushToolTest : public ::testing::Test {
protected:
    std::vector<float> image;
    int res = 10;
    float hScale = 10.0f;
    float dt = 1.0f;

    SculptingBrushTool& brush = SculptingBrushTool::getInstance();

    void SetUp() override {
        image.assign(res * res * 3, 0.5f);
    }
};

TEST_F(SculptingBrushToolTest, IncreasesHeight) {
    brush.setRadius(2);
    brush.setStrength(2.0f);
    brush.setFalloff(0.0f);

    bool modified = false;

    brush.applyBrushEffect(5, 5, 5, 5, 2, res, hScale, dt, image, modified);

    int index = (5 * res + 5) * 3;
    EXPECT_TRUE(modified);
    EXPECT_GT(image[index], 0.5f);
}

TEST_F(SculptingBrushToolTest, RespectsMaximumClamp) {
    brush.setRadius(2);
    brush.setStrength(100.0f);
    brush.setFalloff(0.0f);

    bool modified = false;

    brush.applyBrushEffect(5, 5, 5, 5, 2, res, hScale, dt, image, modified);

    int index = (5 * res + 5) * 3;
    EXPECT_TRUE(modified);
    EXPECT_FLOAT_EQ(image[index], 1.0f);
}

TEST_F(SculptingBrushToolTest, FalloffReducesStrengthAtEdges) {
	brush.setRadius(5);
	brush.setStrength(5.0f);
	brush.setFalloff(1.0f);

    bool mod1 = false, mod2 = false;

    brush.applyBrushEffect(5, 5, 5, 5, 5, res, hScale, dt, image, mod1);
    float centerHeight = image[(5 * res + 5) * 3];

    image[(9 * res + 5) * 3] = 0.5f;
    brush.applyBrushEffect(9, 5, 5, 5, 5, res, hScale, dt, image, mod2);
    float edgeHeight = image[(9 * res + 5) * 3];

    EXPECT_GT(centerHeight, edgeHeight);
}
