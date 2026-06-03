#include "SetupController.h"

#include "AppEvents.h"
#include "AppController.h"

SetupController::SetupController()
	: setupWindow(nullptr),
	m_activeTerrain(nullptr),
	isGenerated(false),
	newGenerationSubscriptionId(-1)
{}

SetupController& SetupController::getInstance()
{
	static SetupController instance;
	return instance;
}

void SetupController::init(SetupWindow* window)
{
	this->setupWindow = window;

	if (this->setupWindow)
	{
		this->setupWindow->setListener(this);
	}

	newGenerationSubscriptionId = MenuDispatcer::getInstance().subscribe(AppEvents::MENU_NEW_PROJECT, [this]() { onNewGeneration(); });

	onNewGeneration();
}

void SetupController::update()
{
	if (m_activeTerrain)
		m_activeTerrain->update();
}

void SetupController::free()
{
	delete m_activeTerrain;
	m_activeTerrain = nullptr;

	MenuDispatcer::getInstance().unsubscribe(AppEvents::MENU_NEW_PROJECT, newGenerationSubscriptionId);
}

void SetupController::onTerrainGenerationRequest(TerrainConfig terrainConfig, TextureConfig textureConfig)
{
	if (m_activeTerrain)
	{
		delete m_activeTerrain;
		m_activeTerrain = nullptr;
	}

	auto tempTerrain = std::make_shared<TerrainModel*>(nullptr);

	AppController::getInstance().runBackgroundTask(
		"Generating...",
		[tempTerrain, terrainConfig, textureConfig](std::atomic<float>* progress) 
		{
			progress->store(0.0f);
			*tempTerrain = TerrainModel::generateTerrain(terrainConfig, textureConfig, progress);
			progress->store(1.0f);
		},
		[tempTerrain, terrainConfig, this]()
		{
			m_activeTerrain = *tempTerrain;
			m_activeTerrain->loadOnScene();

			this->isGenerated = true;
		}
	);

	if (setupWindow)
		setupWindow->setVisible(false);
}

TerrainModel* SetupController::getActiveTerrainModel() const
{
	return m_activeTerrain;
}

bool SetupController::isTerrainGenerated() const
{
	return isGenerated;
}

void SetupController::onNewGeneration()
{
	if (setupWindow)
		setupWindow->setVisible(true);

	isGenerated = false;
}
