#ifndef __SDL_RENDERER_H__
#define __SDL_RENDERER_H__

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

#include <iostream>
#include <unordered_map>

#include "hash_util.hpp"

enum FontSize { SMALL = 12, MID = 36, LARGE = 72, EXTRA = 144 };

struct HashableColor : SDL_Color {
	bool operator==(const HashableColor &other) const noexcept
	{
		return (r == other.r && g == other.g && b == other.b &&
			a == other.a);
	}
};

struct Font {
	FontSize size;
	HashableColor color;

	bool operator==(const Font &other) const noexcept
	{
		return (size == other.size && color == other.color);
	}
};

struct TextOverlay {
	glm::ivec2 pos;

	std::string text;
	Font font;

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
		return HashUtil::hash_values(static_cast<int>(f.size), f.color);
	}
};

template <> struct hash<TextOverlay> {
	std::size_t operator()(const TextOverlay &ov) const noexcept
	{
		return HashUtil::hash_values(ov.text, ov.font);
	}
};

} // namespace std

class Renderer
{
      private:
	SDL_Renderer *_renderer;

	std::string _fontName; // assumes font is the same for everything

	std::unordered_map<Font, _TTF_Font *, std::hash<Font>> _fontCache{};

	std::unordered_map<TextOverlay, SDL_Texture *, std::hash<TextOverlay>>
	    _textCache{};

      public:
	Renderer() {}
	~Renderer();

	void init(SDL_Window *window, std::string font = "FeeeMono");

	void clear()
	{
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);
	}

	void present() { SDL_RenderPresent(_renderer); }

	SDL_Texture *createTexture(TextOverlay ov);

	_TTF_Font *fetchCachedFont(Font font);
	SDL_Texture *fetchCachedText(TextOverlay ov);

	void drawCachedText(TextOverlay &ov);
	void drawDynamicText(TextOverlay &ov);

	void updateText(TextOverlay &ov, std::string text) { ov.text = text; }
	void moveText(TextOverlay &ov, glm::ivec2 pos) { ov.pos = pos; }
	void recolorText(TextOverlay &ov, HashableColor c)
	{
		ov.font.color = c;
	}
	void resizeText(TextOverlay &ov, FontSize size) { ov.font.size = size; }

	SDL_Renderer *getRenderer() const { return _renderer; }
};

#endif
