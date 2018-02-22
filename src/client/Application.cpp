#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <string>
#include <memory>
#include <cassert>
#include "SceneInit.h"
#include "Application.h"
#include "Resource.h"
#include "SceneMainMenu.h"
#include "SceneGame.h"
#include "BlockList.h"

Application::Application(const std::vector<std::string>& args)
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Starting %s", args[0].c_str());
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Args: %i", args.size() - 1);

	if (args.size() % 2 == 0)
		throw std::invalid_argument("Wrong args! Problem appeared""");

	for (size_t i = 1;i < args.size();i++)
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%c=%c", args[1].c_str(), args[2].c_str());

		if (args[1] == "WindowWidth")
			mWindowWidth = std::stoi(args[2]);
		else if (args[1] == "WindowHeight")
			mWindowHeight = std::stoi(args[2]);
	}
}

int Application::run()
{
	//init sdl
	SDL_Init(SDL_INIT_EVERYTHING);

	//create sdl window
	const auto window = SDL_CreateWindow("MagicCube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight,SDL_WINDOW_OPENGL);

	//make context current
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	const auto oglContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, oglContext);

	//init glew
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to init glew");

	//shown init scene and init resource
	mShownScene = std::make_shared<SceneInit>([&]()
	{
		//refresh
		SDL_GL_SwapWindow(window);

		//init resource
		Resource::init();

		//init block
		BlockList.init();
	});

	//switch to main scene
	switchSceneTo<SceneGame>();

	//should close
	auto shouldProgramClose = false;

	while(!shouldProgramClose)
	{
		//switch scene
		if (mNextScene)
		{
			mShownScene = mNextScene;
			mNextScene = nullptr;
		}

		//refresh event
		SDL_Event sdlEvent;
		while(SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				shouldProgramClose = true;
				break;
			default:
				break;
			}
		}

		//draw
		mShownScene->draw();

		SDL_GL_SwapWindow(window);
	}

	return 0;
}