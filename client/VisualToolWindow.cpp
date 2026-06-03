#include "VisualToolWindow.h"

VisualToolWindow::VisualToolWindow()
	: ToolWindow("Visual Tool", AnchorSide::RIGHT, ImVec2(0, -125), true, false)
{}

VisualToolWindow::~VisualToolWindow()
{}

void VisualToolWindow::init(std::vector<std::vector<BaseTool*>> toolGroups)
{
	this->tools = toolGroups;
}
