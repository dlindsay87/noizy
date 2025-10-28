#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "base_classes.h"
#include "base_shapes.h"

template <typename U> class TriButton : public IControl
{
      protected:
	Triangle _shape;
	U *_refValue;

      public:
	void init(glm::ivec2 scale, bool down, U *ref)
	{
		_refValue = ref;
		_shape.init(scale, down);
	}

	virtual void processInput(Input *ip) override
	{
		glm::ivec2 mousePos = ip->getMousePosition();

		if (_shape.isOverlapping(mousePos)) {
			_shape.setColor(ColorSelection[DC::HOVER_Y]);
			if (ip->isButtonPressed(SDL_BUTTON_LEFT)) {
				_shape.setColor(ColorSelection[DC::SELECT_B]);
				*_refValue += _shape.getOrientation();
			}
		} else {
			_shape.setColor(ColorSelection[DC::NEUTRAL_W]);
		}
	}

	virtual void update() override {}

	virtual void draw(Renderer *ren, float intp) override
	{
		_shape.draw(ren, intp);
	}

	virtual void setPosition(glm::ivec2 p) override
	{
		_shape.setPosition(p);
	}

	virtual glm::ivec2 getSize(Renderer *ren) const override
	{
		return _shape.getShape();
	}
};

#endif
