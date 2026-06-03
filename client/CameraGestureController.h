#pragma once

#include "EventDispatcher.h"

using MouseMoveDispatcher = EventDispatcher<int, int, int, int>;
using MouseWheelDispatcher = EventDispatcher<int, int, int>;
using MenuDispatcher = EventDispatcher<>;

class CameraGestureController
{
public:
	static CameraGestureController& getInstance();

	~CameraGestureController();

	void init();
	void free() const;

	void cameraPan(int x, int y, int lastX, int lastY);
	void cameraRotate(int x, int y, int lastX, int lastY);
	void cameraZoom(int x, int y, int direction);
	void cameraReset();

private:
	CameraGestureController() = default;

	CameraGestureController(const CameraGestureController&) = delete;
	CameraGestureController& operator=(const CameraGestureController&) = delete;

	size_t panSubscriptionId;
	size_t rotateSubscriptionId;
	size_t zoomSubscriptionId;
	size_t resetSubscriptionId;
};
