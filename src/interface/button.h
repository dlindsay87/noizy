#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "base_shapes.h"

template <typename U> class TriButton
{
      private:
	Triangle _shape;
	U *_refValue;

      public:
	TriButton() {}

	void init(glm::ivec2 pos, glm::ivec2 scale, bool down, U *ref)
	{
		_refValue = ref;
		_shape.init(pos, scale, down);
	}

	virtual void processInput(Input *ip)
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

	virtual void draw(Renderer *ren, float intp) { _shape.draw(ren, intp); }
};

#endif
