#pragma once

#include "ToolController.h"

#include "EventDispatcher.h"
#include "ToolWindow.h"
#include "TopMenuBar.h"

using MenuDispatcher = EventDispatcher<>;

class VisualController : public ToolController
{
public:
	static VisualController& getInstance();

	void init(ToolWindow* window, IToolSettingsWindow* editorWindow = nullptr) override;
	void setTopMenuSupport(TopMenuBar* topMenuBar);
	void free() const override;

	BaseTool* getActiveTool() const override;

	void onToolSelected(BaseTool* tool, int groupPos, int itemPos) override;
	void onToolEditor(BaseTool* tool, int groupPos, int itemPos) override {}

private:
	VisualController();

	VisualController(const VisualController&) = delete;
	VisualController& operator=(const VisualController&) = delete;

	ToolWindow* toolWindow;
	TopMenuBar* topMenuBar;

	size_t realisticSubscriptionId;
	size_t solidSubscriptionId;
	size_t wireframeSubscriptionId;
	size_t altitudeShadingSubscriptionId;

	void selectShadingMode(int position);
};
