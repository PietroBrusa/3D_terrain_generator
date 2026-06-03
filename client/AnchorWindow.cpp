#include "AnchorWindow.h"

AnchorWindow::AnchorWindow(
	const std::string& name,
	AnchorSide side,
	ImVec2 offset,
	bool startVisible,
	bool cancelable,
	ImGuiWindowFlags windowFlags
)
	: BaseWindow(name, startVisible, cancelable, windowFlags),
	side(side),
	offset(offset)
{
	setFlags(windowFlags | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
}

void AnchorWindow::basePosition()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 targetPos;
	ImVec2 pivot;

	switch (side)
	{
	case AnchorSide::RIGHT:
		targetPos = ImVec2(
			viewport->WorkPos.x + viewport->WorkSize.x - offset.x,
			viewport->WorkPos.y + viewport->WorkSize.y / 2.0f + offset.y
		);
		pivot = ImVec2(1.0f, 0.5f);
		break;

	case AnchorSide::LEFT:
		targetPos = ImVec2(
			viewport->WorkPos.x + offset.x,
			viewport->WorkPos.y + viewport->WorkSize.y / 2.0f + offset.y
		);
		pivot = ImVec2(0.0f, 0.5f);
		break;

	case AnchorSide::BOTTOM:
		targetPos = ImVec2(
			viewport->WorkPos.x + viewport->WorkSize.x / 2.0f + offset.x,
			viewport->WorkPos.y + viewport->WorkSize.y - offset.y
		);
		pivot = ImVec2(0.5f, 1.0f);
		break;
	}

	ImGui::SetNextWindowPos(targetPos, ImGuiCond_Always, pivot);

	if (side == AnchorSide::RIGHT || side == AnchorSide::LEFT)
	{
		ImGui::SetNextWindowSize(ImVec2(250.0f, viewport->WorkSize.y * 0.8f), ImGuiCond_FirstUseEver);
	}
}

AnchorSide AnchorWindow::getAnchorSide() const
{
	return side;
}

ImVec2 AnchorWindow::getOffset() const
{
	return offset;
}

void AnchorWindow::setAnchor(AnchorSide newSide, ImVec2 newOffset)
{
	side = newSide;
	offset = newOffset;
}
