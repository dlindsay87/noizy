#ifndef __BASE_SHAPES_H__
#define __BASE_SHAPES_H__

#include "renderer.h"

#ifndef S_POINTS
#define S_POINTS 4
#endif

#ifndef T_POINTS
#define T_POINTS 3
#endif

#ifndef K_POINTS
#define K_POINTS 16
#endif

class IShape
{
      protected:
	glm::ivec2 _pos, _scale;
	HashableColor _color;

      public:
	virtual bool isOverlapping(glm::ivec2 mousePos) const = 0;
	virtual void draw(Renderer *ren, float intp) = 0;

	virtual void setColor(HashableColor c) { _color = c; }
};

class Box : public IShape
{
      protected:
	glm::ivec2 _pointArr[S_POINTS];

      public:
	Box() {}

	virtual void init(glm::ivec2 pos, glm::ivec2 scale);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void draw(Renderer *ren, float intp);
};

class Triangle : public IShape
{
      protected:
	bool _downright;
	// int *_refVal;

	glm::ivec2 _pointArr[T_POINTS];

      public:
	virtual void init(glm::ivec2 pos, glm::ivec2 scale,
			  bool down); //, int *ref);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void draw(Renderer *ren, float intp);

	virtual int getOrientation() { return _downright ? -1 : 1; }
};

class Circle : public IShape
{
      protected:
	glm::ivec2 _pointArr[K_POINTS];

      public:
	virtual void init(glm::ivec2 pos, int radius);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void rotate(float angle);

	virtual void draw(Renderer *ren, float intp);
};

#endif
