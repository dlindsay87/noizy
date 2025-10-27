#include "base_shapes.h"

void Box::init(glm::ivec2 shape)
{
	_shape = shape;
	_color = ColorSelection[DC::NEUTRAL_W];

	_pointArr[0] = {shape.x / 2, shape.y / 2};
	_pointArr[1] = {shape.x / 2, -shape.y / 2};
	_pointArr[2] = {-shape.x / 2, -shape.y / 2};
	_pointArr[3] = {-shape.x / 2, shape.y / 2};
}

bool Box::isOverlapping(glm::ivec2 mousePos) const
{
	bool within_x = std::abs(mousePos.x - _pos.x) <= _shape.x / 2;

	bool within_y = std::abs(mousePos.y - _pos.y) <= _shape.y / 2;

	return within_x && within_y;
}

void Box::draw(Renderer *ren, float intp)
{
	SDL_SetRenderDrawColor(ren->getRenderer(), _color.r, _color.g, _color.b,
			       _color.a);

	glm::ivec2 oldpoint = _pointArr[S_POINTS - 1] + _pos;
	for (size_t i = 0; i < S_POINTS; ++i) {

		glm::ivec2 newpoint = _pointArr[i] + _pos;
		SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x, oldpoint.y,
				   newpoint.x, newpoint.y);
		oldpoint = newpoint;
	}
}

void Triangle::init(glm::ivec2 shape, bool down)
{
	_shape = shape;
	_downright = down;

	int dir = _downright ? 1 : -1; // 1 = down, -1 = up
	_pointArr[0] = glm::ivec2(-shape.x / 2, 0);
	_pointArr[1] = glm::ivec2(shape.x / 2, 0);
	_pointArr[2] = glm::ivec2(0, dir * shape.y);
}

bool Triangle::isOverlapping(glm::ivec2 mousePos) const
{
	auto p = glm::vec2(mousePos);
	auto a = glm::vec2(_pointArr[0] + _pos);
	auto b = glm::vec2(_pointArr[1] + _pos);
	auto c = glm::vec2(_pointArr[2] + _pos);

	// Compute barycentric coordinates
	glm::vec2 v0 = b - a;
	glm::vec2 v1 = c - a;
	glm::vec2 v2 = p - a;

	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	if (denom == 0.0f)
		return false; // degenerate triangle

	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	// Inside if all barycentric coordinates are between 0 and 1
	return (u >= 0.0f && v >= 0.0f && w >= 0.0f);
}

void Triangle::draw(Renderer *ren, float intp)
{
	SDL_SetRenderDrawColor(ren->getRenderer(), _color.r, _color.g, _color.b,
			       _color.a);

	glm::ivec2 oldpoint = _pointArr[T_POINTS - 1] + _pos;
	for (size_t i = 0; i < T_POINTS; ++i) {

		glm::ivec2 newpoint = _pointArr[i] + _pos;
		SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x, oldpoint.y,
				   newpoint.x, newpoint.y);
		oldpoint = newpoint;
	}
}

void Circle::init(int radius)
{
	_shape = glm::ivec2(radius);
	rotate(0.0f);
}

bool Circle::isOverlapping(glm::ivec2 mousePos) const
{
	return sqrt(pow(mousePos.x - _pos.x, 2) +
		    pow(mousePos.y - _pos.y, 2)) <= _shape.x;
}

void Circle::rotate(float angle)
{
	angle *= M_PI / 180;
	for (auto &p : _pointArr) {
		int px = static_cast<int>(_shape.x * cos(angle));
		int py = static_cast<int>(_shape.x * sin(angle));
		p = {-px, -py};
		angle += 2 * M_PI / K_POINTS;
	}
}

void Circle::draw(Renderer *ren, float intp)
{
	SDL_SetRenderDrawColor(ren->getRenderer(), _color.r, _color.g, _color.b,
			       _color.a);

	glm::ivec2 oldpoint = _pos;
	for (const auto &p : _pointArr) {

		glm::ivec2 newpoint = p + _pos;
		SDL_RenderDrawLine(ren->getRenderer(), oldpoint.x, oldpoint.y,
				   newpoint.x, newpoint.y);
		oldpoint = newpoint;
	}
}