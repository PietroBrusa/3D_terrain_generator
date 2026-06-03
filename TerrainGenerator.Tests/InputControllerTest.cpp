#include <gtest/gtest.h>
#include <imgui.h>
#include "InputController.h"

#ifndef ENGINE_MOUSE_BUTTON_LEFT
#define ENGINE_MOUSE_BUTTON_LEFT 0
#define ENGINE_MOUSE_BUTTON_MIDDLE 1
#define ENGINE_MOUSE_BUTTON_RIGHT 2
#define ENGINE_MOUSE_BUTTON_DOWN 0
#define ENGINE_MOUSE_BUTTON_UP 1
#endif

class InputControllerTest : public ::testing::Test
{
protected:
    InputController& input = InputController::getInstance();

    void SetUp() override
    {
        ImGui::CreateContext();

        input.onMouseButton(ENGINE_MOUSE_BUTTON_LEFT, ENGINE_MOUSE_BUTTON_UP, 0, 0);
        input.onMouseButton(ENGINE_MOUSE_BUTTON_MIDDLE, ENGINE_MOUSE_BUTTON_UP, 0, 0);
        input.onMouseButton(ENGINE_MOUSE_BUTTON_RIGHT, ENGINE_MOUSE_BUTTON_UP, 0, 0);
    }

    void TearDown() override
    {
        ImGui::DestroyContext();
    }
};

TEST_F(InputControllerTest, MousePositionUpdatesOnMove)
{
    input.onMouseMove(450, 300);

    glm::vec2 pos = input.getMousePosition();
    EXPECT_FLOAT_EQ(pos.x, 450.0f);
    EXPECT_FLOAT_EQ(pos.y, 300.0f);
}

TEST_F(InputControllerTest, MousePositionUpdatesOnHover)
{
    input.onMouseHover(120, 80);

    glm::vec2 pos = input.getMousePosition();
    EXPECT_FLOAT_EQ(pos.x, 120.0f);
    EXPECT_FLOAT_EQ(pos.y, 80.0f);
}

TEST_F(InputControllerTest, MouseDownIsBlockedWhenNoTerrainGenerated)
{
    input.onMouseButton(ENGINE_MOUSE_BUTTON_LEFT, ENGINE_MOUSE_BUTTON_DOWN, 100, 100);
    EXPECT_FALSE(input.isLeftMouseDown());
}

TEST_F(InputControllerTest, MouseDownIsBlockedByUI)
{
    ImGui::GetIO().WantCaptureMouse = true;

    input.onMouseButton(ENGINE_MOUSE_BUTTON_LEFT, ENGINE_MOUSE_BUTTON_DOWN, 200, 200);

    EXPECT_FALSE(input.isLeftMouseDown());

    glm::vec2 pos = input.getMousePosition();
    EXPECT_FLOAT_EQ(pos.x, 200.0f);
    EXPECT_FLOAT_EQ(pos.y, 200.0f);
}
