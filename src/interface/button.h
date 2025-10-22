#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "input_manager.h"
#include "renderer.h"

#ifndef S_POINTS
#define S_POINTS 4
#endif

#ifndef T_POINTS
#define T_POINTS 3
#endif

class Box
{
      protected:
	glm::ivec2 _pointArr[S_POINTS];

	glm::ivec2 _pos, _scale;
	HashableColor _color;

	virtual bool _isOverlapping(glm::ivec2 mousePos) const
	{
		bool within_x = std::abs(mousePos.x - _pos.x) <= _scale.x / 2;

		bool within_y = std::abs(mousePos.y - _pos.y) <= _scale.y / 2;

		return within_x && within_y;
	}

      public:
	Box() {}

	virtual void init(glm::ivec2 pos, glm::ivec2 scale)
	{
		_pos = pos;
		_scale = scale;
		_color = ColorSelection[DC::NEUTRAL_W];

		_pointArr[0] = {scale.x / 2, scale.y / 2};
		_pointArr[1] = {scale.x / 2, -scale.y / 2};
		_pointArr[2] = {-scale.x / 2, -scale.y / 2};
		_pointArr[3] = {-scale.x / 2, scale.y / 2};
	}

	virtual void draw(Renderer *ren, float intp)
	{
		SDL_SetRenderDrawColor(ren->getRenderer(), _color.r, _color.g,
				       _color.b, _color.a);

		glm::ivec2 oldpoint = _pointArr[S_POINTS - 1] + _pos;
		for (size_t i = 0; i < S_POINTS; ++i) {

			glm::ivec2 newpoint = _pointArr[i] + _pos;
			SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x,
					   oldpoint.y, newpoint.x, newpoint.y);
			oldpoint = newpoint;
		}
	}
};

class TriButton
{
      protected:
	glm::ivec2 _pos, _scale;
	bool _downright;
	HashableColor _color;
	int *_refVal;

	glm::ivec2 _pointArr[T_POINTS];

	virtual bool _isOverlapping(glm::ivec2 mousePos) const
	{
		auto p = glm::vec2(mousePos);
		auto a = glm::vec2(_pointArr[0] + _pos);
		auto b = glm::vec2(_pointArr[1] + _pos);
		auto c = glm::vec2(_pointArr[2] + _pos);

		// Compute barycentric coordinates
		glm::vec2 v0 = b - a;
		glm::vec2 v1 = c - a;
		glm::vec2 v2 = p - a;

		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);

		float denom = d00 * d11 - d01 * d01;
		if (denom == 0.0f)
			return false; // degenerate triangle

		float v = (d11 * d20 - d01 * d21) / denom;
		float w = (d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;

		// Inside if all barycentric coordinates are between 0 and 1
		return (u >= 0.0f && v >= 0.0f && w >= 0.0f);
	}

      public:
	TriButton() {}

	virtual void init(glm::ivec2 pos, glm::ivec2 scale, bool down, int *ref)

	{
		_pos = pos;
		_scale = scale;
		_downright = down;
		_refVal = ref;

		int dir = _downright ? 1 : -1; // 1 = down, -1 = up
		_pointArr[0] = glm::ivec2(-scale.x / 2, 0);
		_pointArr[1] = glm::ivec2(scale.x / 2, 0);
		_pointArr[2] = glm::ivec2(0, dir * scale.y);
	}

	virtual void processInput(Input *ip)
	{
		glm::ivec2 mousePos = ip->getMousePosition();

		if (_isOverlapping(mousePos)) {
			_color = ColorSelection[DC::HOVER_Y];
			if (ip->isButtonPressed(SDL_BUTTON_LEFT)) {
				_color = ColorSelection[DC::SELECT_B];
				*_refVal -= (_downright * 2 - 1);
			}
		} else {
			_color = ColorSelection[DC::NEUTRAL_W];
		}
	}

	virtual void draw(Renderer *ren, float intp)
	{
		SDL_SetRenderDrawColor(ren->getRenderer(), _color.r, _color.g,
				       _color.b, _color.a);

		glm::ivec2 oldpoint = _pointArr[T_POINTS - 1] + _pos;
		for (size_t i = 0; i < T_POINTS; ++i) {

			glm::ivec2 newpoint = _pointArr[i] + _pos;
			SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x,
					   oldpoint.y, newpoint.x, newpoint.y);
			oldpoint = newpoint;
		}
	}
};

/*
template <typename U> class IncButton
{
      private:
	TriButton _up, _down;
	glm::vec<2, U, glm::lowp> _valueLimits;
	U _value;
	bool _inc; // 2 * _inc - 1

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
*/
#endif
