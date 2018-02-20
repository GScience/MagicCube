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
	int run();

	//!switch scene
	template<class scene, 
		class = typename std::enable_if<std::is_convertible<scene*, IScene*>::value, scene>::type, 
		class... argsType> 
	void switchSceneTo(argsType&& ...args)
	{
		mNextScene = std::make_shared<scene>(args...);
	}
};