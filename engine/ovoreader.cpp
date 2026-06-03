#define _CRT_SECURE_NO_WARNINGS
#include "engine.h"

#include <glm/gtc/packing.hpp>

#include <limits.h>

#include <vector>
#include <iomanip>
#include <iostream>

Eng::OvoReader::OvoReader() : file(nullptr)
{
}

Eng::OvoReader::~OvoReader()
{
	if (file)
	{
		fclose(file);
	}
}

Eng::Node* Eng::OvoReader::load(const std::string& filename)
{
	file = fopen(filename.c_str(), "rb");

	if (!file)
	{
		std::cerr << "ERROR: Could not open file " << filename << std::endl;
		return nullptr;
	}

	unsigned int chunkId;
	unsigned int chunkSize;

	Eng::Node* rootNode = nullptr;

	size_t lastSlash = filename.find_last_of("/\\");
	if (lastSlash != std::string::npos) {
		basePath = filename.substr(0, lastSlash + 1);
	}
	else {
		basePath = "./";
	}

	// Configure stream:
	std::cout.precision(2);
	std::cout << std::fixed;

	while (true)
	{
		fread(&chunkId, sizeof(unsigned int), 1, file);
		if (feof(file))
			break;
		fread(&chunkSize, sizeof(unsigned int), 1, file);

#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "Processing chunk (" << chunkId << "; " << chunkSize << " bytes)" << std::endl;
#endif

		char* chunkData = new char[chunkSize];
		if (fread(chunkData, sizeof(char), chunkSize, file) != chunkSize)
		{
			std::cerr << "ERROR: Could not read chunk data from file" << std::endl;
			delete[] chunkData;
			break;
		}

		unsigned int position = 0;
		Type type = (Type)chunkId;

		switch (type)
		{
		case Eng::OvoReader::Type::OBJECT:
			// Process OBJECT chunk
			processObjectChunk(chunkData, position);
			break;

		case Eng::OvoReader::Type::MATERIAL:
			// Process MATERIAL chunk
			processMaterialChunk(chunkData, position);
			break;

		case Eng::OvoReader::Type::NODE:
		case Eng::OvoReader::Type::LIGHT:
		case Eng::OvoReader::Type::MESH:
			// Process NODE / LIGHT / MESH chunk
			if (!rootNode)
			{
				rootNode = processNodeChunk(chunkData, position, type);
			}
			else
			{
				std::cerr << "WARNING: Multiple root nodes found in file " << filename << "; ignoring extra nodes." << std::endl;
			}
			break;
		default:
			break;
		}
		delete[] chunkData;
	}

	if (!rootNode)
	{
		std::cerr << "ERROR: Could not load scene from file " << filename << std::endl;
	}

	fclose(file);
	file = nullptr;
	return rootNode;
}

// Loaders

Eng::Node* Eng::OvoReader::loadNextNode()
{
	unsigned int chunkId;
	unsigned int chunkSize;

	fread(&chunkId, sizeof(unsigned int), 1, file);
	if (feof(file))
		return nullptr;
	fread(&chunkSize, sizeof(unsigned int), 1, file);

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Processing NODE chunk (" << chunkId << "; " << chunkSize << " bytes)" << std::endl;
#endif

	char* chunkData = new char[chunkSize];
	if (fread(chunkData, sizeof(char), chunkSize, file) != chunkSize)
	{
		std::cerr << "ERROR: Could not read chunk data from file" << std::endl;
		delete[] chunkData;
		return nullptr;
	}

	unsigned int position = 0;
	Type type = (Type)chunkId;

	if (type == Type::NODE || type == Type::LIGHT || type == Type::MESH)
	{
		// Process NODE chunk
		Eng::Node* node = processNodeChunk(chunkData, position, type);
		delete[] chunkData;
		return node;
	}
	else
	{
		std::cerr << "ERROR: Unexpected chunk type " << (int)chunkId << " while loading node" << std::endl;
		delete[] chunkData;
		return nullptr;
	}
}

// Processors for different chunk types

void Eng::OvoReader::processObjectChunk(char* data, unsigned int& size)
{
	unsigned int versionId;
	memcpy(&versionId, data + size, sizeof(unsigned int));
	std::cout << "Version:  " << versionId << std::endl;
	size += sizeof(unsigned int);
}

