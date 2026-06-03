#pragma once

#include "ToolController.h"

#include "ToolWindow.h"
#include "EventDispatcher.h"

using MouseMoveDispatcher = EventDispatcher<int, int, int, int>;

class PointerController : public ToolController
{
public:
	static PointerController& getInstance();

	~PointerController();

	void init(ToolWindow* window, IToolSettingsWindow* editorWindow = nullptr) override;
	void update(float deltaTime);
	void free() const override;

	BaseTool* getActiveTool() const override;

	void onToolSelected(BaseTool* tool, int groupPos, int itemPos) override;
	void onToolEditor(BaseTool* tool, int groupPos, int itemPos) override;

private:
	PointerController();

	PointerController(const PointerController&) = delete;
	void operator=(const PointerController&) = delete;

	size_t mouseMoveSubscriptionId;

	int brushPositionUniformLoc;
	int brushRadiusUniformLoc;
	int brushActiveUniformLoc;
	int brushRadiusColorUniformLoc;

	void onCursorMove(int x, int y, int lastX, int lastY);

	void hideBrushArea();
	void showBrushArea(BaseBrushTool* brush, glm::vec3 mousePos);
};
