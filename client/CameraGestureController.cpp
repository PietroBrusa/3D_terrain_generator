#include "CameraGestureController.h"

#include <imgui.h>

#include "engine.h"

#include "configuration.h"
#include "AppEvents.h"

CameraGestureController& CameraGestureController::getInstance()
{
	static CameraGestureController instance;
	return instance;
}

CameraGestureController::~CameraGestureController()
{}

void CameraGestureController::init()
{
	panSubscriptionId = MouseMoveDispatcher::getInstance().subscribe(
		AppEvents::RIGHT_MOUSE_MOVE,
		[this](int x, int y, int lastX, int lastY)
		{
			this->cameraPan(x, y, lastX, lastY);
		}
	);

	rotateSubscriptionId = MouseMoveDispatcher::getInstance().subscribe(
		AppEvents::MIDDLE_MOUSE_MOVE,
		[this](int x, int y, int lastX, int lastY)
		{
			this->cameraRotate(x, y, lastX, lastY);
		}
	);

	zoomSubscriptionId = MouseWheelDispatcher::getInstance().subscribe(
		AppEvents::MOUSE_WHEEL,
		[this](int x, int y, int direction)
		{
			this->cameraZoom(x, y, direction);
		}
	);

	resetSubscriptionId = MenuDispatcher::getInstance().subscribe(
		AppEvents::MENU_RESET_CAMERA,
		[this]()
		{
			this->cameraReset();
		}
	);
}

void CameraGestureController::free() const
{
	MouseMoveDispatcher::getInstance().unsubscribe(AppEvents::RIGHT_MOUSE_MOVE, panSubscriptionId);
	MouseMoveDispatcher::getInstance().unsubscribe(AppEvents::MIDDLE_MOUSE_MOVE, rotateSubscriptionId);

	MouseWheelDispatcher::getInstance().unsubscribe(AppEvents::MOUSE_WHEEL, zoomSubscriptionId);
}

void CameraGestureController::cameraPan(int x, int y, int lastX, int lastY)
{
	float panSpeed = 0.5f;
	Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();

	float deltaX = (float)(x - lastX);
	float deltaY = (float)(y - lastY);

	glm::vec3 right = glm::normalize(glm::vec3(camera->getMatrix()[0]));
	glm::vec3 up = glm::normalize(glm::vec3(camera->getMatrix()[1]));

	glm::vec3 translation = (right * (-deltaX * panSpeed)) + (up * (deltaY * panSpeed));

	camera->setMatrix(glm::translate(glm::mat4(1.0f), translation) * camera->getMatrix());
}

void CameraGestureController::cameraRotate(int x, int y, int lastX, int lastY)
{
	Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();

	float deltaX = (float)(x - lastX);
	float deltaY = (float)(y - lastY);

	glm::mat4 matrix = camera->getMatrix();
	glm::vec3 localRight = glm::normalize(glm::vec3(matrix[0]));

	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(-deltaY * CURSOR_SENSITIVITY), localRight);
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(-deltaX * CURSOR_SENSITIVITY), glm::vec3(0.0f, 1.0f, 0.0f));

	camera->setMatrix(rotationY * rotationX * matrix);
}

void CameraGestureController::cameraZoom(int x, int y, int direction)
{
	bool isControlPressed = ImGui::GetIO().KeyCtrl;
	Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();

	float sensitivity = isControlPressed ? ZOOM_CTRL_SENSITIVITY : ZOOM_SENSITIVITY;
	float scrollDelta = (direction > 0) ? sensitivity : -sensitivity;

	glm::vec3 forward = -glm::vec3(camera->getMatrix()[2]);

	glm::vec3 translation = glm::normalize(forward) * scrollDelta;
	camera->setMatrix(glm::translate(glm::mat4(1.0f), translation) * camera->getMatrix());
}

void CameraGestureController::cameraReset()
{
	Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 96.0f, 512.0f));
	camera->setMatrix(translation);
}
