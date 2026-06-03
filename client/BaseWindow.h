#pragma once

#include "imgui.h"
#include <string>

class BaseWindow {
public:
    BaseWindow(
        const std::string& name = "",
        bool startVisible = true,
        bool cancelable = true,
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None
    );

    virtual ~BaseWindow() = default;

    void setNextWindowPos(float x, float y, ImGuiCond cond = ImGuiCond_FirstUseEver);
    void setNextWindowSize(float w, float h);

    virtual void render();
    virtual void drawContent() = 0;

    virtual void onResize(int width, int height);

    bool isOpen() const;

    virtual void setVisible(bool visible);
    void setFlags(ImGuiWindowFlags newFlags);

protected:
    std::string title;

    bool isVisible;
    bool cancelable;

    ImGuiWindowFlags flags;
    ImVec2 defaultPos;
    ImVec2 defaultSize;
    ImGuiCond posCond;

    bool forceReposition;
    virtual void basePosition();
};
