#include "pch.h"
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "ObjExporter.h"

#include "engine.h" 

namespace fs = std::filesystem;

class ObjExporterTest : public ::testing::Test {
protected:
    fs::path tempDir;
    fs::path tempFile;

    void SetUp() override {
        tempDir = fs::temp_directory_path() / "TerrainEditorMockDir";
        tempFile = tempDir / "test_mesh.obj";
    }

    void TearDown() override {
        if (fs::exists(tempDir)) {
            fs::remove_all(tempDir);
        }
    }
};

TEST_F(ObjExporterTest, ExportToObj_CreatesDirectoriesAndValidHeader) {
    int size = 2;
    float hScale = 1.0f;
    std::vector<float> dummyImage(size * size * 3, 0.0f);
    std::vector<Eng::Mesh*> emptyChunks;
    std::atomic<float> progress(0.0f);

    ASSERT_FALSE(fs::exists(tempDir));

    ObjExporter::exportToObj(tempFile.string(), emptyChunks, dummyImage, size, hScale, 0, &progress);

    ASSERT_TRUE(fs::exists(tempFile));

    std::ifstream file(tempFile);
    ASSERT_TRUE(file.is_open());

    std::string firstLine;
    std::getline(file, firstLine);
    EXPECT_EQ(firstLine, "# Exported Terrain Mesh");
}

TEST_F(ObjExporterTest, ExportToObj_SafelyHandlesNullProgressPointer) {
    int size = 2;
    std::vector<float> dummyImage(size * size * 3, 0.0f);
    std::vector<Eng::Mesh*> emptyChunks;

    EXPECT_NO_THROW({
        ObjExporter::exportToObj(tempFile.string(), emptyChunks, dummyImage, size, 1.0f, 0, nullptr);
        });
}