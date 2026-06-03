#pragma once

#include "BaseTool.h"

class BaseVisualTool : public BaseTool
{
public:
	BaseVisualTool(std::string name = "VisualTool", std::string res = "")
		: BaseTool(name, res) 
	{}
	
	virtual void use() = 0;
};
