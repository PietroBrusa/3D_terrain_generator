#include "ToolWindow.h"

ToolWindow::ToolWindow(const std::string& name, AnchorSide side, ImVec2 offset, bool startVisible, bool cancelable, ImGuiWindowFlags windowFlags)
	: AnchorWindow(name, side, offset, startVisible, cancelable, windowFlags),
	toolListener(nullptr)
{}

void ToolWindow::setListener(IToolListener* listener)
{
	this->toolListener = listener;
    setSelectedTool(0, 0);
}

void ToolWindow::setSelectedTool(int groupPos, int itemPos)
{
    if (!this->toolListener) return;
    if (groupPos < 0 || groupPos >= this->tools.size()) return;
    if (itemPos < 0 || itemPos >= this->tools[groupPos].size()) return;

    this->toolListener->onToolSelected(tools[groupPos][itemPos], groupPos, itemPos);
}

void ToolWindow::drawContent()
{
    ImVec2 iconSize(24, 24);

    for (int i = 0; i < tools.size(); i++)
    {
        const auto& group = tools[i];

        for (int j = 0; j < group.size(); j++)
        {
            if (j > 0)
            {
                ImGui::Dummy(ImVec2(0, 5));
            }

            renderToolButton(group[j], iconSize, i, j);
        }

        if (i < tools.size() - 1)
        {
            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 5));
        }
    }
}

void ToolWindow::clearTools()
{
    tools.clear();
}

void ToolWindow::renderToolButton(BaseTool* tool, ImVec2 iconSize, int groupPos, int itemPos)
{
    if (!tool) return;
	BaseTool* currentTool = toolListener ? toolListener->getActiveTool() : nullptr;
    bool isBrushActive = (currentTool == tool);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    if (isBrushActive) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    }
    else
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    }

    ImGui::PushID(tool->getName().c_str());

    ImVec4 tint = isBrushActive ? ImVec4(1.0f, 0.85f, 0.4f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImGui::ImageButton(
        "##btn", (ImTextureID)(intptr_t)tool->getIconId(),
        iconSize, ImVec2(0, 1), ImVec2(1, 0),
        ImVec4(0, 0, 0, 0), tint
    );

	if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", tool->getName().c_str());
    }
    if (toolListener && ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        toolListener->onToolSelected(tool, groupPos, itemPos);
    }
    if (toolListener && ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        toolListener->onToolEditor(tool, groupPos, itemPos);
    }

    ImGui::PopID();

    ImGui::PopStyleVar(2);
    if (isBrushActive) {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }
    else {
        ImGui::PopStyleVar();
    }
}
