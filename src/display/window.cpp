#include "window.h"

Window::Window(const char *name, int w, int h, unsigned int f)
{
	init(name, w, h, f);
}

Window::~Window()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
}

void Window::_createWindow()
{
	_window =
	    SDL_CreateWindow(_name, SDL_WINDOWPOS_CENTERED,
			     SDL_WINDOWPOS_CENTERED, _width, _height, _flags);

	if (!_window) {
		std::cerr << "SDL window creation failed: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}

	std::cout << "SDL Window created." << std::endl;
}

void Window::_setContextVersion()
{
	_context = SDL_GL_CreateContext(_window);

	if (_context == NULL) {
		std::cerr << "OpenGL context creation failed: "
			  << SDL_GetError() << std::endl;
		exit(1);
	}

	std::cout << "OpenGL context created." << std::endl;

	SDL_GL_MakeCurrent(_window, _context);

	std::cout << "SDL GL context current." << std::endl;

	if (glewInit() != GLEW_OK) {
		std::cerr << "Could not initialize GLEW!" << std::endl;
		exit(1);
	}

	std::cout << "GLEW initialized." << std::endl;

	_version = glGetString(GL_VERSION);
	if (_version == 0) {
		std::cerr << "OpenGL configuration failed:" << std::endl;
		exit(1);
	}

	std::cout << "*** OpenGL configured. Version: " << _version << " ***"
		  << std::endl;
}

void Window::init(const char *name, int w, int h, unsigned int f)
{
	_name = name;
	_width = w, _height = h;
	_flags = f;
	_vSync = 0;

	_createWindow();
	_setContextVersion();

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, _width, _height);
	std::cout << "Display initialized.";
	setVsync();
}

void Window::processInput(Input *ip)
{
	if (ip->isWindowAdjusted()) {
		resize();
	}
	if (ip->isKeyPressed(SDL_SCANCODE_V)) {
		setVsync();
	}
}

void Window::update(uint state)
{
	switch (state) {
	case 0:
		//	SDL_ShowCursor(SDL_DISABLE);
		//	SDL_WarpMouseInWindow(_window, _width / 2, _height / 2);
		//	SDL_SetWindowGrab(_window, SDL_TRUE);
		//	break;
		// default:
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetWindowGrab(_window, SDL_FALSE);
		break;
	}
}

void Window::resize()
{
	SDL_GetWindowSize(_window, &_width, &_height);
	glViewport(0, 0, _width, _height);
}

void Window::setVsync()
{
	SDL_GL_SetSwapInterval(_vSync);
	_vSync = (_vSync + 1 == 2 ? -1 : _vSync + 1);

	std::cout << "\nVsync is ";
	switch (SDL_GL_GetSwapInterval()) {
	case -1:
		std::cout << "on and adaptive.\n";
		break;
	case 0:
		std::cout << "off.\n";
		break;
	case 1:
		std::cout << "on.\n";
		break;
	}
}
