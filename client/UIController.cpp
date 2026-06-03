#include "UIController.h"

#include <imgui.h>
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl3.h>

#include "engine.h"

UIController::UIController()
    : topMenuBar(nullptr),
    statusBar(nullptr)
{}

UIController& UIController::getInstance()
{
    static UIController instance;
    return instance;
}

void UIController::init(std::vector<BaseWindow*> windows, BaseWindow* topMenuBar, StatusBar* statusBar)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL3_Init("#version 440");

	this->windows = windows;
	this->topMenuBar = topMenuBar;
	this->statusBar = statusBar;

	setupTheme();
}

void UIController::free()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    if (this->topMenuBar)
    {
        delete this->topMenuBar;
        this->topMenuBar = nullptr;
    }

    if (this->statusBar)
    {
        delete this->statusBar;
        this->statusBar = nullptr;
	}

    for (auto window : this->windows)
    {
        if (window)
        {
            delete window;
        }
    }
	this->windows.clear();
}

void UIController::prepareFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();
}

void UIController::render()
{
    renderDockSpace();

    for (auto window : this->windows)
    {
        if (window && window->isOpen())
        {
            window->render();
        }
	}
}

void UIController::onResize(int width, int height)
{
	if (width <= 0 || height <= 0) return;

    if (this->topMenuBar)
    {
        this->topMenuBar->onResize(width, height);
	}

    if (this->statusBar)
    {
        this->statusBar->onResize(width, height);
    }

    for (auto window : this->windows)
    {
        if (window)
        {
            window->onResize(width, height);
        }
	}
}

void UIController::setupTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // --- BASE / BACKGROUND ---
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.08f, 1.00f); // #121315 Basalt Black
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.13f, 0.14f, 1.00f); // #1E2023 Graphite Panel
    colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.13f, 0.14f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.16f, 0.17f, 0.19f, 1.00f); // #2A2D31 Slate Divider
    colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    // --- TEXT ---
    colors[ImGuiCol_Text] = ImVec4(0.78f, 0.74f, 0.65f, 1.00f); // #C7BBA4 Sand Neutral
    colors[ImGuiCol_TextDisabled] = ImVec4(0.45f, 0.43f, 0.38f, 1.00f);

    // --- HEADERS / SELECTED / ACTIVE ---
    colors[ImGuiCol_Header] = ImVec4(0.44f, 0.56f, 0.42f, 0.55f); // Moss Accent
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.56f, 0.42f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.56f, 0.42f, 1.00f);

    // --- BUTTONS (Moss) ---
    colors[ImGuiCol_Button] = ImVec4(0.31f, 0.43f, 0.40f, 0.60f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.43f, 0.40f, 0.80f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.31f, 0.43f, 0.40f, 1.00f);

    // --- PRIMARY ACTION (Cyan Professionale) ---
    colors[ImGuiCol_CheckMark] = ImVec4(0.24f, 0.78f, 0.75f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.78f, 0.75f, 0.80f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.78f, 0.75f, 1.00f);

    // --- SLIDERS / DRAG (Clay Brown) ---
    colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.17f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.30f, 0.23f, 0.80f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.30f, 0.23f, 1.00f);

    // --- TABS ---
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.13f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.78f, 0.75f, 0.60f);
    colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.78f, 0.75f, 0.90f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.13f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.78f, 0.75f, 0.40f);

    // --- DOCKING ---
    colors[ImGuiCol_DockingPreview] = ImVec4(0.24f, 0.78f, 0.75f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);

    // --- SCROLLBAR ---
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.43f, 0.40f, 0.60f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.31f, 0.43f, 0.40f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.31f, 0.43f, 0.40f, 1.00f);

    // --- STYLE TWEAKS ---
    style.WindowRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
}

void UIController::renderDockSpace()
{
    if (this->topMenuBar)
    {
        this->topMenuBar->render();
	}

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGui::DockSpaceOverViewport(0U, viewport, dockspaceFlags);

    if (this->statusBar)
    {
        Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();
        int fps = Eng::Base::getInstance().getCurrentFPS();

        this->statusBar->updateStats(camera->getMatrix()[3], fps);
        this->statusBar->render();
	}
}
