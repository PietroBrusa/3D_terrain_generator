#pragma once

#include "BaseVisualTool.h"

class WireframeVisualTool : public BaseVisualTool
{
public:
	static WireframeVisualTool& getInstance();

	void use() override;

private:
	WireframeVisualTool();

	WireframeVisualTool(const WireframeVisualTool&) = delete;
	WireframeVisualTool& operator=(const WireframeVisualTool&) = delete;
};
