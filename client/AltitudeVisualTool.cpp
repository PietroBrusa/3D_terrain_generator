#include "AltitudeVisualTool.h"

AltitudeVisualTool::AltitudeVisualTool()
    : BaseVisualTool("Colors Altitude", "./res/altitude.png")
{}

AltitudeVisualTool& AltitudeVisualTool::getInstance()
{
    static AltitudeVisualTool instance;
    return instance;
}

void AltitudeVisualTool::use()
{
	Eng::Shader* shader = Eng::Shader::getCurrentInstance();

	if (shader)
	{
		int shadingModeLoc = shader->getParamLocation("shadingMode");
		shader->setInt(shadingModeLoc, 3);
	}

	Eng::Base::getInstance().changeWireFrame(false);
}
