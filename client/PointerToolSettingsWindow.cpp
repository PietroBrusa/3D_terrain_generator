#include "PointerToolSettingsWindow.h"

PointerToolSettingsWindow::PointerToolSettingsWindow()
	: CentredWindow("PointerTool Settings", false),
	IToolSettingsWindow(),
	currentTool(nullptr),
    radius(0),
    strength(0.0f),
    falloff(0.0f)
{}

PointerToolSettingsWindow::~PointerToolSettingsWindow()
{}

void PointerToolSettingsWindow::drawContent()
{
    if (!currentTool)
    {
        ImGui::TextDisabled("No tool selected.");
        return;
    }

    ImGui::TextDisabled("ACTIVE BRUSH");
    ImGui::Separator();

    ImVec4 accentColor = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
    ImGui::TextColored(accentColor, "%s", currentTool->getName().c_str());

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::SliderInt("Radius", &radius, 1, 200, "%d m"))
    {
        currentTool->setRadius(radius);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brush footprint size on the terrain.");

    if (ImGui::SliderFloat("Strength", &strength, 0.1f, 50.0f, "%.1f m/s"))
    {
        currentTool->setStrength(strength);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Maximum height added (or subtracted) at the center of the brush.");

    int falloffPercent = static_cast<int>(falloff * 100.0f);
    if (ImGui::SliderInt("Falloff", &falloffPercent, 0, 100, "%d%%"))
    {
        falloff = falloffPercent / 100.0f;
        currentTool->setFalloff(falloff);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("0%% = Hard edge (cylinder). 100%% = Maximum smoothness (soft hill).");

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Button("Close", ImVec2(-1.0f, 30.0f)))
    {
        this->setVisible(false);
    }
}

void PointerToolSettingsWindow::setVisible(bool visible)
{
	CentredWindow::setVisible(visible);
}

void PointerToolSettingsWindow::setActiveTool(BaseBrushTool * tool)
{
	this->currentTool = tool;

    this->radius = this->currentTool->getRadius();
    this->strength = this->currentTool->getStrength();
    this->falloff = this->currentTool->getFalloff();
}

void PointerToolSettingsWindow::saveAndDismiss()
{
	if (!this->currentTool) return;
    this->setVisible(false);
}
