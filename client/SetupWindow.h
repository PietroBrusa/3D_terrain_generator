#pragma once

#include "CentredWindow.h"
#include "ISetupListener.h" 

#include "TextureConfig.h"
#include "TerrainConfig.h"

class SetupWindow : public CentredWindow
{
public:
	SetupWindow();

	void drawContent() override;

	void setListener(ISetupListener* listener);

private:
	TextureConfig m_textureConfig;
	TerrainConfig m_terrainConfig;

	ISetupListener* m_listener; 

	void init();
};