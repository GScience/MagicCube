#include <SDL2/SDL.h>
#include <string>
#include "SceneMainMenu.h"
#include "Application.h"

Application::Application(const std::vector<std::string>& args)
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Starting %s", args[0].c_str());
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Args: %i", args.size() - 1);

	if (args.size() % 2 == 0)
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Wrong args! Problem appeared");

	for (size_t i = 1;i < args.size();i++)
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%c=%c", args[1].c_str(), args[2].c_str());

		if (args[1] == "WindowWidth")
			mWindowWidth = std::stoi(args[2]);
		else if (args[1] == "WindowHeight")
			mWindowHeight = std::stoi(args[2]);
	}

	//shown scene
	mShownScene = std::make_shared<SceneMainMenu>();
}

int Application::run() const
{
	//create sdl window
	const auto window = SDL_CreateWindow("MagicCube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight,SDL_WINDOW_OPENGL);

	//make context current
	const auto oglContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, oglContext);

	//should close
	auto shouldProgramClose = false;

	while(!shouldProgramClose)
	{
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

void Application::switchSceneTo(const IScenePtr& scene)
{
	mNextScene = scene;
}

int main(const int argc, char* argv[])
{
	std::vector<std::string> args(argc);

	for (auto i = 0; i < argc; i++)
		args[i] = argv[i];

	return Application(args).run();
}