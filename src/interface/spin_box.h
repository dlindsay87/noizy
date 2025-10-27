#ifndef __SPIN_BOX_H__
#define __SPIN_BOX_H__

#include <iomanip>
#include <sstream>

#include "base_control.h"
#include "button.h"

class SpinBox : public IControl
{
      private:
	TextOverlay labelOV;
	TextOverlay valueOV;

	glm::ivec2 _valueLimits;
	int *_valueRef;

	TriButton<int> _upButton, _downButton;
	Box _box;

	virtual std::string _setValueText() const
	{
		std::stringstream ss;
		ss << *_valueRef;
		return ss.str();
	}

      public:
	void init(const char *label, int *ref, glm::ivec2 size,
		  glm::ivec2 v_lim)
	{
		_valueRef = ref;
		_valueLimits = v_lim;

		_box.init(size);
		_upButton.init(size, false, ref);
		_downButton.init(size, true, ref);

		labelOV.init(label, size.y, ColorSelection[DC::NEUTRAL_W]);

		valueOV.init(_setValueText(), size.y,
			     ColorSelection[DC::NEUTRAL_W]);
	}

	void processInput(Input *ip) override
	{
		_upButton.processInput(ip);
		_downButton.processInput(ip);
		*_valueRef =
		    std::clamp(*_valueRef, _valueLimits.x, _valueLimits.y);
	}

	virtual void update() override
	{
		*_valueRef =
		    std::clamp(*_valueRef, _valueLimits.x, _valueLimits.y);

		valueOV.text = _setValueText();
	}

	void draw(Renderer *ren, float intp) override
	{
		ren->drawCachedText(labelOV);
		ren->drawDynamicText(valueOV);

		_box.draw(ren, intp);
		_downButton.draw(ren, intp);
		_upButton.draw(ren, intp);
	}

	virtual void setPosition(glm::ivec2 p) override
	{
		int unit_h = labelOV.font.size;

		_box.setPosition(p);
		_upButton.setPosition({p.x, p.y - unit_h});
		_downButton.setPosition({p.x, p.y + unit_h});

		labelOV.setPosition({p.x, p.y - 3 * unit_h});
		valueOV.setPosition(p);
	}

	glm::ivec2 getSize(Renderer *ren) const override
	{
		SDL_Texture *texture = ren->fetchCachedTexture(labelOV);

		SDL_Rect tRect;
		SDL_QueryTexture(texture, NULL, NULL, &tRect.w, &tRect.h);

		int height = valueOV.pos.y - labelOV.pos.y;
		height += (tRect.h + _box.getShape().y) / 2;

		int b_width = _box.getShape().x;
		int width = b_width >= tRect.w ? b_width : tRect.w;

		return {width, height};
	}
};

#endif
