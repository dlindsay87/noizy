#include "sdl_init.h"

void initSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}
	std::cout << "SDL initialized. ";

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, OPENGL_PROFILE) !=
		0 ||
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0 ||
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) != 0) {
		std::cerr << "SDL GL set context failed: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}
	std::cout << "SDL GL context set." << std::endl;

	if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BIT_SIZE) != 0 ||
	    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BIT_SIZE) != 0 ||
	    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BIT_SIZE) != 0 ||
	    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BIT_SIZE) != 0) {
		std::cerr << "SDL GL set color buffer size failed: "
			  << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << "SDL GL color buffer size attributes set." << std::endl;

	if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_SIZE) != 0 ||
	    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0) {
		std::cerr << "SDL GL set color buffer size failed: "
			  << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << "SDL GL buffer and depth attributes set." << std::endl;
}
