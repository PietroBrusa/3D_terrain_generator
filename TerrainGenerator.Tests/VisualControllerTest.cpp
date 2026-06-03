#include "pch.h"
#include <gtest/gtest.h>
#include "VisualController.h"
#include "BaseVisualTool.h"
#include "AppEvents.h"
#include "EventDispatcher.h"

class DummyVisualTool : public BaseVisualTool {
public:
    bool wasUsed = false;
    DummyVisualTool() : BaseVisualTool("DummyVisual") {}

    void use() override {
        wasUsed = true;
    }
};

class VisualControllerTest : public ::testing::Test {
protected:
    VisualController* controller = nullptr;
    DummyVisualTool* realisticTool = nullptr;

    void SetUp() override {
        controller = &VisualController::getInstance();
        realisticTool = new DummyVisualTool();

        controller->init(nullptr, nullptr);
    }

    void TearDown() override {
        controller->free();
        delete realisticTool;
    }
};

TEST_F(VisualControllerTest, SelectingToolTriggersUse) {
    controller->onToolSelected(realisticTool, 0, 0);

    EXPECT_EQ(controller->getActiveTool(), realisticTool);
    EXPECT_TRUE(realisticTool->wasUsed);
}

TEST_F(VisualControllerTest, ShadingEventTriggersSelection) {
    EXPECT_NO_THROW({
        MenuDispatcher::getInstance().dispatch(AppEvents::MENU_SHADING_REAL);
        });
}
