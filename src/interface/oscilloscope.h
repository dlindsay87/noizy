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

	glm::u8vec4 _wavCol = {0, 255, 0, 255};
	glm::u8vec4 _bgCol = {31, 63, 55, 255};
	glm::u8vec4 _borCol = {255, 255, 255, 255};

	int16_t *oldDisplay;

	int _interpolateAndCenter(int16_t &y_o, const int16_t y_n, float intp)
	{
		int16_t y_intp = static_cast<int16_t>(y_o + (y_n - y_o) * intp);
		y_o = y_n;
		return _pos.y - y_intp * _shape.y / 2 / INT16_MAX;
	}

      public:
	Oscilloscope() {}
	~Oscilloscope() { delete[] oldDisplay; }

	void init(glm::ivec2 pos, glm::ivec2 shape, size_t samples)
	{
		_pos = pos;
		_shape = shape;
		_boundBox = {_pos.x, _pos.y - _shape.y / 2, _shape.x, _shape.y};

		_scale = samples / (float)_shape.x;

		oldDisplay = new int16_t[_shape.x];
		// oldDisplay[0] = 0;
	}

	void draw(const int16_t *buffer, Renderer *ren, float intp)
	{
		// Draw background and bounding box
		SDL_SetRenderDrawColor(ren->getRenderer(), _bgCol.r, _bgCol.g,
				       _bgCol.b, _bgCol.a);
		SDL_RenderFillRect(ren->getRenderer(), &_boundBox);

		// Draw the waveform
		SDL_SetRenderDrawColor(ren->getRenderer(), _wavCol.r, _wavCol.g,
				       _wavCol.b, _wavCol.a);

		int16_t prevY =
		    _interpolateAndCenter(oldDisplay[0], buffer[0], intp);

		for (int i = 1; i < _shape.x; ++i) {
			size_t scaled_idx = static_cast<size_t>(i * _scale);

			int16_t thisY = _interpolateAndCenter(
			    oldDisplay[i], buffer[scaled_idx], intp);

			SDL_RenderDrawLine(ren->getRenderer(), _pos.x + i - 1,
					   prevY, _pos.x + i, thisY);

			prevY = thisY;
		}

		SDL_SetRenderDrawColor(ren->getRenderer(), _borCol.r, _borCol.g,
				       _borCol.b, _borCol.a);
		SDL_RenderDrawRect(ren->getRenderer(), &_boundBox);
	}
};

#endif
