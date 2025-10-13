#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <SDL2/SDL.h>
#include <array>
#include <glm/glm.hpp>

class Input
{
      private:
	std::array<bool, SDL_NUM_SCANCODES> _keyMap{};
	std::array<bool, SDL_NUM_SCANCODES> _previousKeyMap{};

	std::array<bool, SDL_BUTTON_X2 + 1> _mouseMap{};
	std::array<bool, SDL_BUTTON_X2 + 1> _previousMouseMap{};

	glm::ivec2 _mousePos;
	glm::ivec2 _dMouse;

	float _dWheel;
	bool _winAdj;
	bool _exit;

      public:
	Input();

	~Input() {}

	void reset();

	void processEvents(SDL_Event &e);

	bool isKeyDown(unsigned int scancode) const;
	bool isKeyPressed(unsigned int scancode) const;
	bool isButtonDown(unsigned int buttonID) const;
	bool isButtonPressed(unsigned int buttonID) const;

	glm::ivec2 getMousePosition() const { return _mousePos; }
	glm::ivec2 getMouseDelta() const { return _dMouse; }
	float getWheelDelta() const { return _dWheel; }
	bool isWindowAdjusted() const { return _winAdj; }
	bool willExit() const { return _exit; }
};

#endif