// Material and Texture processors

Eng::Texture* Eng::OvoReader::processTextureChunk(char* data, unsigned int& size)
{
	char textureName[FILENAME_MAX];
	strcpy(textureName, data + size);
	size += (unsigned int)strlen(textureName) + 1;

	if (!strcmp(textureName, "[none]"))
	{
		return nullptr;
	}

	// Read texture props
	char normalMapName[FILENAME_MAX];
	strcpy(normalMapName, data + size);
	size += (unsigned int)strlen(normalMapName) + 1;

	char heightMapName[FILENAME_MAX];
	strcpy(heightMapName, data + size);
	size += (unsigned int)strlen(heightMapName) + 1;

	char roughnessMapName[FILENAME_MAX];
	strcpy(roughnessMapName, data + size);
	size += (unsigned int)strlen(roughnessMapName) + 1;

	char metalnessMapName[FILENAME_MAX];
	strcpy(metalnessMapName, data + size);
	size += (unsigned int)strlen(metalnessMapName) + 1;

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Texture:" << std::endl;
	std::cout << "- normal map: " << normalMapName << std::endl;
	std::cout << "- height map: " << heightMapName << std::endl;
	std::cout << "- roughness map: " << roughnessMapName << std::endl;
	std::cout << "- metalness map: " << metalnessMapName << std::endl;
#endif

	// Create texture
	std::string texturePath = basePath + std::string(textureName);
	Eng::Texture* texture = new Eng::Texture(textureName, texturePath);
	return texture;
}

void Eng::OvoReader::processMaterialChunk(char* data, unsigned int& size)
{
	// Get material name
	char materialName[FILENAME_MAX];
	strcpy(materialName, data + size);
	size += (unsigned int)strlen(materialName) + 1;

	// Get material properties
	glm::vec3 emission;
	glm::vec3 albedo;
	float roughness;
	float metalness;
	float transparency;

	memcpy(&emission, data + size, sizeof(glm::vec3));
	size += sizeof(glm::vec3);

	memcpy(&albedo, data + size, sizeof(glm::vec3));
	size += sizeof(glm::vec3);

	memcpy(&roughness, data + size, sizeof(float));
	size += sizeof(float);

	memcpy(&metalness, data + size, sizeof(float));
	size += sizeof(float);

	memcpy(&transparency, data + size, sizeof(float));
	size += sizeof(float);

	// Calculate material components
	glm::vec4 ambient = glm::vec4(albedo * 0.2f, transparency);
	glm::vec4 specular = glm::vec4(albedo * 0.4f, transparency);
	glm::vec4 diffuse = glm::vec4(albedo * 0.6f, transparency);
	float shininess = (1.0f - std::sqrt(roughness)) * 128.0f;

	// Create and store material
	Material* material = new Material(std::string(materialName), glm::vec4(emission, 1.0f), ambient, diffuse, specular, shininess);
	materials[materialName] = material;

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Loaded material " << materialName << std::endl;
	std::cout << " - Ambient: " << ambient.r << ", " << ambient.g << ", " << ambient.b << ", " << ambient.a << std::endl;
	std::cout << " - Diffuse: " << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ", " << diffuse.a << std::endl;
	std::cout << " - Specular: " << specular.r << ", " << specular.g << ", " << specular.b << ", " << specular.a << std::endl;
	std::cout << " - Shininess: " << shininess << std::endl;
#endif

	// <Optional> Load texture
	Eng::Texture* texture = processTextureChunk(data, size);

	if (texture)
	{
		material->setTexture(texture);
	}
}

// Nodes processors

