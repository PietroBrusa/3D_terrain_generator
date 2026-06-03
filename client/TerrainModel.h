#pragma once
#include "engine.h"
#include "TerrainConfig.h"
#include "TextureConfig.h"
#include <vector>

class TerrainModel
{
public:
	static TerrainModel* generateTerrain(TerrainConfig terrainConfig, TextureConfig textureConfig, std::atomic<float>* progress);

	TerrainModel(TerrainConfig terrainConfig, TextureConfig textureConfig);
	~TerrainModel();

	void loadOnScene();
	void update();

	std::vector<Eng::Mesh*> getTerrainChunks() const;
	Eng::Texture* getHeightMapTexture() const;
	std::vector<float>& getTerrainImage();
	TerrainConfig getTerrainConfig() const;
	TextureConfig getTextureConfig() const;

private:
	void generate(std::atomic<float>* progress);

	int gridDimX;
	int gridDimZ;

	TerrainConfig terrainConfig;
	TextureConfig textureConfig;

	Eng::Texture* heightMapTexture;
	std::vector<Eng::Mesh*> terrainChunks;
	std::vector<float> terrainImage;
};
