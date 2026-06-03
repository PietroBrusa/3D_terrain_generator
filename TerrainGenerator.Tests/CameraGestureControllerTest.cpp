#include "pch.h"
#include <gtest/gtest.h>
#include <imgui.h>

#include "CameraGestureController.h"
#include "engine.h" 

#ifndef ZOOM_SENSITIVITY
#define ZOOM_SENSITIVITY 10.0f
#endif

class CameraGestureControllerTest : public ::testing::Test
{
protected:
    CameraGestureController& controller = CameraGestureController::getInstance();
    Eng::Camera* dummyCamera = nullptr;

    void SetUp() override
    {
        ImGui::CreateContext();

        dummyCamera = new Eng::Camera("testCamera", glm::mat4(1.0f));
        Eng::Base::getInstance().setActiveCamera(dummyCamera);
    }

    void TearDown() override
    {
        ImGui::DestroyContext();

        Eng::Base::getInstance().setActiveCamera(nullptr);
        delete dummyCamera;
    }
};

TEST_F(CameraGestureControllerTest, CameraResetSetsCorrectPosition)
{
    controller.cameraReset();

    glm::mat4 mat = dummyCamera->getMatrix();

    EXPECT_FLOAT_EQ(mat[3].x, 0.0f);
    EXPECT_FLOAT_EQ(mat[3].y, 96.0f);
    EXPECT_FLOAT_EQ(mat[3].z, 512.0f);
}

TEST_F(CameraGestureControllerTest, CameraZoomMovesForward)
{
    controller.cameraReset();
    glm::mat4 initialMat = dummyCamera->getMatrix();

    controller.cameraZoom(0, 0, 1);

    glm::mat4 newMat = dummyCamera->getMatrix();
    EXPECT_LT(newMat[3].z, initialMat[3].z);
    EXPECT_FLOAT_EQ(newMat[3].x, initialMat[3].x);
    EXPECT_FLOAT_EQ(newMat[3].y, initialMat[3].y);
}

TEST_F(CameraGestureControllerTest, CameraPanTranslatesCorrectly)
{
    dummyCamera->setMatrix(glm::mat4(1.0f));
    controller.cameraPan(10, 10, 0, 0);

    glm::mat4 mat = dummyCamera->getMatrix();

    EXPECT_FLOAT_EQ(mat[3].x, -5.0f);
    EXPECT_FLOAT_EQ(mat[3].y, 5.0f);
    EXPECT_FLOAT_EQ(mat[3].z, 0.0f);
}