Eng::Node* Eng::OvoReader::processNodeChunk(char* data, unsigned int& size, Type type)
{
	// Get node name
	char nodeName[FILENAME_MAX];
	strcpy(nodeName, data + size);
	size += (unsigned int)strlen(nodeName) + 1;

	// Get node matrix
	glm::mat4 nodeMatrix;
	memcpy(&nodeMatrix, data + size, sizeof(glm::mat4));
	size += sizeof(glm::mat4);

	// Get number of children
	unsigned int numberOfChildren;
	memcpy(&numberOfChildren, data + size, sizeof(unsigned int));
	size += sizeof(unsigned int);

	// <Optional> Get target name - [none] if not used
	char targetName[FILENAME_MAX];
	strcpy(targetName, data + size);
	size += (unsigned int)strlen(targetName) + 1;

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Loaded node " << nodeName << " with " << numberOfChildren << " children." << std::endl;
	std::cout << "Current read size: " << size << " bytes." << std::endl;
	MAT2STR(nodeMatrix);
#endif

	Eng::Node* node = nullptr;

	if (type == Type::LIGHT)
	{
		// Process LIGHT chunk
		node = processLightChunk(data, size, nodeName, nodeMatrix);
	}
	else if (type == Type::MESH)
	{
		// Process MESH chunk
		node = processMeshChunk(data, size, nodeName, nodeMatrix);
	}
	else
	{
		// Create node
		node = new Eng::Node(std::string(nodeName), nodeMatrix);
	}

	if (!node)
	{
		return nullptr;
	}

	for (unsigned int i = 0; i < numberOfChildren; ++i)
	{
		Eng::Node* childNode = loadNextNode();
		if (childNode)
		{
			node->addChild(childNode);
		}
	}

	return node;
}

Eng::Node* Eng::OvoReader::processLightChunk(char* data, unsigned int& size, char* name, glm::mat4& matrix)
{
	// Get light subtype
	unsigned char lightSubtype;
	memcpy(&lightSubtype, data + size, sizeof(unsigned char));
	size += sizeof(unsigned char);

	// <Color> Get light properties
	glm::vec3 color;
	memcpy(&color, data + size, sizeof(glm::vec3));
	size += sizeof(glm::vec3);

	// Influence radius
	float influenceRadius;
	memcpy(&influenceRadius, data + size, sizeof(float));
	size += sizeof(float);

	// <Light> Get light properties
	glm::vec3 direction;
	memcpy(&direction, data + size, sizeof(glm::vec3));
	size += sizeof(glm::vec3);

	// Cutoff angle (for spotlights)
	float cutoffAngle;
	memcpy(&cutoffAngle, data + size, sizeof(float));
	size += sizeof(float);

	// Create light
	Eng::Light* light;

	if ((LightSubtype)lightSubtype == LightSubtype::OMNI)
	{
		light = new Eng::OmniLight(std::string(name), matrix);
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "Created OmniLight: " << name << std::endl;
#endif
	}
	else if ((LightSubtype)lightSubtype == LightSubtype::DIRECTIONAL)
	{
		light = new Eng::InfiniteLight(std::string(name), matrix, direction);
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "Created InfiniteLight: " << name << std::endl;
#endif
	}
	else if ((LightSubtype)lightSubtype == LightSubtype::SPOT)
	{
		light = new Eng::SpotLight(std::string(name), matrix, direction, cutoffAngle, influenceRadius);
#if defined(DEBUG) || defined(_DEBUG)
		std::cout << "Created SpotLight: " << name << "; cutoff: " << cutoffAngle << std::endl;
#endif
	}
	else
	{
		std::cerr << "ERROR: Unknown light subtype " << (int)lightSubtype << std::endl;
		return nullptr;
	}

	light->setAmbient(glm::vec4(color, 1.0f));
	light->setDiffuse(glm::vec4(color, 1.0f));
	light->setSpecular(glm::vec4(color, 1.0f));

	return light;
}

