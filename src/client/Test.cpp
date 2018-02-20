#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_image.h>

int main(int argc, char* argv[])
{
	const auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 704, SDL_WINDOW_OPENGL);

	const auto oglContext = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_MakeCurrent(window, oglContext);

	glewInit();
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	const auto surface = IMG_Load("TestSDLLogo.png");

	SDL_Event sdlEvent;
	auto shouldCloseWindow = false;
	while(!shouldCloseWindow)
	{
		while(SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				shouldCloseWindow = true;
				break;
			default:
				break;
			}
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}