#include "pch.h"
#include <gtest/gtest.h>

#include "SetupController.h"
#include "AppEvents.h"
#include "EventDispatcher.h"

class SetupControllerTest : public ::testing::Test
{
protected:
    SetupController& setup = SetupController::getInstance();

    void SetUp() override
    {
        setup.init(nullptr);
    }

    void TearDown() override
    {
        setup.free();
    }
};

TEST_F(SetupControllerTest, InitSetsStateToNotGenerated)
{
    EXPECT_FALSE(setup.isTerrainGenerated());
    EXPECT_EQ(setup.getActiveTerrainModel(), nullptr);
}

TEST_F(SetupControllerTest, MenuNewProjectEventResetsState)
{
    setup.init(nullptr);
    MenuDispatcer::getInstance().dispatch(AppEvents::MENU_NEW_PROJECT);

    EXPECT_FALSE(setup.isTerrainGenerated());
}
