#include "TopMenuBar.h"

#include <imgui.h>

#include "AppEvents.h"

TopMenuBar::TopMenuBar()
	: BaseWindow("TopMenu", true, false),
    showAxes(true),
    showGrid(true),
    currentShadingMode(0)
{}

void TopMenuBar::render()
{
	if (!isVisible) return;

    if (ImGui::BeginMainMenuBar())
    {
        // ==========================================
        // MENU: FILE
        // ==========================================
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_NEW_PROJECT);

            if (ImGui::MenuItem("Open Heightmap...", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_OPEN_HEIGHTMAP);

            ImGui::Separator();

            if (ImGui::MenuItem("Save", "Ctrl+S", false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SAVE_PROJECT);

            if (ImGui::MenuItem("Save as...", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SAVE_AS);

            ImGui::Separator();

            if (ImGui::MenuItem("Export 3D Terrain (.obj)"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_EXPORT_MESH);

            if (ImGui::MenuItem("Export Heightmap (.exr)", "Ctrl+E"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_EXPORT_HEIGHTMAP);

            ImGui::Separator();

            if (ImGui::MenuItem("Quit", "Alt+F4"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::APP_EXIT);

            ImGui::EndMenu();
        }

        // ==========================================
        // MENU: EDIT
        // ==========================================
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_UNDO);

            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_REDO);

            ImGui::Separator();

            if (ImGui::MenuItem("Editor Settings...", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_OPTIONS);

            ImGui::EndMenu();
        }

        // ==========================================
        // MENU: VIEW
        // ==========================================
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Shading Mode"))
            {
                if (ImGui::MenuItem("Realisitc", nullptr, currentShadingMode == 0))
                    EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SHADING_REAL);

                if (ImGui::MenuItem("Solid", nullptr, currentShadingMode == 1))
                    EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SHADING_SOLID);

                if (ImGui::MenuItem("Wireframe", nullptr, currentShadingMode == 2))
                    EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SHADING_WIREFRAME);

                if (ImGui::MenuItem("Colors Altitude", nullptr, currentShadingMode == 3))
                    EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_SHADING_ALTITUDE);

                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Show Grid", nullptr, &showGrid, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_TOGGLE_GRID);

            if (ImGui::MenuItem("Show Axsis XYZ", nullptr, &showAxes, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_TOGGLE_AXES);

            ImGui::Separator();

            if (ImGui::MenuItem("Camera Reset"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_RESET_CAMERA);

            if (ImGui::MenuItem("Layout UI Reset", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_RESET_LAYOUT);

            ImGui::EndMenu();
        }

        // ==========================================
        // MENU: TERRAIN
        // ==========================================
        if (ImGui::BeginMenu("Terrain"))
        {
            if (ImGui::MenuItem("Resize Terrain...", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_RESIZE_TERRAIN);

            if (ImGui::MenuItem("Generates from Noise...", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_GENERATE_NOISE);

            ImGui::Separator();

            if (ImGui::MenuItem("Reset Altitudes", nullptr, false, false))
                EventDispatcher<>::getInstance().dispatch(AppEvents::MENU_FLATTEN_TERRAIN);

            ImGui::EndMenu();
        }

        // ==========================================
        // MENU: HELP
        // ==========================================
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("See documentation"))
                EventDispatcher<>::getInstance().dispatch(AppEvents::APP_DOCUMENTATION);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
