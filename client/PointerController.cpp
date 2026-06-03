#include "PointerController.h"

#include "AppEvents.h"

#include "CameraGestureController.h"
#include "InputController.h"
#include "SetupController.h"

#include "CursorTool.h"
#include "BaseBrushTool.h"

PointerController::PointerController()
	: ToolController(),
	mouseMoveSubscriptionId(-1),
	brushPositionUniformLoc(-1),
	brushRadiusUniformLoc(-1),
	brushActiveUniformLoc(-1),
	brushRadiusColorUniformLoc(-1)
{}

PointerController& PointerController::getInstance()
{
	static PointerController instance;
	return instance;
}

PointerController::~PointerController()
{}

void PointerController::init(ToolWindow* window, IToolSettingsWindow* editorWindow)
{
	this->toolWindow = window;
	this->editorToolWindow = editorWindow;

	if (this->toolWindow)
	{
		this->toolWindow->setListener(this);
	}

	mouseMoveSubscriptionId = MouseMoveDispatcher::getInstance().subscribe(
		AppEvents::LEFT_MOUSE_MOVE,
		[this](int x, int y, int lastX, int lastY)
		{
			this->onCursorMove(x, y, lastX, lastY);
		}
	);
}

void PointerController::update(float deltaTime)
{
	if (!this->currentTool) return;
	auto* brushTool = dynamic_cast<BaseBrushTool*>(this->currentTool);
	if (!brushTool) return;

	glm::vec3 nodePosition;
	bool isHittingTerrain = InputController::getInstance().isHittingTerrain(nodePosition);

	if (!isHittingTerrain)
	{
		hideBrushArea();
		return;
	}

	showBrushArea(brushTool, nodePosition);

	if (!InputController::getInstance().isLeftMouseDown()) return;

	TerrainModel* terrain = SetupController::getInstance().getActiveTerrainModel();
	if (!terrain) return;

	std::vector<float>& imageData = terrain->getTerrainImage();
	int resolution = terrain->getTextureConfig().size;

	UpdateArea area = brushTool->use(nodePosition, terrain->getTerrainConfig(), terrain->getTextureConfig(), deltaTime, imageData);

	if (area.isModified && terrain->getHeightMapTexture())
	{
		terrain->getHeightMapTexture()->updateSubImage(
			area.startX,
			area.startY,
			area.width,
			area.height,
			imageData,
			resolution
		);
	}
}

void PointerController::free() const
{
	MouseMoveDispatcher::getInstance().unsubscribe(AppEvents::LEFT_MOUSE_MOVE, mouseMoveSubscriptionId);
}

BaseTool* PointerController::getActiveTool() const
{
	return this->currentTool;
}

void PointerController::onToolSelected(BaseTool* tool, int groupPos, int itemPos)
{
	this->currentTool = tool;

	Eng::Shader* shader = Eng::Shader::getCurrentInstance();

	if (shader)
	{
		brushPositionUniformLoc = shader->getParamLocation("brushPosition");
		brushRadiusUniformLoc = shader->getParamLocation("brushRadius");
		brushActiveUniformLoc = shader->getParamLocation("isBrushActive");
		brushRadiusColorUniformLoc = shader->getParamLocation("brushRadiusColor");
	}

	if (this->editorToolWindow)
	{
		auto* brushTool = dynamic_cast<BaseBrushTool*>(tool);
		if (brushTool)
			editorToolWindow->setActiveTool(brushTool);
		else
			editorToolWindow->setVisible(false);
	}
}

void PointerController::onToolEditor(BaseTool* tool, int groupPos, int itemPos)
{
	auto* brushTool = dynamic_cast<BaseBrushTool*>(tool);
	if (!brushTool) return;

	editorToolWindow->setActiveTool(brushTool);
	editorToolWindow->setVisible(true);
	this->currentTool = brushTool;
}

void PointerController::onCursorMove(int x, int y, int lastX, int lastY)
{
	if (!this->currentTool) return;

	if (auto* cursorTool = dynamic_cast<CursorTool*>(this->currentTool))
	{
		CameraGestureController::getInstance().cameraRotate(x, y, lastX, lastY);
		hideBrushArea();
	}
}

void PointerController::hideBrushArea()
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();
	if (shader) shader->setBool(brushActiveUniformLoc, false);
}

void PointerController::showBrushArea(BaseBrushTool* brush, glm::vec3 mousePos)
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();
	if (!shader) return;

	shader->setBool(brushActiveUniformLoc, true);
	shader->setFloat(brushRadiusUniformLoc, brush->getRadius());
	shader->setVec3(brushRadiusColorUniformLoc, brush->getRadiusColor());

	Eng::Camera* camera = Eng::Base::getInstance().getActiveCamera();
	glm::vec4 eyeBrushPos = camera->getViewMatrix() * glm::vec4(mousePos, 1.0f);
	shader->setVec4(brushPositionUniformLoc, eyeBrushPos);
}
