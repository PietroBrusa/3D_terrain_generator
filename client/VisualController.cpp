#include "VisualController.h"

#include "BaseVisualTool.h"
#include "AppEvents.h"

VisualController::VisualController()
	: ToolController(),
	toolWindow(nullptr),
	topMenuBar(nullptr),
	realisticSubscriptionId(-1),
	solidSubscriptionId(-1),
	wireframeSubscriptionId(-1),
	altitudeShadingSubscriptionId(-1)
{}

VisualController& VisualController::getInstance()
{
	static VisualController instance;
	return instance;
}

BaseTool* VisualController::getActiveTool() const
{
    return this->currentTool;
}

void VisualController::onToolSelected(BaseTool* tool, int groupPos, int itemPos)
{
	this->currentTool = tool;

	if (auto visualTool = dynamic_cast<BaseVisualTool*>(this->currentTool))
	{
		visualTool->use();

		if (this->topMenuBar)
		{
			this->topMenuBar->setActiveShadingMode(itemPos);
		}
	}
}

void VisualController::init(ToolWindow* window, IToolSettingsWindow* editorWindow)
{
	this->toolWindow = window;

	if (this->toolWindow)
	{
		this->toolWindow->setListener(this);
	}

	realisticSubscriptionId = MenuDispatcher::getInstance().subscribe(
		AppEvents::MENU_SHADING_REAL,
		[this]()
		{
			this->selectShadingMode(0);
		}
	);

	solidSubscriptionId = MenuDispatcher::getInstance().subscribe(
		AppEvents::MENU_SHADING_SOLID,
		[this]()
		{
			this->selectShadingMode(1);
		}
	);

	wireframeSubscriptionId = MenuDispatcher::getInstance().subscribe(
		AppEvents::MENU_SHADING_WIREFRAME,
		[this]()
		{
			this->selectShadingMode(2);
		}
	);

	altitudeShadingSubscriptionId = MenuDispatcher::getInstance().subscribe(
		AppEvents::MENU_SHADING_ALTITUDE,
		[this]()
		{
			this->selectShadingMode(3);
		}
	);
}

void VisualController::setTopMenuSupport(TopMenuBar* topMenuBar)
{
	this->topMenuBar = topMenuBar;
}

void VisualController::free() const
{
	MenuDispatcher::getInstance().unsubscribe(AppEvents::MENU_SHADING_REAL, realisticSubscriptionId);
	MenuDispatcher::getInstance().unsubscribe(AppEvents::MENU_SHADING_SOLID, solidSubscriptionId);
	MenuDispatcher::getInstance().unsubscribe(AppEvents::MENU_SHADING_WIREFRAME, wireframeSubscriptionId);
	MenuDispatcher::getInstance().unsubscribe(AppEvents::MENU_SHADING_ALTITUDE, altitudeShadingSubscriptionId);
}

void VisualController::selectShadingMode(int position)
{
	if (this->toolWindow)
	{
		this->toolWindow->setSelectedTool(0, position);
	}
}
