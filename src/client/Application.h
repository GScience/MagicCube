#pragma once

#include "IScene.h"
#include <vector>

class Application
{
	//!now used scene
	IScenePtr mShownScene;
	//!new scene will be used
	IScenePtr mNextScene;
	
	//!window width
	int mWindowWidth = 1024;
	//!window height
	int mWindowHeight = 704;

public:
	explicit Application(const std::vector<std::string>& args);

	//!start the program
	int run() const;

	//!switch scene
	void switchSceneTo(const IScenePtr& scene);
};