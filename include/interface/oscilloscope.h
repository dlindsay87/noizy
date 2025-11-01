#ifndef __OSCILLOSCOPE_H__
#define __OSCILLOSCOPE_H__

#include "renderer.h"

class Oscilloscope
{
      private:
	glm::ivec2 _pos;
	glm::ivec2 _shape;
	float _scale;

	SDL_Rect _boundBox;

	HashableColor _wavCol = ColorSelection[DC::WAVE_G];
	HashableColor _bgCol = ColorSelection[DC::MUTED_G];
	HashableColor _borCol = ColorSelection[DC::NEUTRAL_W];

	int16_t *oldDisplay;

	int _interpolateAndCenter(int16_t &y_o, const int16_t y_n, float intp);

      public:
	Oscilloscope() {}
	~Oscilloscope() { delete[] oldDisplay; }

	void init(glm::ivec2 pos, glm::ivec2 shape, size_t samples);

	void draw(const int16_t *buffer, Renderer *ren, float intp);
};

#endif
