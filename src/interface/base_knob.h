#ifndef __BASE_KNOB_H__
#define __BASE_KNOB_H__

#include <iomanip>
#include <sstream>

#include "input_manager.h"
#include "renderer.h"

#ifndef N_POINTS
#define N_POINTS 16
#endif

typedef enum class ColorSelection { NEUTRAL, HOVERING, SELECTED } CS;
HashableColor KnobColors[3] = {
    {255, 255, 255, 255}, {127, 191, 0, 255}, {0, 255, 0, 255}};

typedef enum class KnobRadius { SMALL = 20, MID = 40, LARGE = 80 } KR;

/*inline HashableColor ColorSelect(ColorSelection c)
{
	switch (c) {
	case SELECTED:
		return {0, 255, 0, 255};
	case HOVERING:
		return {127, 191, 0, 255};
	case NEUTRAL:
	default:
		return {255, 255, 255, 255};
	}
	// return KnobColors[c];
}*/

struct KnobForm {
	TextOverlay labelOV;
	TextOverlay valueOV;

	glm::ivec2 pointArr[N_POINTS];
	glm::ivec2 pos;
	glm::vec2 rotationLimits;
	int radius;

	// glm::u8vec4 (*colorSelect)(ColorSelection);
	HashableColor color;
};

template <typename U> class IKnob
{
      private:
	KnobForm _knob;
	float _displayScale;

	glm::vec<2, U, glm::lowp> _valueLimits;
	U _value;
	float _step;

	bool _selected;

	virtual bool _isOverlapping(glm::ivec2 mousePos) const
	{
		return sqrt(pow(mousePos.x - _knob.pos.x, 2) +
			    pow(mousePos.y - _knob.pos.y, 2)) <= _knob.radius;
	}

	virtual std::string _valToString() const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << _value;
		return ss.str();
	}

      public:
	IKnob() {}

	void init(const char *label, U value, glm::ivec2 pos, KnobRadius radius,
		  glm::vec2 rotLimits, glm::vec<2, U, glm::lowp> valLimits,
		  float step)
	{
		_knob.labelOV.text = label;
		_knob.labelOV.font.color = KnobColors[(int)CS::NEUTRAL];

		_value = value;
		_knob.valueOV.text = _valToString();
		_knob.valueOV.font.color = KnobColors[(int)CS::NEUTRAL];

		_knob.pos = pos;
		_knob.radius = (int)radius;

		switch (radius) {
		case KR::SMALL:
		default:
			_knob.labelOV.font.size = (int)FS::MID;
			_knob.valueOV.font.size = (int)FS::SMALL;
			break;
		case KR::MID:
			_knob.labelOV.font.size = (int)FS::LARGE;
			_knob.valueOV.font.size = (int)FS::MID;
			break;
		case KR::LARGE:
			_knob.labelOV.font.size = (int)FS::EXTRA;
			_knob.valueOV.font.size = (int)FS::LARGE;
			break;
		}

		_knob.labelOV.pos.x = pos.x;
		_knob.labelOV.pos.y =
		    pos.y - (_knob.radius + _knob.labelOV.font.size);

		_knob.valueOV.pos.x = pos.x;
		_knob.valueOV.pos.y =
		    pos.y + (_knob.radius + _knob.valueOV.font.size);

		_knob.rotationLimits = rotLimits;
		_valueLimits = valLimits;

		_displayScale =
		    (rotLimits.y - rotLimits.x) / (valLimits.y - valLimits.x);

		_selected = false;
		_step = step;
	}

	virtual ~IKnob() {}

	virtual void processInput(Input *ip)
	{
		glm::ivec2 mousePos = ip->getMousePosition();
		if (!_selected) {
			if (_isOverlapping(mousePos)) {
				_knob.color = KnobColors[(int)CS::HOVERING];
				if (ip->isButtonPressed(SDL_BUTTON_LEFT)) {
					_selected = true;
					_knob.color =
					    KnobColors[(int)CS::SELECTED];
				}
			} else {
				_knob.color = KnobColors[(int)CS::NEUTRAL];
			}
		} else if (!_isOverlapping(mousePos) &&
			   ip->isButtonPressed(SDL_BUTTON_LEFT)) {
			_selected = false;
			_knob.color = KnobColors[(int)CS::NEUTRAL];
		}

		if (_selected) {
			if (_value > _valueLimits.x &&
			    (ip->getWheelDelta() < 0))
				_value -= _step;
			else if (_value < _valueLimits.y &&
				 (ip->getWheelDelta() > 0))
				_value += _step;

			_value =
			    std::clamp(_value, _valueLimits.x, _valueLimits.y);
		}
	}

	virtual void update()
	{
		// static U oldValue;

		// if (oldValue != _value) {
		float angle = ((_value - _valueLimits.x) * _displayScale +
			       _knob.rotationLimits.x) *
			      M_PI / 180;

		for (auto &p : _knob.pointArr) {
			int px = (int)(_knob.radius * cos(angle));
			int py = (int)(_knob.radius * sin(angle));
			p = {_knob.pos.x - px, _knob.pos.y - py};
			angle += 2 * M_PI / N_POINTS;
		}

		// oldValue = _value;
		_knob.valueOV.text = _valToString();
		//}
	}

	virtual void draw(Renderer *ren, float intp)
	{

		ren->drawCachedText(_knob.labelOV);
		ren->drawDynamicText(_knob.valueOV);

		SDL_SetRenderDrawColor(ren->getRenderer(), _knob.color.r,
				       _knob.color.g, _knob.color.b,
				       _knob.color.a);

		glm::ivec2 oldpoint = _knob.pos;
		for (const auto &p : _knob.pointArr) {

			glm::ivec2 newpoint = p;
			SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x,
					   oldpoint.y, newpoint.x, newpoint.y);
			oldpoint = newpoint;
		}
	}

	virtual const char *getLabel() const
	{
		return _knob.labelOV.text.c_str();
	}

	U getValue() const { return _value; }
};

/*template void IKnob::init<float>(const char *label, float value, glm::ivec2
pos, int radius, glm::vec2 limits);

template void IKnob::init<int>(const char *label, int value, glm::ivec2 pos,
			       int radius, glm::vec2 limits);
			       */

#endif
