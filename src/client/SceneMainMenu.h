#pragma once

#include "IScene.h"

class SceneMainMenu: public IScene
{
protected:
	//!render
	void draw() override;
	//!refresh
	void refresh(double timePassed) override {};

public:
	~SceneMainMenu() = default;
};