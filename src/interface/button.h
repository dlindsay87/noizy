#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "input_manager.h"
#include "renderer.h"

#ifndef B_POINTS
#define B_POINTS 4
#endif

struct ButtonForm {
	TextOverlay labelOV;
	TextOverlay valueOV;

	glm::ivec2 pointArr[B_POINTS];
	glm::ivec2 pos, scale;

	HashableColor color;
};

template <typename U> class IButton
{
      private:
	ButtonForm _button;
	glm::vec<2, U, glm::lowp> _valueLimits;
	U _value;

	virtual bool _isOverlapping(glm::ivec2 mousePos) const
	{
		bool within_x =
		    std::abs(mousePos.x - _button.pos.x) <= _button.scale.x / 2;

		bool within_y =
		    std::abs(mousePos.y - _button.pos.y) <= _button.scale.y / 2;

		return within_x && within_y;
	}

	virtual std::string _valToString() const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << _value;
		return ss.str();
	}

      public:
	IButton() {}

	void init(const char *label, U value, glm::ivec2 pos, glm::ivec2 scale,
		  glm::vec<2, U, glm::lowp> valLimits)
	{
		_button.labelOV.text = label;
		_button.labelOV.font.color = ColorSelection[DC::NEUTRAL_W];

		_value = value;
		_button.valueOV.text = _valToString();
		_button.valueOV.font.color = ColorSelection[DC::NEUTRAL_W];

		_button.pos = pos;
		_button.scale = scale;

		_button.pointArr[0] = {pos.x + scale.x / 2,
				       pos.y + scale.y / 2};
		_button.pointArr[1] = {pos.x + scale.x / 2,
				       pos.y - scale.y / 2};
		_button.pointArr[2] = {pos.x - scale.x / 2,
				       pos.y - scale.y / 2};
		_button.pointArr[3] = {pos.x - scale.x / 2,
				       pos.y + scale.y / 2};

		_button.labelOV.font.size = DS::SMALL;
		_button.valueOV.font.size = DS::TINY;

		_button.labelOV.pos.x = pos.x;
		_button.labelOV.pos.y =
		    pos.y - (_button.scale.y + _button.labelOV.font.size);

		_button.valueOV.pos.x = pos.x;
		_button.valueOV.pos.y =
		    pos.y + (_button.scale.y + _button.valueOV.font.size);

		_valueLimits = valLimits;
	}

	virtual void processInput(Input *ip)
	{
		glm::ivec2 mousePos = ip->getMousePosition();

		if (_isOverlapping(mousePos)) {
			_button.color = ColorSelection[DC::HOVER_Y];
			if (ip->isButtonPressed(SDL_BUTTON_LEFT)) {
				_button.color = ColorSelection[DC::SELECT_B];
				_value++;
			}
		} else {
			_button.color = ColorSelection[DC::NEUTRAL_W];
		}
	}

	virtual void update()
	{
		_value = std::clamp(_value, _valueLimits.x, _valueLimits.y);
		_button.valueOV.text = _valToString();
	};

	virtual void draw(Renderer *ren, float intp)
	{
		ren->drawCachedText(_button.labelOV);
		ren->drawDynamicText(_button.valueOV);

		SDL_SetRenderDrawColor(ren->getRenderer(), _button.color.r,
				       _button.color.g, _button.color.b,
				       _button.color.a);

		glm::ivec2 oldpoint = _button.pointArr[B_POINTS - 1];
		for (size_t i = 0; i < B_POINTS; ++i) {

			glm::ivec2 newpoint = _button.pointArr[i];
			SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x,
					   oldpoint.y, newpoint.x, newpoint.y);
			oldpoint = newpoint;
		}
	}

	virtual const char *getLabel() const
	{
		return _button.labelOV.text.c_str();
	}

	U getValue() const { return _value; }
};

#endif
