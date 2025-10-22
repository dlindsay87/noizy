#ifndef __SPIN_BOX_H__
#define __SPIN_BOX_H__

#include <iomanip>
#include <sstream>

#include "button.h"

class SpinBox
{
      private:
	TextOverlay labelOV;
	TextOverlay valueOV;

	glm::ivec2 _valueLimits;
	int _value;

	TriButton<int> _upButton, _downButton;
	Box _box;

	virtual std::string _valToString() const
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << _value;
		return ss.str();
	}

      public:
	SpinBox() {}

	void init(const char *label, int val, glm::ivec2 limits, glm::ivec2 pos,
		  glm::ivec2 scale)
	{
		_box.init(pos, scale);
		_upButton.init({pos.x, pos.y - scale.y}, scale, false, &_value);
		_downButton.init({pos.x, pos.y + scale.y}, scale, true,
				 &_value);

		_valueLimits = limits;
		_value = val;

		labelOV.text = label;
		labelOV.font.color = ColorSelection[DC::NEUTRAL_W];
		labelOV.font.size = DS::MID;
		labelOV.pos.x = pos.x;
		labelOV.pos.y = pos.y - 3 * scale.y;

		valueOV.text = _valToString();
		valueOV.font.color = ColorSelection[DC::NEUTRAL_W];
		valueOV.font.size = DS::SMALL;
		valueOV.pos = pos;
	}

	void processInput(Input *ip)
	{
		_upButton.processInput(ip);
		_downButton.processInput(ip);
		_value = std::clamp(_value, _valueLimits.x, _valueLimits.y);
	}

	void update() { valueOV.text = _valToString(); }

	void draw(Renderer *ren, float intp)
	{
		ren->drawCachedText(labelOV);
		ren->drawDynamicText(valueOV);

		_box.draw(ren, intp);
		_downButton.draw(ren, intp);
		_upButton.draw(ren, intp);
	}

	int getValue() { return _value; }
};

#endif
