#ifndef __BASE_CONTROLS_H__
#define __BASE_CONTROLS_H__

#include "input_manager.h"
#include "renderer.h"

class IControl
{
      public:
	virtual ~IControl() {}

	virtual void processInput(Input *ip) = 0;
	virtual void update() = 0;
	virtual void draw(Renderer *ren, float intp) = 0;

	virtual void setPosition(glm::ivec2 p) = 0;
	virtual glm::ivec2 getSize(Renderer *ren) const = 0;
};

#endif
