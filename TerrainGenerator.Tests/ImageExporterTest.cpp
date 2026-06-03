#include "pch.h"
#include <gtest/gtest.h>

#include <filesystem>

#include "ImageExporter.h"

namespace fs = std::filesystem;

class ImageExporterTest : public ::testing::Test {
protected:
    fs::path tempFile;

    void SetUp() override {
        tempFile = fs::temp_directory_path() / "test_terrain_export.exr";
    }

    void TearDown() override {
        if (fs::exists(tempFile)) {
            fs::remove(tempFile);
        }
    }
};

TEST_F(ImageExporterTest, GetPerlinFileNameFormatsCorrectly) {
    TextureConfig config;
    config.size = 1024;
    config.frequency = 5.5f;
    config.octaves = 4;
    config.seed = 9999;

    std::string expected = "perlin_1024_f5_o4_s9999.exr";
    EXPECT_EQ(terrain::ImageExporter::getPerlinFileName(config), expected);
}

TEST_F(ImageExporterTest, SaveEXR_WritesToTempDirectory) {
    TextureConfig config;
    config.size = 2; 

    std::vector<float> dummyData(12, 1.0f);

    bool success = terrain::ImageExporter::saveEXR(dummyData, config, tempFile.string());

    EXPECT_TRUE(success);
    EXPECT_TRUE(fs::exists(tempFile));
    EXPECT_GT(fs::file_size(tempFile), 0); 
}
