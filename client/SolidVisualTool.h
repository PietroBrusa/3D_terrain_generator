#pragma once

#include "BaseVisualTool.h"

class SolidVisualTool : public BaseVisualTool
{
public:
	static SolidVisualTool& getInstance();

	void use() override;

private:
	SolidVisualTool();

	SolidVisualTool(const SolidVisualTool&) = delete;
	SolidVisualTool& operator=(const SolidVisualTool&) = delete;
};
