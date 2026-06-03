#pragma once

#include "BaseVisualTool.h"

class AltitudeVisualTool : public BaseVisualTool
{
public:
	static AltitudeVisualTool& getInstance();

	void use() override;

private:
	AltitudeVisualTool();

	AltitudeVisualTool(const AltitudeVisualTool&) = delete;
	AltitudeVisualTool& operator=(const AltitudeVisualTool&) = delete;
};
