#include "pch.h"
#include <gtest/gtest.h>
#include <imgui.h>

#include "PointerController.h"
#include "CursorTool.h"
#include "BaseBrushTool.h"
#include "engine.h"

#include "AppEvents.h"
#include "EventDispatcher.h"

#ifndef CURSOR_SENSITIVITY
#define CURSOR_SENSITIVITY 0.2f
#endif

class DummyBrushTool : public BaseBrushTool {
public:
    DummyBrushTool() : BaseBrushTool("DummyBrush") {}

    UpdateArea use(glm::vec3 pos, TerrainConfig tCfg, TextureConfig texCfg, float dt, std::vector<float>& img) override 
    {
        return UpdateArea{ 0, 0, 0, 0, false };
    }

    void applyBrushEffect(int x, int y, int pixelX, int pixelY, int pixelRadius, int resolution, float heightScale, float deltaTime, std::vector<float>& image, bool& modified) override 
    {}

	glm::vec3 getRadiusColor() const override  
    { 
        return glm::vec3(0.0f); 
    }
};

class PointerControllerTest : public ::testing::Test
{
protected:
    PointerController* pointer = nullptr;
    Eng::Camera* dummyCamera = nullptr;
    CursorTool* cursorTool = nullptr;
    DummyBrushTool* brushTool = nullptr;

    void SetUp() override
    {
        ImGui::CreateContext();

        pointer = &PointerController::getInstance();
        cursorTool = new CursorTool(CURSOR_SENSITIVITY);
        brushTool = new DummyBrushTool();

        dummyCamera = new Eng::Camera("testCamera", glm::mat4(1.0f));
        Eng::Base::getInstance().setActiveCamera(dummyCamera);

        pointer->init(nullptr, nullptr);
    }

    void TearDown() override
    {
        pointer->free();
        Eng::Base::getInstance().setActiveCamera(nullptr);

        delete dummyCamera;
        delete cursorTool;
        delete brushTool;

        ImGui::DestroyContext();
    }
};

TEST_F(PointerControllerTest, InitialToolIsNull)
{
    EXPECT_EQ(pointer->getActiveTool(), nullptr);
}

TEST_F(PointerControllerTest, OnToolSelectedUpdatesActiveTool)
{
    pointer->onToolSelected(brushTool, 0, 0);

    EXPECT_EQ(pointer->getActiveTool(), brushTool);
}

TEST_F(PointerControllerTest, CursorMoveDelegatesToCameraWhenCursorToolActive)
{
    dummyCamera->setMatrix(glm::mat4(1.0f));
    pointer->onToolSelected(cursorTool, 0, 0);

    MouseMoveDispatcher::getInstance().dispatch(AppEvents::LEFT_MOUSE_MOVE, 10, 10, 0, 0);

    glm::mat4 newMat = dummyCamera->getMatrix();
    EXPECT_NE(newMat[0].x, 1.0f);
}

TEST_F(PointerControllerTest, CursorMoveDoesNotRotateCameraWhenBrushActive)
{
    dummyCamera->setMatrix(glm::mat4(1.0f));

    pointer->onToolSelected(brushTool, 0, 0);

    MouseMoveDispatcher::getInstance().dispatch(AppEvents::LEFT_MOUSE_MOVE, 10, 10, 0, 0);

    glm::mat4 newMat = dummyCamera->getMatrix();
    EXPECT_FLOAT_EQ(newMat[0].x, 1.0f);
    EXPECT_FLOAT_EQ(newMat[1].y, 1.0f);
}
