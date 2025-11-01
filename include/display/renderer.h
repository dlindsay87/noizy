#ifndef __SDL_RENDERER_H__
#define __SDL_RENDERER_H__

#include "hash_structs.hpp"

class Renderer
{
      private:
	SDL_Renderer *_renderer;

	std::string _fontName; // assumes font is the same for everything

	std::unordered_map<Font, _TTF_Font *, std::hash<Font>> _fontCache{};

	std::unordered_map<TextOverlay, SDL_Texture *, std::hash<TextOverlay>>
	    _textCache{};

	HashableColor _cc = ColorSelection[DC::BLACK];

      public:
	Renderer() {}
	~Renderer();

	void init(SDL_Window *window, std::string font = "FreeMono");

	void clear()
	{
		SDL_SetRenderDrawColor(_renderer, _cc.r, _cc.g, _cc.b, _cc.a);
		SDL_RenderClear(_renderer);
	}

	void present() { SDL_RenderPresent(_renderer); }

	_TTF_Font *createFont(Font font);
	_TTF_Font *fetchCachedFont(Font font);

	SDL_Texture *createTexture(TextOverlay ov);
	SDL_Texture *fetchCachedTexture(TextOverlay ov);

	void drawCachedText(TextOverlay &ov);
	void drawDynamicText(TextOverlay &ov);

	void updateText(TextOverlay &ov, std::string text) { ov.text = text; }
	void moveText(TextOverlay &ov, glm::ivec2 pos) { ov.pos = pos; }
	void recolorText(TextOverlay &ov, HashableColor c)
	{
		ov.font.color = c;
	}
	void resizeText(TextOverlay &ov, DisplaySize size)
	{
		ov.font.size = size;
	}

	SDL_Renderer *getRenderer() const { return _renderer; }
};

#endif
