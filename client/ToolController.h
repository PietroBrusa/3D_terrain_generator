#pragma once

#include "IToolListener.h"

#include "BaseTool.h"
#include "ToolWindow.h"
#include "IToolSettingsWindow.h"

class ToolController : public IToolListener
{
public:
	ToolController() 
		: currentTool(nullptr), 
		toolWindow(nullptr),
		editorToolWindow(nullptr)
	{}

	virtual void init(ToolWindow* window, IToolSettingsWindow* editorWindow = nullptr) = 0;
	virtual void free() const = 0;

protected:
	BaseTool* currentTool;

	ToolWindow* toolWindow;
	IToolSettingsWindow* editorToolWindow;
};
