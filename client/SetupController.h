#pragma once

#include "SetupWindow.h"
#include "ISetupListener.h"

#include "TerrainConfig.h"
#include "TextureConfig.h"
#include "TerrainModel.h"

#include "EventDispatcher.h"

using MenuDispatcer = EventDispatcher<>;

class SetupController : public ISetupListener
{
public:
	static SetupController& getInstance();

	void init(SetupWindow* window);
	void update();
	void free();

	void onTerrainGenerationRequest(TerrainConfig terrainConfig, TextureConfig textureConfig) override;

	TerrainModel* getActiveTerrainModel() const;
	bool isTerrainGenerated() const;

private:
	SetupController();
	SetupController(const SetupController&) = delete;
	SetupController& operator=(const SetupController&) = delete;

	SetupWindow*  setupWindow;
	TerrainModel* m_activeTerrain;
	bool isGenerated;

	size_t newGenerationSubscriptionId;

	void onNewGeneration();
};
