#include "ShadesVisualTool.h"

ShadesVisualTool::ShadesVisualTool()
	: BaseVisualTool("Realisitc", "res/shades.png")
{}

ShadesVisualTool& ShadesVisualTool::getInstance()
{
	static ShadesVisualTool instance;
	return instance;
}

void ShadesVisualTool::use()
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();

	if (shader)
	{
		int shadingModeLoc = shader->getParamLocation("shadingMode");
		shader->setInt(shadingModeLoc, 0);
	}

	Eng::Base::getInstance().changeWireFrame(false);
}
