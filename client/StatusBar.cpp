#include "StatusBar.h"

StatusBar::StatusBar()
	: AnchorWindow("StatusBar", AnchorSide::BOTTOM, ImVec2(0, 0), true, false),
    cameraPos(glm::vec3(0.0f)),
    fps(-1),
    currentMessage("Ready"),
    showProgress(false),
    progressValue(0.0f)
{
    flags -= ImGuiWindowFlags_AlwaysAutoResize;
	flags |= (ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoSavedSettings);
}

StatusBar::~StatusBar()
{}

void StatusBar::drawContent()
{
    // --- FIRST SECTION --- //
    ImGui::Text("%s", currentMessage.c_str());

    // --- SECOND SECTION --- //
    if (showProgress)
    {
        float progressBarWidth = 200.0f;
        float centerX = (ImGui::GetWindowWidth() - progressBarWidth) / 2.0f;

        ImGui::SameLine(centerX);
        ImGui::ProgressBar(progressValue, ImVec2(progressBarWidth, 0.0f), "");
    }

    // --- THIRD SECTION --- //
    char buffer[256];

    snprintf(
        buffer,
        sizeof(buffer),
        "Camera Pos: %.1f, %.1f, %.1f | FPS: %d",
        cameraPos.x, 
        cameraPos.y,
        cameraPos.z,
        fps
    );

    std::string statisticsText = buffer;
    float textWidth = ImGui::CalcTextSize(statisticsText.c_str()).x;

    ImGui::SameLine(ImGui::GetWindowWidth() - textWidth - 10.0f);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", statisticsText.c_str());
}

void StatusBar::updateStats(glm::vec3 cameraPos, int fps)
{
    this->cameraPos = cameraPos;
    this->fps = fps;
}

void StatusBar::setMessage(const std::string & msg)
{
    this->currentMessage = msg;
}

void StatusBar::setProgress(bool show, float value)
{
    this->showProgress = show;
    this->progressValue = value;
}

void StatusBar::basePosition()
{
    AnchorWindow::basePosition();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 30.0f), ImGuiCond_Always);
}
