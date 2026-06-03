#include "BaseWindow.h"

#include <iostream>

BaseWindow::BaseWindow(const std::string& name, bool startVisible, bool cancelable, ImGuiWindowFlags windowFlags)
	: title(name),
    isVisible(startVisible),
    cancelable(cancelable),
    flags(windowFlags),
    defaultPos(-1.0f, -1.0f),
    defaultSize(-1.0f, -1.0f),
    posCond(ImGuiCond_FirstUseEver),
	forceReposition(true)
{}

void BaseWindow::setNextWindowPos(float x, float y, ImGuiCond cond)
{
    defaultPos = ImVec2(x, y);
    posCond = cond;
}

void BaseWindow::setNextWindowSize(float w, float h)
{
    defaultSize = ImVec2(w, h);
}

void BaseWindow::basePosition()
{
	std::cout << "BaseWindow::basePosition() called for " << title << std::endl;
    if (defaultPos.x != -1.0f) {
        ImGui::SetNextWindowPos(defaultPos, posCond);
    }

    if (defaultSize.x != -1.0f) {
        ImGui::SetNextWindowSize(defaultSize, ImGuiCond_FirstUseEver);
    }
}

void BaseWindow::render()
{
    if (!isVisible) return;

    if (forceReposition)
    {
        basePosition();
		forceReposition = false;
    }

    if (!ImGui::Begin(title.c_str(), cancelable ? &isVisible : nullptr, flags)) {
        ImGui::End();
        return;
    }

    drawContent();

    ImGui::End();
}

void BaseWindow::onResize(int width, int height)
{
    forceReposition = true;
}

bool BaseWindow::isOpen() const
{
    return isVisible;
}

void BaseWindow::setVisible(bool visible)
{
    isVisible = visible;
    forceReposition = true;
}

void BaseWindow::setFlags(ImGuiWindowFlags newFlags)
{
    flags = newFlags;
}
