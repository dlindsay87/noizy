#ifndef __KNOB_H__
#define __KNOB_H__

#include <iomanip>
#include <sstream>

#include "base_categories.h"
#include "base_control.h"
#include "base_shapes.h"

template <typename U> class IKnob : public IControl
{
      protected:
	TextOverlay labelOV;
	TextOverlay valueOV;
	Circle _knobShape;

	U *_valueRef;
	glm::vec<2, U, glm::lowp> _valueLimits;
	glm::vec2 _rotationLimits;
	float _displayScale;
	U _step;

	bool _selected;

	virtual void _rotateKnob()
	{
		U valDistance = *_valueRef - _valueLimits.x;
		float distScaled = valDistance * _displayScale;
		float scaleOffset = distScaled + _rotationLimits.x;

		_knobShape.rotate(scaleOffset);
	};

	virtual void _select(glm::ivec2 m_pos, bool is_clicked)
	{
		if (_selected) {
			_knobShape.setColor(ColorSelection[DC::SELECT_B]);
			if (!_knobShape.isOverlapping(m_pos) && is_clicked) {
				_knobShape.setColor(
				    ColorSelection[DC::NEUTRAL_W]);
				_selected = false;
			}
		} else if (_knobShape.isOverlapping(m_pos)) {
			_knobShape.setColor(ColorSelection[DC::HOVER_Y]);
			if (is_clicked) {
				_knobShape.setColor(
				    ColorSelection[DC::SELECT_B]);
				_selected = true;
			}
		} else {
			_knobShape.setColor(ColorSelection[DC::NEUTRAL_W]);
		}
	}

	virtual void _adjustValue(float w_delta)
	{
		if (*_valueRef > _valueLimits.x && w_delta < 0)
			*_valueRef -= _step;
		else if (*_valueRef < _valueLimits.y && w_delta > 0)
			*_valueRef += _step;
	}

	virtual std::string _setValueText() const = 0;

      public:
	virtual void init(const char *label, U *ref, int k_rad,
			  glm::vec<2, U, glm::lowp> v_lim, glm::vec2 r_lim,
			  U step)
	{
		_valueRef = ref;
		_valueLimits = v_lim;
		_rotationLimits = r_lim;
		_displayScale = (r_lim.y - r_lim.x) / (v_lim.y - v_lim.x);
		_step = step;
		_selected = false;

		_knobShape.init(k_rad);

		labelOV.init(label, k_rad, ColorSelection[DC::NEUTRAL_W]);

		valueOV.init(_setValueText(), (k_rad / 2),
			     ColorSelection[DC::NEUTRAL_W]);
	}

	virtual void processInput(Input *ip) override
	{
		_select(ip->getMousePosition(),
			ip->isButtonPressed(SDL_BUTTON_LEFT));

		if (_selected) {
			_adjustValue(ip->getWheelDelta());
		}
	}

	virtual void update() override
	{
		*_valueRef =
		    std::clamp(*_valueRef, _valueLimits.x, _valueLimits.y);
		_rotateKnob();

		valueOV.text = _setValueText();
	}

	virtual void draw(Renderer *ren, float intp) override
	{
		ren->drawCachedText(labelOV);
		ren->drawDynamicText(valueOV);

		_knobShape.draw(ren, intp);
	}

	virtual void setPosition(glm::ivec2 p) override
	{
		int unit_h = labelOV.font.size; // aka radius

		_knobShape.setPosition(p);
		labelOV.setPosition({p.x, p.y - 2 * unit_h});
		valueOV.setPosition({p.x, p.y + 3 * unit_h / 2});
	}

	virtual glm::ivec2 getSize(Renderer *ren) const
	{
		SDL_Texture *texture = ren->fetchCachedTexture(labelOV);

		SDL_Rect tRect;
		SDL_QueryTexture(texture, NULL, NULL, &tRect.w, &tRect.h);

		int height = valueOV.pos.y - labelOV.pos.y;
		height += (valueOV.font.size + labelOV.font.size) / 2;

		int k_width = _knobShape.getDiameter();
		int width = k_width >= tRect.w ? k_width : tRect.w;

		return {width, height};
	}
};

class FloatKnob : public IKnob<float>
{
      protected:
	std::string _setValueText() const override
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << *_valueRef;
		return ss.str();
	}
};

class IntKnob : public IKnob<int>
{
      protected:
	std::string _setValueText() const override
	{
		std::stringstream ss;
		ss << *_valueRef;
		return ss.str();
	}
};

template <typename U, typename... Args> class CatKnob : public IKnob<int>
{
      protected:
	const Category<U, Args...> *_arr;

	std::string _setValueText() const override
	{
		return _arr[*_valueRef].label;
	}

      public:
	void specialInit(const char *label, int *ref,
			 const Category<U, Args...> *arr, int k_rad, int v_lim,
			 glm::vec2 r_lim)
	{
		_arr = arr;
		init(label, ref, k_rad, {0, v_lim - 1}, r_lim, 1);
	}

	void draw(Renderer *ren, float intp) override
	{
		ren->drawCachedText(labelOV);
		ren->drawCachedText(valueOV);

		_knobShape.draw(ren, intp);
	}
};

#endif
