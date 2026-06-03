#include <iostream>
#include <vector>

#include <imgui.h>
#include <backends/imgui_impl_glut.h>
#include <backends/imgui_impl_opengl3.h>

#include "engine.h"

#include "configuration.h"

// Models //
#include "CursorTool.h"

#include "ErosionBrushTool.h"
#include "SculpingBrushTool.h"
#include "SmoothingBrushTool.h"

#include "ShadesVisualTool.h"
#include "SolidVisualTool.h"
#include "WireframeVisualTool.h"
#include "AltitudeVisualTool.h"

// Views //
#include "TopMenuBar.h"
#include "StatusBar.h"

#include "PointerToolWindow.h"
#include "PointerToolSettingsWindow.h"
#include "VisualToolWindow.h"

#include "SetupWindow.h"

// Controllers //
#include "AppController.h"
#include "SetupController.h"

#include "CameraGestureController.h"
#include "InputController.h"

#include "PointerController.h"
#include "VisualController.h"

#include "UIController.h"

/////////////
// Globals //
/////////////

std::vector<BaseWindow*> windows;

///////////////
// Callbacks //
///////////////

void onResize(int w, int h) {
	ImGui_ImplGLUT_ReshapeFunc(w, h);
	UIController::getInstance().onResize(w, h);
}

static void renderingLoop(Eng::Node* root)
{
	UIController::getInstance().prepareFrame();
}

