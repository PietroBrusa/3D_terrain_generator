#pragma once

#include "ToolWindow.h"

class PointerToolWindow : public ToolWindow
{
public:
	PointerToolWindow();
	~PointerToolWindow();

	void init(std::vector<std::vector<BaseTool*>> toolGroups) override;
};
