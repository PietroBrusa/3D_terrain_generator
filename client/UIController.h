#pragma once

#include <vector>

#include "StatusBar.h"

class UIController
{
public:
	static UIController& getInstance();
	~UIController() = default;

	void init(std::vector<BaseWindow*> windows, BaseWindow* topMenuBar = nullptr, StatusBar* statusBar = nullptr);
	void free();

	void prepareFrame();
	void render();
	void onResize(int width, int height);

private:
	UIController();

	UIController(const UIController&) = delete;
	UIController& operator=(const UIController&) = delete;

	std::vector<BaseWindow*> windows;
	BaseWindow* topMenuBar;
	StatusBar* statusBar;

	void setupTheme();

	void renderDockSpace();
};