static void renderingImGui(Eng::GUIObjects obj)
{
	float dt = Eng::Base::getInstance().getDeltaTime();

	UIController::getInstance().render();
	AppController::getInstance().update();
	SetupController::getInstance().update();

	PointerController::getInstance().update(dt);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void addSpecialKeyEvents(ImGuiIO& io, int key, bool down)
{
	switch (key)
	{
	case ENGINE_KEY_F1:     io.AddKeyEvent(ImGuiKey_F1, true); break;
	case ENGINE_KEY_F2:     io.AddKeyEvent(ImGuiKey_F2, true); break;
	case ENGINE_KEY_LEFT:   io.AddKeyEvent(ImGuiKey_LeftArrow, true); break;
	case ENGINE_KEY_RIGHT:  io.AddKeyEvent(ImGuiKey_RightArrow, true); break;
	case ENGINE_KEY_UP:     io.AddKeyEvent(ImGuiKey_UpArrow, true); break;
	case ENGINE_KEY_DOWN:   io.AddKeyEvent(ImGuiKey_DownArrow, true); break;
	case ENGINE_KEY_PAGE_UP:   io.AddKeyEvent(ImGuiKey_PageUp, true); break;
	case ENGINE_KEY_PAGE_DOWN: io.AddKeyEvent(ImGuiKey_PageDown, true); break;
	case ENGINE_KEY_HOME:   io.AddKeyEvent(ImGuiKey_Home, true); break;
	case ENGINE_KEY_END:    io.AddKeyEvent(ImGuiKey_End, true); break;
	case ENGINE_KEY_INSERT: io.AddKeyEvent(ImGuiKey_Insert, true); break;
	case ENGINE_KEY_DELETE: io.AddKeyEvent(ImGuiKey_Delete, true); break;

	case ENGINE_KEY_CTRL_L: io.AddKeyEvent(ImGuiKey_LeftCtrl, true); break;
	case ENGINE_KEY_CTRL_R:  io.AddKeyEvent(ImGuiKey_RightCtrl, true); break;
	case ENGINE_KEY_SHIFT_L: io.AddKeyEvent(ImGuiKey_LeftShift, true); break;
	case ENGINE_KEY_SHIFT_R: io.AddKeyEvent(ImGuiKey_RightShift, true); break;
	case ENGINE_KEY_ALT_L: io.AddKeyEvent(ImGuiKey_LeftAlt, true); break;
	case ENGINE_KEY_ALT_R: io.AddKeyEvent(ImGuiKey_RightAlt, true); break;
	}
}

static void onSpecialKeyDownCallback(int key, int x, int y)
{
	if (AppController::getInstance().isBackgroundTaskRunning()) return;

	ImGuiIO& io = ImGui::GetIO();
	addSpecialKeyEvents(io, key, true);
}

static void onSpecialKeyUpCallback(int key, int x, int y)
{
	if (AppController::getInstance().isBackgroundTaskRunning()) return;

	ImGuiIO& io = ImGui::GetIO();
	addSpecialKeyEvents(io, key, false);
}

static void onKeyboardPressedCallback(unsigned char key, int mouseX, int mouseY)
{
	if (AppController::getInstance().isBackgroundTaskRunning()) return;
	ImGuiIO& io = ImGui::GetIO();

	if (key >= 32)
	{
		io.AddInputCharacter((unsigned int)key);
	}

	io.AddKeyEvent(ImGuiKey_Backspace, (key == 8));
	io.AddKeyEvent(ImGuiKey_Enter, (key == 13));
	io.AddKeyEvent(ImGuiKey_Escape, (key == 27));

	if (ImGui::GetIO().WantCaptureKeyboard)
	{
		return;
	}
}

static void onMouseCallback(int buttonId, int buttonState, int x, int y)
{
	ImGui_ImplGLUT_MouseFunc(buttonId, buttonState, x, y);
	InputController::getInstance().onMouseButton(buttonId, buttonState, x, y);
}

static void onMouseMotionCallback(int x, int y)
{
	ImGui_ImplGLUT_MotionFunc(x, y);
	InputController::getInstance().onMouseMove(x, y);
}

static void onPassiveMouseMotionCallback(int x, int y)
{
	ImGui_ImplGLUT_MotionFunc(x, y);
	InputController::getInstance().onMouseHover(x, y);
}

static void onMouseWheelCallback(int wheelId, int direction, int x, int y)
{
	ImGui_ImplGLUT_MouseWheelFunc(wheelId, direction, x, y);
	InputController::getInstance().onMouseWeel(wheelId, direction,x, y);
}

//////////
// Main //
//////////

int main(int argc, char* argv[])
{
	// Engine Initialization
	Eng::Base& eng = Eng::Base::getInstance();
	eng.init(&argc, argv, "3D Terrain Editor");

	Eng::Camera* mainCamera = nullptr;
	Eng::Shader* terrainShader = nullptr;
	Eng::InfiniteLight* sunLight = nullptr;

	// Shader Setup
	Eng::Shader* vShader = new Eng::Shader();
	Eng::Shader* fShader = new Eng::Shader();
	vShader->loadFromFile(Eng::Shader::TYPE_VERTEX, "./shaders/terrain.vert");
	fShader->loadFromFile(Eng::Shader::TYPE_FRAGMENT, "./shaders/terrain.frag");

	terrainShader = new Eng::Shader();
	terrainShader->build(vShader, fShader);
	terrainShader->render();

	// Visual Tools Setup
	std::vector<std::vector<BaseTool*>> visualToolGroups;
	std::vector<BaseTool*> visualGroup;

	visualGroup.push_back(&ShadesVisualTool::getInstance());
	visualGroup.push_back(&SolidVisualTool::getInstance());
	visualGroup.push_back(&WireframeVisualTool::getInstance());
	visualGroup.push_back(&AltitudeVisualTool::getInstance());

	visualToolGroups.push_back(visualGroup);

	// Brush Tools Setup
	std::vector<std::vector<BaseTool*>> brushToolGroups;
	std::vector<BaseTool*> selectGroup;
	std::vector<BaseTool*> brushGroup;

	selectGroup.push_back(new CursorTool(CURSOR_SENSITIVITY));

	brushGroup.push_back(&SculptingBrushTool::getInstance());
	brushGroup.push_back(&SmoothingBrushTool::getInstance());
	brushGroup.push_back(&ErosionBrushTool::getInstance());

	brushToolGroups.push_back(selectGroup);
	brushToolGroups.push_back(brushGroup);

	// Views setup
	TopMenuBar* topMenuBar = new TopMenuBar();
	StatusBar* statusBar = new StatusBar();

	PointerToolWindow* pointerToolWin = new PointerToolWindow();
	PointerToolSettingsWindow* pointerToolSetWin = new PointerToolSettingsWindow();
	VisualToolWindow* visualToolWin = new VisualToolWindow();
	SetupWindow* setupWin = new SetupWindow();

	pointerToolWin->init(brushToolGroups);
	visualToolWin->init(visualToolGroups);

	windows.push_back(pointerToolWin);
	windows.push_back(pointerToolSetWin);
	windows.push_back(visualToolWin);
	windows.push_back(setupWin);

	// Controllers setup
	SetupController& setupController = SetupController::getInstance();
	AppController& appController = AppController::getInstance();
	CameraGestureController& cameraController = CameraGestureController::getInstance();
	PointerController& pointerController = PointerController::getInstance();
	VisualController& visualController = VisualController::getInstance();
	UIController& uiController = UIController::getInstance();

	setupController.init(setupWin);
	appController.init(statusBar);
	uiController.init(windows, topMenuBar, statusBar);
	cameraController.init();
	pointerController.init(pointerToolWin, pointerToolSetWin);
	visualController.init(visualToolWin);

	visualController.setTopMenuSupport(topMenuBar);

	// Camera Setup
	glm::mat4 camera = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 96.0f, 512.0f));
	Eng::PerspectiveCamera* perspectiveCamera = new Eng::PerspectiveCamera("mainCamera", camera);
	perspectiveCamera->setCameraParams(45.0f, RATIO_16_9, 1.0f, 5000.0f);

	// Lighting Setup
	sunLight = new Eng::InfiniteLight("sun");
	sunLight->setDirection(glm::normalize(glm::vec3(-0.5f, -0.707f, -0.5f)));
	sunLight->setAmbient(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	sunLight->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	sunLight->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	// Scene Graph Setup
	Eng::Node* root = eng.getSceneGraphInstance();
	root->addChild(perspectiveCamera);
	root->addChild(sunLight);

	mainCamera = perspectiveCamera;
	eng.setActiveCamera(mainCamera);

	// Callbacks Setup
	eng.setOnSpecialPressedCallback(onSpecialKeyDownCallback);
	eng.setOnSpecialReleasedCallback(onSpecialKeyUpCallback);
	eng.setOnKeyboardPressedCallback(onKeyboardPressedCallback);

	eng.setOnMouseCallback(onMouseCallback);
	eng.setOnMouseMotionCallback(onMouseMotionCallback);
	eng.setOnPassiveMouseMotionCallback(onPassiveMouseMotionCallback);
	eng.setOnMouseWheelCallback(onMouseWheelCallback);

	eng.setOnResizeCallback(onResize);
	eng.setOnTextDrawCallback(renderingImGui);

	// Start Main Loop
	eng.start(renderingLoop);

	// Cleanup
	eng.free();

	appController.free();
	cameraController.free();
	pointerController.free();
	visualController.free();
	setupController.free();

	uiController.free();

	return 0;
}
