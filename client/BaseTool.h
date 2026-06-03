#pragma once

#include "engine.h"

class BaseTool
{
public:
	BaseTool(std::string name = "Tool", std::string iconResourcePath = "")
		: name(name)
	{
		iconTexture = new Eng::Texture(name + "Texture", iconResourcePath);
		iconId = iconTexture->getTexId();
	}

	virtual ~BaseTool()
	{
		if (iconTexture)
		{
			delete iconTexture;
			iconTexture = nullptr;
		}
	}

	std::string getName() const { return name; }

	int getIconId() const { return iconId; }

private:
	std::string name;
	int iconId;

	Eng::Texture* iconTexture;
};
