#pragma once

#include "TerrainConfig.h"
#include "TextureConfig.h"

class ISetupListener
{
public:
	virtual ~ISetupListener() = default;

	virtual void onTerrainGenerationRequest(TerrainConfig terrainConfig, TextureConfig textureConfig) = 0;
};