#pragma once

#include "ToolWindow.h"

class VisualToolWindow : public ToolWindow
{
public:
	VisualToolWindow();
	~VisualToolWindow();

	void init(std::vector<std::vector<BaseTool*>> toolGroups) override;
};
