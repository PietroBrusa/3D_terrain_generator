#include "SetupWindow.h"

#include <imgui.h>
#include <random>

#include "configuration.h"

SetupWindow::SetupWindow()
	: CentredWindow("Texture Setup", false),
	m_listener(nullptr)
{
	init();
}

void SetupWindow::setListener(ISetupListener* listener)
{
	m_listener = listener;
}

void SetupWindow::drawContent()
{
    // ==========================================
    // 1. WORLD PARAMETERS (Master Variables)
    // ==========================================

    ImGui::TextDisabled("WORLD PARAMETERS");
    ImGui::Separator();
    ImGui::Spacing();

    const char* worldDimensions[] = { "256", "512", "1024", "2048", "4096", "8192" };
    int current_res_idx = 0;
    for (int i = 0; i < 6; ++i) {
        if (m_terrainConfig.size == (1 << (8 + i))) current_res_idx = i;
    }

    if (ImGui::Combo("World Size (m)", &current_res_idx, worldDimensions, IM_ARRAYSIZE(worldDimensions))) {
        m_terrainConfig.size = 1 << (8 + current_res_idx);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Total physical dimension of the terrain in 3D space.");

    const char* spacingList[] = {
        "1.0m (Ultra HD)",
        "2.0m (High Detail)",
        "4.0m (Balanced)",
        "8.0m (Low Poly)",
        "16.0m (Retro/Blocky)"
    };

    int current_spacing_idx = 0;
    if (m_terrainConfig.vertexSpacing == 2.0f) current_spacing_idx = 1;
    else if (m_terrainConfig.vertexSpacing == 4.0f) current_spacing_idx = 2;
    else if (m_terrainConfig.vertexSpacing == 8.0f) current_spacing_idx = 3;
    else if (m_terrainConfig.vertexSpacing == 16.0f) current_spacing_idx = 4;

    if (ImGui::Combo("Style (Quad Size)", &current_spacing_idx, spacingList, IM_ARRAYSIZE(spacingList))) {
        m_terrainConfig.vertexSpacing = static_cast<float>(1 << current_spacing_idx);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Distance between vertices. Forces powers of 2 for perfect LOD scaling.");

    ImGui::DragFloat("Height Scale", &m_terrainConfig.heightScale, 1.0f, 0.1f, 5000.0f, "%.1f");
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Vertical multiplier for the terrain height.");

    ImGui::Spacing();
    ImGui::Spacing();

    // ==========================================
    // 2. HEIGHTMAP & NOISE
    // ==========================================

    ImGui::TextDisabled("NOISE PARAMETERS");
    ImGui::Separator();
    ImGui::Spacing();

    const char* densities[] = {
        "0.25x (Very Low)",
        "0.5x (Low)",
        "1.0x (1 Pixel / Meter)",
        "2.0x (High)",
        "4.0x (Ultra HD)"
    };
    float multipliers[] = { 0.25f, 0.5f, 1.0f, 2.0f, 4.0f };

    float currentRatio = static_cast<float>(m_textureConfig.size) / static_cast<float>(m_terrainConfig.size);
    int current_density_idx = 2;
    if (currentRatio <= 0.25f) current_density_idx = 0;
    else if (currentRatio <= 0.5f) current_density_idx = 1;
    else if (currentRatio == 1.0f) current_density_idx = 2;
    else if (currentRatio <= 2.0f) current_density_idx = 3;
    else current_density_idx = 4;

    if (ImGui::Combo("Map Detail", &current_density_idx, densities, IM_ARRAYSIZE(densities))) {
        m_textureConfig.size = static_cast<int>(m_terrainConfig.size * multipliers[current_density_idx]);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Heightmap pixel density relative to the physical World Size.");

    m_textureConfig.size = static_cast<int>(m_terrainConfig.size * multipliers[current_density_idx]);

    ImVec4 dimColor = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
    ImGui::TextColored(dimColor, "Generated Texture Size: %d x %d px", m_textureConfig.size, m_textureConfig.size);
    ImGui::Spacing();

    ImGui::SliderFloat("Frequency", &m_textureConfig.frequency, 0.1f, 20.0f, "%.2f");
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Increase this value to create tighter, more dense hills.");

    ImGui::SliderInt("Octaves", &m_textureConfig.octaves, 1, 10);
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Adds noise detail. Higher values result in more jagged rocks.");

    ImGui::PushItemWidth(ImGui::CalcItemWidth() - 60.0f);
    ImGui::InputScalar("Seed", ImGuiDataType_U32, &m_textureConfig.seed);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button("RND")) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);
        m_textureConfig.seed = dis(gen);
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Generate a new random seed.");

    ImGui::Spacing();
    ImGui::Spacing();

    // ==========================================
    // 3. ACTION
    // ==========================================

    if (ImGui::Button("Generate New Terrain", ImVec2(-1, 40))) {
        if (m_listener) {
            m_listener->onTerrainGenerationRequest(m_terrainConfig, m_textureConfig);
        }
    }
}

void SetupWindow::init()
{
	m_textureConfig.size = TERRAIN_BASE_SIZE;
	m_textureConfig.frequency = TERRAIN_BASE_FREQUENCY;
	m_textureConfig.octaves = TERRAIN_BASE_OCTAVES;
	m_textureConfig.seed = TERRAIN_BASE_SEED;

	m_terrainConfig.size = TERRAIN_BASE_SIZE;
	m_terrainConfig.heightScale = TERRAIN_BASE_HEIGHTSCALE;
    m_terrainConfig.vertexSpacing = TERRAIN_BASE_SPACING;
}
