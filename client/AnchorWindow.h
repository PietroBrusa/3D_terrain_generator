#pragma once

#include "BaseWindow.h"

enum class AnchorSide { LEFT, RIGHT, TOP, BOTTOM, CENTER };

class AnchorWindow : public BaseWindow
{
public:
	AnchorWindow(
		const std::string& name = "",
		AnchorSide side = AnchorSide::RIGHT, 
		ImVec2 offset = ImVec2(0, 0),
		bool startVisible = true,
		bool cancelable = false,
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration
	);

	AnchorSide getAnchorSide() const;

	ImVec2 getOffset() const;

	void setAnchor(AnchorSide newSide, ImVec2 newOffset = ImVec2(0, 0));

protected:
	void basePosition() override;

private:
	AnchorSide side;
	ImVec2 offset;
};
