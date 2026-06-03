#pragma once

#include "BaseBrushTool.h"

class IToolSettingsWindow
{
public:
	~IToolSettingsWindow() = default;

	virtual void setVisible(bool visible) = 0;
	virtual void setActiveTool(BaseBrushTool* tool) = 0;
};
