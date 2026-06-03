#include "pch.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "AppController.h"

class AppControllerTest : public ::testing::Test
{
protected:
    AppController& app = AppController::getInstance();

    void SetUp() override
    {
        app.init(nullptr);
    }

    void TearDown() override
    {
        app.free();
    }
};

TEST_F(AppControllerTest, BackgroundTaskExecutesAndCompletes)
{
    bool isCallbackFired = false;

    app.runBackgroundTask(
        "Test Task",
        [](std::atomic<float>* progress)
        {
            *progress = 0.5f;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            *progress = 1.0f;
        },
        [&isCallbackFired]()
        {
            isCallbackFired = true;
        }
    );

    EXPECT_TRUE(app.isBackgroundTaskRunning());

    while (app.isBackgroundTaskRunning())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_FALSE(isCallbackFired);
    app.update();
    EXPECT_TRUE(isCallbackFired);
}
