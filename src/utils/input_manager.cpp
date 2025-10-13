#include "input_manager.h"

Input::Input()
    : _mousePos{0, 0}, _dMouse{0, 0}, _dWheel(0.0f), _winAdj(false),
      _exit(false)
{
}

void Input::reset()
{
	_previousKeyMap = _keyMap;
	_previousMouseMap = _mouseMap;
	_dMouse = {0, 0};
	_dWheel = 0.0f;
	_winAdj = false;
}

void Input::processEvents(SDL_Event &e)
{
	switch (e.type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		_keyMap[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		_mouseMap[e.button.button] = (e.type == SDL_MOUSEBUTTONDOWN);
		break;
	case SDL_MOUSEMOTION:
		_mousePos = glm::ivec2(e.motion.x, e.motion.y);
		_dMouse = glm::ivec2(e.motion.xrel, e.motion.yrel);
		break;
	case SDL_MOUSEWHEEL:
		_dWheel = e.wheel.preciseY;
		break;
	case SDL_WINDOWEVENT:
		if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
		    e.window.event == SDL_WINDOWEVENT_RESIZED) {
			_winAdj = true;
		}
		break;
	case SDL_QUIT:
		_exit = true;
		break;
	}
}

bool Input::isKeyDown(unsigned int scancode) const { return _keyMap[scancode]; }

bool Input::isKeyPressed(unsigned int scancode) const
{
	return _keyMap[scancode] && !_previousKeyMap[scancode];
}

bool Input::isButtonDown(unsigned int buttonID) const
{
	return _mouseMap[buttonID];
}

bool Input::isButtonPressed(unsigned int buttonID) const
{
	return _mouseMap[buttonID] && !_previousMouseMap[buttonID];
}