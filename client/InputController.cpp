#include "InputController.h"

#include <imgui.h>

#include "AppEvents.h"
#include "AppController.h"
#include "SetupController.h"

#include "engine.h"

InputController::InputController()
    : m_mousePosition(0.0f, 0.0f),
    m_isLeftMouseDown(false),
    m_isMiddleMouseDown(false),
    m_isRightMouseDown(false)
{}

InputController& InputController::getInstance()
{
    static InputController instance;
    return instance;
}

void InputController::onMouseButton(int id, int state, int x, int y)
{
    bool blockedByUI = ImGui::GetIO().WantCaptureMouse;
    bool blockedByTask = AppController::getInstance().isBackgroundTaskRunning();
    bool noTerrain = !SetupController::getInstance().isTerrainGenerated();

    if (blockedByUI || blockedByTask || noTerrain)
    {
        m_isLeftMouseDown = false;
        m_isMiddleMouseDown = false;
        m_isRightMouseDown = false;

        m_mousePosition = glm::vec2(x, y);
        return;
    }

    bool isDown = (state == ENGINE_MOUSE_BUTTON_DOWN);
    if (id == ENGINE_MOUSE_BUTTON_LEFT) m_isLeftMouseDown = isDown;
    if (id == ENGINE_MOUSE_BUTTON_MIDDLE) m_isMiddleMouseDown = isDown;
    if (id == ENGINE_MOUSE_BUTTON_RIGHT) m_isRightMouseDown = isDown;

    m_mousePosition = glm::vec2(x, y);
}

void InputController::onMouseMove(int x, int y)
{
    bool blockedByUI = ImGui::GetIO().WantCaptureMouse;
    bool blockedByTask = AppController::getInstance().isBackgroundTaskRunning();
    bool noTerrain = !SetupController::getInstance().isTerrainGenerated();

    if (blockedByUI || blockedByTask || noTerrain)
    {
        m_isLeftMouseDown = false;
        m_isMiddleMouseDown = false;
        m_isRightMouseDown = false;

        m_mousePosition = glm::vec2(x, y);
        return;
    }

    std::string eventType = AppEvents::MOUSE_MOVE;
    if (m_isLeftMouseDown) eventType = AppEvents::LEFT_MOUSE_MOVE;
    else if (m_isMiddleMouseDown) eventType = AppEvents::MIDDLE_MOUSE_MOVE;
    else if (m_isRightMouseDown) eventType = AppEvents::RIGHT_MOUSE_MOVE;

    MouseMoveDispatcher::getInstance().dispatch(eventType, x, y, m_mousePosition.x, m_mousePosition.y);

    m_mousePosition = glm::vec2(x, y);
}

void InputController::onMouseHover(int x, int y)
{
    bool blockedByUI = ImGui::GetIO().WantCaptureMouse;
    if (blockedByUI)
    {
        m_mousePosition = glm::vec2(x, y);
        return;
    }

    MouseMoveDispatcher::getInstance().dispatch(AppEvents::MOUSE_HOVER, x, y, m_mousePosition.x, m_mousePosition.y);

    m_mousePosition = glm::vec2(x, y);
}

void InputController::onMouseWeel(int id, int direction, int x, int y)
{
    bool blockedByUI = ImGui::GetIO().WantCaptureMouse;
    if (blockedByUI)
    {
        m_mousePosition = glm::vec2(x, y);
        return;
    }

    MouseWheelDispatcher::getInstance().dispatch(AppEvents::MOUSE_WHEEL, x, y, direction);
}

bool InputController::isHittingTerrain(glm::vec3& nodePosition) const
{
    return Eng::Base::getInstance().getClickedNode(m_mousePosition.x, m_mousePosition.y, nodePosition);
}


