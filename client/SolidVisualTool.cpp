#include "SolidVisualTool.h"

SolidVisualTool::SolidVisualTool()
    : BaseVisualTool("Solid", "res/solid.png")
{}

SolidVisualTool& SolidVisualTool::getInstance()
{
	static SolidVisualTool instance;
	return instance;
}

void SolidVisualTool::use()
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();

	if (shader)
	{
		int shadingModeLoc = shader->getParamLocation("shadingMode");
		shader->setInt(shadingModeLoc, 1);
	}

	Eng::Base::getInstance().changeWireFrame(false);
}
