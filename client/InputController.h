#pragma once

#include <glm/glm.hpp>

#include "EventDispatcher.h"

using MouseMoveDispatcher = EventDispatcher<int, int, int, int>;
using MouseWheelDispatcher = EventDispatcher<int, int, int>;

class InputController
{
public:
	static InputController& getInstance();
	~InputController() = default;

	void onMouseButton(int id, int state, int x, int y);
	void onMouseMove(int x, int y);
	void onMouseHover(int x, int y);
	void onMouseWeel(int id, int direction, int x, int y);

	bool isHittingTerrain(glm::vec3& nodePosition) const;

	glm::vec2 getMousePosition() const { return m_mousePosition; }
	bool isLeftMouseDown() const { return m_isLeftMouseDown; }
	bool isMiddleMouseDown() const { return m_isMiddleMouseDown; }
	bool isRightMouseDown() const { return m_isRightMouseDown; }

private:
	InputController();
	InputController(const InputController&) = delete;
	void operator=(const InputController&) = delete;

	glm::vec2 m_mousePosition;

	bool m_isLeftMouseDown;
	bool m_isMiddleMouseDown;
	bool m_isRightMouseDown;
};
