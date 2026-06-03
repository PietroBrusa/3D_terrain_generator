#include "TerrainModel.h"

#include "TerrainGenerator.h"
#include "TextureGenerator.h"

#include "configuration.h"

TerrainModel* TerrainModel::generateTerrain(TerrainConfig terrainConfig, TextureConfig textureConfig, std::atomic<float>* progress)
{
	if (!terrainConfig.isValid() || !textureConfig.isValid()) return nullptr;

	TerrainModel* model = new TerrainModel(terrainConfig, textureConfig);
	model->generate(progress);
	return model;
}

TerrainModel::TerrainModel(TerrainConfig terrainConfig, TextureConfig textureConfig)
	: gridDimX(0),
	gridDimZ(0),
	heightMapTexture(nullptr),
	terrainConfig(terrainConfig),
	textureConfig(textureConfig)
{}

TerrainModel::~TerrainModel()
{
	for (auto* chunk : terrainChunks)
	{
		if (chunk)
		{
			Eng::Node* root = Eng::Base::getInstance().getSceneGraphInstance();
			root->removeChild(chunk);
			delete chunk;
		}
	}

	terrainChunks.clear();
}

void TerrainModel::loadOnScene()
{
	heightMapTexture = new Eng::Texture("TerrainHeightMap", textureConfig.size, textureConfig.size, terrainImage);

	Eng::Material* material = new Eng::Material("TerrainMaterial");
	material->setSpecular(glm::vec4(0.0f));
	material->setTexture(heightMapTexture);

	Eng::Node* root = Eng::Base::getInstance().getSceneGraphInstance();

	for (auto* chunk : terrainChunks)
	{
		if (!chunk) continue;

		chunk->setMaterial(material);
		root->addChild(chunk);
	}

	Eng::Shader* shader = Eng::Shader::getCurrentInstance();
	if (shader) {
		int heightScaleLoc = shader->getParamLocation("heightScale");
		shader->setFloat(heightScaleLoc, terrainConfig.heightScale);
	}
}

void TerrainModel::update()
{
	glm::mat4 viewMatrix = Eng::Base::getInstance().getActiveCamera()->getViewMatrix();
	glm::vec3 cameraPos = glm::inverse(viewMatrix)[3];

	float spacing = terrainConfig.vertexSpacing;
	int totalVertices = terrainConfig.size / spacing;
	int chunkSize = std::min(totalVertices, (int)TERRAIN_CHUNK_SIZE);

	int gridDimX = totalVertices / chunkSize;
	float physicalGlobalSize = terrainConfig.size;

	for (int i = 0; i < terrainChunks.size(); i++)
	{
		Eng::Mesh* chunk = terrainChunks[i];
		if (!chunk) continue;

		int gridX = i % gridDimX;
		int gridZ = i / gridDimX;

		int startX = gridX * chunkSize;
		int startZ = gridZ * chunkSize;

		float centerX = ((startX + (chunkSize / 2.0f)) * spacing) - (physicalGlobalSize / 2.0f);
		float centerZ = ((startZ + (chunkSize / 2.0f)) * spacing) - (physicalGlobalSize / 2.0f);
		glm::vec3 chunkCenter(centerX, 0.0f, centerZ);

		float distance = glm::distance(cameraPos, chunkCenter);

		unsigned int targetLod = 0;
		float maxLodDistance = LOD_MIN_DIST;

		for (int lod = 0; lod < TERRAIN_LODS_COUNT; lod++)
		{
			if (distance <= maxLodDistance) break;
			maxLodDistance *= 2;
			targetLod++;
		}

		chunk->setActiveLOD(targetLod);
	}
}

std::vector<Eng::Mesh*> TerrainModel::getTerrainChunks() const
{
	return terrainChunks;
}

Eng::Texture* TerrainModel::getHeightMapTexture() const
{
	return this->heightMapTexture;
}

std::vector<float>& TerrainModel::getTerrainImage()
{
	return this->terrainImage;
}

TerrainConfig TerrainModel::getTerrainConfig() const
{
	return this->terrainConfig;
}

TextureConfig TerrainModel::getTextureConfig() const
{
	return this->textureConfig;
}

void TerrainModel::generate(std::atomic<float>* progress)
{
	TerrainGenerator terrGenerator(terrainConfig);
	TextureGenerator textGenerator(textureConfig);

	terrainImage.clear();
	terrainImage = textGenerator.generate(progress);
	terrainChunks = terrGenerator.generate(gridDimX, gridDimZ, progress);
}