Eng::Node* Eng::OvoReader::processMeshChunk(char* data, unsigned int& size, char* name, glm::mat4& matrix)
{
	// Get mesh subtype
	size += sizeof(unsigned char); // Subtype is currently unused

	// Get mesh material name - [none] if not used
	char materialName[FILENAME_MAX];
	strcpy(materialName, data + size);
	size += (unsigned int)strlen(materialName) + 1;

	// Mesh bounding sphere radius
	float meshRadius;
	memcpy(&meshRadius, data + size, sizeof(float));
	size += sizeof(float);

	// Mesh min & max bounding box [skipped]
	size += sizeof(glm::vec3); // min
	size += sizeof(glm::vec3); // max

	// Phisical properties [skipped]
	size += sizeof(char); // phisics used

	// Load LODs
	// unsigned int numberOfLods;
	// memcpy(&numberOfLods, data + size, sizeof(unsigned int));
	size += sizeof(unsigned int);

	// Read LODs data: vertex
	std::vector<glm::vec3> meshVertexList;
	std::vector<glm::uvec3> meshFaceList;
	std::vector<glm::vec3> meshNormalList;
	std::vector<glm::vec2> meshTextureCoordList;

	unsigned int numberOfVertexes;
	unsigned int numberOfFaces;

	memcpy(&numberOfVertexes, data + size, sizeof(unsigned int));
	size += sizeof(unsigned int);

	memcpy(&numberOfFaces, data + size, sizeof(unsigned int));
	size += sizeof(unsigned int);

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "LOD " << ": " << numberOfVertexes << " vertexes, " << numberOfFaces << " faces." << std::endl;
#endif

	for (int v = 0; v < numberOfVertexes; ++v)
	{
		// Read vertex data
		glm::vec3 vertex;
		memcpy(&vertex, data + size, sizeof(glm::vec3));
		size += sizeof(glm::vec3);

		meshVertexList.push_back(vertex);

		// Vertex normal
		unsigned int normalData;
		memcpy(&normalData, data + size, sizeof(unsigned int));
		size += sizeof(unsigned int);
		// Return only (x, y, z)
		meshNormalList.push_back(glm::unpackSnorm3x10_1x2(normalData));

		// Texture coordinates
		unsigned int textureData;
		memcpy(&textureData, data + size, sizeof(unsigned int));
		size += sizeof(unsigned int);
		meshTextureCoordList.push_back(glm::unpackHalf2x16(textureData));

		// Tangent vector
		unsigned int tangentData;
		memcpy(&tangentData, data + size, sizeof(unsigned int));
		size += sizeof(unsigned int);

		/*#if defined(DEBUG) || defined(_DEBUG)
				std::cout << "Vertex data:  v" << v << std::endl;
				std::cout << " - xyz: " << vertex.x << ", " << vertex.y << ", " << vertex.z << std::endl;

				glm::vec4 normal = glm::unpackSnorm3x10_1x2(normalData);
				std::cout << " - normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;

				glm::vec2 uv = glm::unpackHalf2x16(textureData);
				std::cout << " - uv: " << uv.x << ", " << uv.y << std::endl;

				glm::vec4 tangent = glm::unpackSnorm3x10_1x2(tangentData);
				std::cout << " - tangent: " << tangent.x << ", " << tangent.y << ", " << tangent.z << ", sign: " << tangent.w << std::endl;
		#endif*/
	}

	for (int f = 0; f < numberOfFaces; f++)
	{
		unsigned int face[3];
		memcpy(&face, data + size, sizeof(unsigned int) * 3);
		size += sizeof(unsigned int) * 3;

		meshFaceList.push_back(glm::uvec3(face[0], face[1], face[2]));

		/*#if defined(DEBUG) || defined(_DEBUG)
				std::cout << "Face data:  f" << f << std::endl;
				std::cout << " - v1: " << face[0] << ", v2: " << face[1] << ", v3: " << face[2] << std::endl;
		#endif*/
	}

	std::vector<std::vector<glm::uvec3>> meshLodFaceList;
	meshLodFaceList.push_back(meshFaceList); // Position 0 is the best resolution

	// Create mesh
	Eng::Mesh* mesh = new Eng::Mesh(std::string(name), matrix, meshVertexList, meshLodFaceList, meshNormalList, meshTextureCoordList);

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << name << " Mash has material " << materialName << std::endl;
#endif

	// Assign material if available
	if (strcmp(materialName, "[none]"))
	{
		auto it = materials.find(materialName);
		if (it != materials.end())
		{
#if defined(DEBUG) || defined(_DEBUG)
			std::cout << "Assigning material " << materialName << " to mesh " << name << std::endl;
#endif
			mesh->setMaterial(it->second);
		}
	}

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Created Mesh: " << name << std::endl;
#endif

	return mesh;
}
