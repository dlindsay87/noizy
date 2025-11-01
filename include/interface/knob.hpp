#ifndef __KNOB_H__
#define __KNOB_H__

#include <iomanip>
#include <sstream>

#include "base_categories.h"
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

	virtual void _rotateKnob();

	virtual void _select(glm::ivec2 m_pos, bool is_clicked);

	virtual void _adjustValue(float w_delta);
	virtual std::string _setValueText() const = 0;

      public:
	IKnob() = default;

	virtual void init(const char *label, U *ref, int k_rad,
			  glm::vec<2, U, glm::lowp> v_lim, glm::vec2 r_lim,
			  U step);

	virtual void processInput(Input *ip) override;

	virtual void update() override;

	virtual void draw(Renderer *ren, float intp) override;

	virtual void setPosition(glm::ivec2 p) override;

	virtual glm::ivec2 getSize(Renderer *ren) const;
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

      public:
	FloatKnob() : IKnob<float>() {}

	FloatKnob(const char *label, float *ref, int k_rad, glm::vec2 v_lim,
		  glm::vec2 r_lim, float step)
	{
		init(label, ref, k_rad, v_lim, r_lim, step);
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

      public:
	IntKnob() : IKnob<int>() {}

	IntKnob(const char *label, int *ref, int k_rad, glm::vec2 v_lim,
		glm::ivec2 r_lim, int step)
	{
		init(label, ref, k_rad, v_lim, r_lim, step);
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
	CatKnob() : IKnob<int>() {}

	CatKnob(const char *label, int *ref, const Category<U, Args...> *arr,
		int k_rad, int v_lim, glm::vec2 r_lim)
	{
		specialInit(label, ref, arr, k_rad, v_lim, r_lim);
	}

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

template <typename U> void IKnob<U>::_rotateKnob()
{
	U valDistance = *_valueRef - _valueLimits.x;
	float distScaled = valDistance * _displayScale;
	float scaleOffset = distScaled + _rotationLimits.x;

	_knobShape.rotate(scaleOffset);
};

template <typename U> void IKnob<U>::_select(glm::ivec2 m_pos, bool is_clicked)
{
	if (_selected) {
		_knobShape.setColor(ColorSelection[DC::SELECT_B]);
		if (!_knobShape.isOverlapping(m_pos) && is_clicked) {
			_knobShape.setColor(ColorSelection[DC::NEUTRAL_W]);
			_selected = false;
		}
	} else if (_knobShape.isOverlapping(m_pos)) {
		_knobShape.setColor(ColorSelection[DC::HOVER_Y]);
		if (is_clicked) {
			_knobShape.setColor(ColorSelection[DC::SELECT_B]);
			_selected = true;
		}
	} else {
		_knobShape.setColor(ColorSelection[DC::NEUTRAL_W]);
	}
}

template <typename U> void IKnob<U>::_adjustValue(float w_delta)
{
	if (*_valueRef > _valueLimits.x && w_delta < 0)
		*_valueRef -= _step;
	else if (*_valueRef < _valueLimits.y && w_delta > 0)
		*_valueRef += _step;
}

template <typename U>
void IKnob<U>::init(const char *label, U *ref, int k_rad,
		    glm::vec<2, U, glm::lowp> v_lim, glm::vec2 r_lim, U step)
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

template <typename U> void IKnob<U>::processInput(Input *ip)
{
	_select(ip->getMousePosition(), ip->isButtonPressed(SDL_BUTTON_LEFT));

	if (_selected) {
		_adjustValue(ip->getWheelDelta());
	}
}

template <typename U> void IKnob<U>::update()
{
	*_valueRef = std::clamp(*_valueRef, _valueLimits.x, _valueLimits.y);
	_rotateKnob();

	valueOV.text = _setValueText();
}

template <typename U> void IKnob<U>::draw(Renderer *ren, float intp)
{
	ren->drawCachedText(labelOV);
	ren->drawDynamicText(valueOV);

	_knobShape.draw(ren, intp);
}

template <typename U> void IKnob<U>::setPosition(glm::ivec2 p)
{
	int unit_h = labelOV.font.size; // aka radius

	_knobShape.setPosition(p);
	labelOV.setPosition({p.x, p.y - 2 * unit_h});
	valueOV.setPosition({p.x, p.y + 3 * unit_h / 2});
}
template <typename U> glm::ivec2 IKnob<U>::getSize(Renderer *ren) const
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

#endif
