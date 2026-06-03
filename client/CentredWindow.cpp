#include "CentredWindow.h"

CentredWindow::CentredWindow(const std::string& name, bool startVisible, bool cancelable, ImGuiWindowFlags windowFlags)
	: BaseWindow(name, startVisible, cancelable, windowFlags)
{}

void CentredWindow::basePosition()
{
	ImGui::SetNextWindowPos(defaultPos, posCond, ImVec2(0.5f, 0.5f));

	setNextWindowPos(
		ImGui::GetMainViewport()->GetCenter().x,
		ImGui::GetMainViewport()->GetCenter().y,
		ImGuiCond_Always
	);
}
