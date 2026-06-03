#pragma once

#include "AnchorWindow.h"

#include <string>
#include <glm/glm.hpp>

class StatusBar : public AnchorWindow
{
public:
	StatusBar();
	~StatusBar() override;

	void drawContent() override;

	void updateStats(glm::vec3 cameraPos, int fps);
	void setMessage(const std::string& msg);
	void setProgress(bool show, float value = 0.0f);

protected:
	void basePosition() override;

private:
	glm::vec3 cameraPos;
	int fps;
	std::string currentMessage;

	bool showProgress;
	float progressValue;
};
