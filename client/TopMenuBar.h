#pragma once

#include "BaseWindow.h"

#include "EventDispatcher.h"

using TopMenuDispatcher = EventDispatcher<>;

class TopMenuBar : public BaseWindow
{
public:
	TopMenuBar();

	void render() override;
	void drawContent() override {}

    void setGridVisible(bool visible) { showGrid = visible; }
    void setAxesVisible(bool visible) { showAxes = visible; }
    void setActiveShadingMode(int mode) { currentShadingMode = mode; }

private:
    bool showGrid;
    bool showAxes;
    int currentShadingMode;
};
