#pragma once

#include "BaseWindow.h"

class CentredWindow : public BaseWindow
{
public:
	CentredWindow(
		const std::string& name = "",
		bool startVisible = true,
		bool cancelable = false,
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration
	);

protected:
	void basePosition() override;
};
