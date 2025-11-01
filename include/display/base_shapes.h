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
	glm::ivec2 _pos{0, 0};
	glm::ivec2 _shape;
	HashableColor _color;

      public:
	virtual bool isOverlapping(glm::ivec2 mousePos) const = 0;
	virtual void draw(Renderer *ren, float intp) = 0;

	virtual void setColor(HashableColor c) { _color = c; }
	virtual void setPosition(glm::ivec2 p) { _pos = p; }

	virtual glm::ivec2 getShape() const { return _shape; }
};

class Box : public IShape
{
      protected:
	glm::ivec2 _pointArr[S_POINTS];

      public:
	Box() {}

	virtual void init(glm::ivec2 shape);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void draw(Renderer *ren, float intp);
};

class Triangle : public IShape
{
      protected:
	bool _downright;

	glm::ivec2 _pointArr[T_POINTS];

      public:
	virtual void init(glm::ivec2 shape, bool down);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void draw(Renderer *ren, float intp);

	virtual int getOrientation() { return _downright ? -1 : 1; }
};

class Circle : public IShape
{
      protected:
	glm::ivec2 _pointArr[K_POINTS];

      public:
	virtual void init(int radius);

	virtual bool isOverlapping(glm::ivec2 mousePos) const;

	virtual void rotate(float angle);

	virtual void draw(Renderer *ren, float intp);

	int getDiameter() const { return _shape.x * 2; }
};

#endif
