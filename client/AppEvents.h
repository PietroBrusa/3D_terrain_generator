#pragma once

#include <string>

namespace AppEvents
{
    // --- FILE ---
    inline const std::string MENU_NEW_PROJECT = "MENU_NEW_PROJECT";
    inline const std::string MENU_OPEN_HEIGHTMAP = "MENU_OPEN_HEIGHTMAP";
    inline const std::string MENU_SAVE_PROJECT = "MENU_SAVE_PROJECT";
    inline const std::string MENU_SAVE_AS = "MENU_SAVE_AS";
    inline const std::string MENU_EXPORT_MESH = "MENU_EXPORT_MESH";
    inline const std::string MENU_EXPORT_HEIGHTMAP = "MENU_EXPORT_HEIGHTMAP";
    inline const std::string APP_EXIT = "APP_EXIT";

    // --- EDIT ---
    inline const std::string MENU_UNDO = "MENU_UNDO";
    inline const std::string MENU_REDO = "MENU_REDO";
    inline const std::string MENU_OPTIONS = "MENU_OPTIONS";

    // --- VIEW ---
    inline const std::string MENU_SHADING_REAL = "MENU_SHADING_REAL";
    inline const std::string MENU_SHADING_WIREFRAME = "MENU_SHADING_WIREFRAME";
    inline const std::string MENU_SHADING_SOLID = "MENU_SHADING_SOLID";
    inline const std::string MENU_SHADING_ALTITUDE = "MENU_SHADING_ALTITUDE";
    inline const std::string MENU_TOGGLE_GRID = "MENU_TOGGLE_GRID";
    inline const std::string MENU_TOGGLE_AXES = "MENU_TOGGLE_AXES";
    inline const std::string MENU_RESET_CAMERA = "MENU_RESET_CAMERA";
    inline const std::string MENU_RESET_LAYOUT = "MENU_RESET_LAYOUT";

    // --- TERRAIN ---
    inline const std::string MENU_RESIZE_TERRAIN = "MENU_RESIZE_TERRAIN";
    inline const std::string MENU_GENERATE_NOISE = "MENU_GENERATE_NOISE";
    inline const std::string MENU_FLATTEN_TERRAIN = "MENU_FLATTEN_TERRAIN";

    // --- HELP ---
    inline const std::string APP_DOCUMENTATION = "APP_DOCUMENTATION";

    // --- GESTURE ---
    inline const std::string MOUSE_MOVE = "MOUSE_MOVE";
    inline const std::string LEFT_MOUSE_MOVE = "LEFT_MOUSE_MOVE";
    inline const std::string MIDDLE_MOUSE_MOVE = "MIDDLE_MOUSE_MOVE";
    inline const std::string RIGHT_MOUSE_MOVE = "RIGHT_MOUSE_MOVE";
    inline const std::string MOUSE_WHEEL = "MOUSE_WHEEL";
    inline const std::string MOUSE_HOVER = "MOUSE_HOVER";
}
