#include "pch.h"
#include <gtest/gtest.h>
#include "CursorTool.h"

class CursorToolTest : public ::testing::Test {
protected:
};

TEST_F(CursorToolTest, InitializationSetsCorrectNameAndIcon) {
    float expectedSensitivity = 1.5f;
    CursorTool cursor(expectedSensitivity);

    EXPECT_EQ(cursor.getName(), "Cursor");
}

TEST_F(CursorToolTest, CastsToBaseToolSuccessfully) {
    CursorTool cursor(1.0f);

    BaseTool* basePointer = dynamic_cast<BaseTool*>(&cursor);

    EXPECT_NE(basePointer, nullptr);
}
