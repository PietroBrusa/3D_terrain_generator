#include "pch.h"
#include <gtest/gtest.h>
#include "TextureGenerator.h"

class TextureGeneratorTest : public ::testing::Test {
protected:
    TextureConfig config;

    void SetUp() override {
        config.size = 64;
        config.frequency = 2.0f;
        config.octaves = 4;
        config.seed = 12345;
    }
};

TEST_F(TextureGeneratorTest, GenerateProducesCorrectImageSize) {
    TextureGenerator generator(config);

    auto image = generator.generate(nullptr);

    size_t expectedSize = config.size * config.size * 3;
    EXPECT_EQ(image.size(), expectedSize);
}

TEST_F(TextureGeneratorTest, GeneratedTextureIsGrayscale) {
    TextureGenerator generator(config);
    auto image = generator.generate(nullptr);

    ASSERT_FALSE(image.empty());

    std::vector<size_t> testIndices = { 0, (image.size() / 2) - 1, image.size() - 3 };

    for (size_t i : testIndices) {
        size_t pixelStart = (i / 3) * 3;

        float r = image[pixelStart];
        float g = image[pixelStart + 1];
        float b = image[pixelStart + 2];

        EXPECT_FLOAT_EQ(r, g);
        EXPECT_FLOAT_EQ(g, b);

        EXPECT_GE(r, 0.0f);
    }
}

TEST_F(TextureGeneratorTest, ProgressPointerIsUpdated) {
    TextureGenerator generator(config);
    std::atomic<float> progress(0.0f);

    generator.generate(&progress);

    EXPECT_FLOAT_EQ(progress.load(), 1.0f);
}
