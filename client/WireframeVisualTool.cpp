#include "WireframeVisualTool.h"

WireframeVisualTool::WireframeVisualTool()
    : BaseVisualTool("Wireframe", "res/wireframe.png")
{}

WireframeVisualTool& WireframeVisualTool::getInstance()
{
	static WireframeVisualTool instance;
	return instance;
}

void WireframeVisualTool::use()
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();

	if (shader)
	{
		int shadingModeLoc = shader->getParamLocation("shadingMode");
		shader->setInt(shadingModeLoc, 2);
	}

	Eng::Base::getInstance().changeWireFrame(true);
}
