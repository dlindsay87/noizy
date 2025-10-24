#ifndef __KNOB_H__
#define __KNOB_H__

#include <iomanip>
#include <sstream>

#include "base_shapes.h"
#include "input_manager.h"

template <typename U> class IKnob
{
      protected:
	TextOverlay labelOV;
	TextOverlay valueOV;
	Circle _knob;

	U *_valueRef;
	glm::vec<2, U, glm::lowp> _valueLimits;
	glm::vec2 _rotationLimits;
	float _displayScale;
	float _step;

	bool _selected;

	virtual std::string _valToString() const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << *_valueRef;
		return ss.str();
	}

	virtual float _calcRotation()
	{
		return ((*_valueRef - _valueLimits.x) * _displayScale +
			_rotationLimits.x) *
		       M_PI / 180;
	}

      public:
	IKnob() {}

	IKnob(const char *label, U *ref, glm::ivec2 pos, DisplaySize radius,
	      glm::vec2 rotLimits, glm::vec<2, U, glm::lowp> valLimits,
	      float step)
	{
		init(label, ref, pos, radius, rotLimits, valLimits, step);
	}

	void init(const char *label, U *ref, glm::ivec2 pos, DisplaySize radius,
		  glm::vec2 rotLimits, glm::vec<2, U, glm::lowp> valLimits,
		  float step)
	{
		_valueRef = ref;
		_valueLimits = valLimits;
		_rotationLimits = rotLimits;
		_displayScale =
		    (rotLimits.y - rotLimits.x) / (valLimits.y - valLimits.x);
		_step = step;
		_selected = false;

		_knob.init(pos, radius);

		labelOV.text = label;
		labelOV.font.color = ColorSelection[DC::NEUTRAL_W];
		labelOV.font.size = radius;
		labelOV.pos.x = pos.x;
		labelOV.pos.y = pos.y - (radius + labelOV.font.size);

		valueOV.font.color = ColorSelection[DC::NEUTRAL_W];
		valueOV.font.size = radius / 2;
		valueOV.pos.x = pos.x;
		valueOV.pos.y = pos.y + (radius + valueOV.font.size);
	}

	virtual ~IKnob() {}

	virtual void processInput(Input *ip)
	{
		glm::ivec2 mousePos = ip->getMousePosition();

		if (!_selected) {
			if (_knob.isOverlapping(mousePos)) {
				_knob.setColor(ColorSelection[DC::HOVER_Y]);
				if (ip->isButtonPressed(SDL_BUTTON_LEFT)) {
					_selected = true;
					_knob.setColor(
					    ColorSelection[DC::SELECT_B]);
				}
			} else {
				_knob.setColor(ColorSelection[DC::NEUTRAL_W]);
			}
		} else if (!_knob.isOverlapping(mousePos) &&
			   ip->isButtonPressed(SDL_BUTTON_LEFT)) {
			_selected = false;
			_knob.setColor(ColorSelection[DC::NEUTRAL_W]);
		}

		if (_selected) {
			if (*_valueRef > _valueLimits.x &&
			    (ip->getWheelDelta() < 0))
				*_valueRef -= _step;
			else if (*_valueRef < _valueLimits.y &&
				 (ip->getWheelDelta() > 0))
				*_valueRef += _step;
		}
		*_valueRef =
		    std::clamp(*_valueRef, _valueLimits.x, _valueLimits.y);
	}

	virtual void applyCat(const char *label) { valueOV.text = label; }
	virtual void applyNum() { valueOV.text = _valToString(); }

	virtual void draw(Renderer *ren, float intp)
	{
		ren->drawCachedText(labelOV);
		ren->drawDynamicText(valueOV);

		_knob.rotate(_calcRotation());
		_knob.draw(ren, intp);
	}

	virtual const char *getLabel() const { return labelOV.text.c_str(); }

	U getValue() const { return *_valueRef; }
};

/*template void IKnob::init<float>(const char *label, float value, glm::ivec2
pos, int radius, glm::vec2 limits);

template void IKnob::init<int>(const char *label, int value, glm::ivec2 pos,
			       int radius, glm::vec2 limits);
			       */

#endif
