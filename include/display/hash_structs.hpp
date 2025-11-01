#ifndef __HASH_STRUCTS_HPP__
#define __HASH_STRUCTS_HPP__

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include <iostream>
#include <unordered_map>

#include "hash_util.hpp"

typedef struct HashableColor : SDL_Color {
	bool operator==(const HashableColor &other) const noexcept
	{
		return (r == other.r && g == other.g && b == other.b &&
			a == other.a);
	}
} HC;

typedef enum DisplaySize { TINY = 10, SMALL = 20, MID = 40, LARGE = 80 } DS;
typedef enum DisplayColor {
	NEUTRAL_W = 0,
	ERROR_R,
	HOVER_Y,
	WAVE_G,
	MUTED_G,
	SELECT_B,
	BLACK,
	NUM_COLORS
} DC;

inline HC ColorSelection[DC::NUM_COLORS] = {
    {255, 255, 255, 255}, // NEUTRAL_W
    {255, 0, 0, 255},	  // ERROR_R
    {255, 255, 0, 255},	  // HOVER_Y
    {0, 255, 0, 255},	  // WAVE_G
    {31, 63, 55, 255},	  // MUTED_G
    {0, 0, 255, 255},	  // SELECT_B
    {0, 0, 0, 255},	  // BLACK
};

struct Font {
	int size;
	HC color;

	bool operator==(const Font &other) const noexcept
	{
		return (size == other.size && color == other.color);
	}
};

struct TextOverlay {
	std::string text;
	Font font;

	glm::ivec2 pos{0, 0};

	void init(std::string t, int s, HC c)
	{
		this->text = t;
		this->font.size = s;
		this->font.color = c;
	}

	void setPosition(glm::ivec2 p) { this->pos = p; }

	bool operator==(const TextOverlay &other) const noexcept
	{
		return (text == other.text && font == other.font);
	}
};

namespace std
{

template <> struct hash<HashableColor> {
	std::size_t operator()(const HashableColor &c) const noexcept
	{
		return HashUtil::hash_values(c.r, c.g, c.b, c.a);
	}
};

template <> struct hash<Font> {
	std::size_t operator()(const Font &f) const noexcept
	{
		return HashUtil::hash_values(f.size, f.color);
	}
};

template <> struct hash<TextOverlay> {
	std::size_t operator()(const TextOverlay &ov) const noexcept
	{
		return HashUtil::hash_values(ov.text, ov.font);
	}
};

} // namespace std

#endif
