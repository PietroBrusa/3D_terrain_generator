#pragma once

#include "BaseVisualTool.h"

class ShadesVisualTool : public BaseVisualTool
{
public:
	static ShadesVisualTool& getInstance();

	void use() override;

private:
	ShadesVisualTool();

	ShadesVisualTool(const ShadesVisualTool&) = delete;
	ShadesVisualTool& operator=(const ShadesVisualTool&) = delete;
};
