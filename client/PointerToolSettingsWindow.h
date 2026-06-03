#pragma once

#include "CentredWindow.h"
#include "IToolSettingsWindow.h"

#include "BaseBrushTool.h"

class PointerToolSettingsWindow : public IToolSettingsWindow, public CentredWindow
{
public:
	PointerToolSettingsWindow();
	~PointerToolSettingsWindow();

	void drawContent() override;
	
	void setVisible(bool visible) override;
	void setActiveTool(BaseBrushTool* tool) override;

private:
	BaseBrushTool* currentTool;
	int radius;
	float strength;
	float falloff;

	void saveAndDismiss();
};
