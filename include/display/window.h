#ifndef __SDL_WINDOW_H__
#define __SDL_WINDOW_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

#include "input_manager.h"

typedef enum WindowFlags {
	FULLSCREEN = 0x1,
	CONTEXTUAL = 0x2,
	VISIBLE = 0x4,
	INVISIBLE = 0x8,
	BORDERLESS = 0x10,
	RESIZABLE = 0x20,
	MOUSE_GRABBED = 0x100,
	INPUT_FOCUS = 0x200,
	MOUSE_FOCUS = 0x400,
	MOUSE_CAPTURE = 0x4000,
	ALWAYS_ON_TOP = 0X8000,
	NO_TASKBAR = 0x10000,
	KEY_GRABBED = 0x100000,
	TYPICAL = CONTEXTUAL | INPUT_FOCUS | RESIZABLE | VISIBLE
} WF;

class Window
{
      private:
	const char *_name;
	int _width, _height;
	unsigned int _flags;
	int _vSync;

	SDL_Window *_window;
	SDL_GLContext _context;
	const unsigned char *_version;

	void _createWindow();
	void _setContextVersion();

      public:
	Window(const char *name, int w, int h, unsigned int f = WF::TYPICAL);
	Window() {}

	~Window();

	void init(const char *name, int w, int h, unsigned int f = WF::TYPICAL);

	void processInput(Input *ip);
	void update(uint state);

	void resize();
	void setVsync();

	SDL_Window *getWindow() const { return _window; }
	SDL_GLContext getContext() const { return _context; }

	int getWidth() const { return _width; }
	int getHeight() const { return _height; }

	float getAspectRatio() const
	{
		return static_cast<float>(_width) / _height;
	}
};

#endif
