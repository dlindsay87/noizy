#include "renderer.h"

Renderer::~Renderer()
{
	for (auto &[key, font] : _fontCache) {
		TTF_CloseFont(font);
	}
	TTF_Quit();

	for (auto &[key, texture] : _textCache) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyRenderer(_renderer);
}

void Renderer::init(SDL_Window *window, std::string font)

{
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!_renderer) {
		std::cerr << "SDL render creation failed: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}

	if (SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND) != 0) {
		std::cerr << "SDL blend mode setting failed: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}

	TTF_Init();
	_fontName = font;
}

SDL_Texture *Renderer::createTexture(TextOverlay ov)
{
	_TTF_Font *tFont = fetchCachedFont(ov.font);

	SDL_Surface *tSurface =
	    TTF_RenderText_Solid(tFont, ov.text.c_str(), ov.font.color);
	if (!tSurface) {
		std::cerr << "Failed to load surface: " << SDL_GetError()
			  << std::endl;
		SDL_FreeSurface(tSurface);
		exit(1);
	}

	SDL_Texture *texture =
	    SDL_CreateTextureFromSurface(_renderer, tSurface);
	if (!texture) {
		std::cerr << "Failed to load texture: " << SDL_GetError()
			  << std::endl;
		exit(1);
	}
	SDL_FreeSurface(tSurface);
	return texture;
}

_TTF_Font *Renderer::fetchCachedFont(Font font)
{
	_TTF_Font *tFont;
	auto it = _fontCache.find(font);
	if (it != _fontCache.end()) {
		tFont = it->second;
	} else {
		std::string path = "fonts/" + _fontName + ".ttf";
		tFont = TTF_OpenFont(path.c_str(), font.size);
		if (!tFont) {
			std::cerr << "Failed to load font: " << SDL_GetError()
				  << std::endl;
			TTF_CloseFont(tFont);
			exit(1);
		}
		_fontCache[font] = tFont; // Cache the font
	}
	return tFont;
}

SDL_Texture *Renderer::fetchCachedText(TextOverlay ov)
{
	SDL_Texture *texture;
	auto it = _textCache.find(ov);
	if (it != _textCache.end()) {
		texture = it->second;
	} else {
		texture = createTexture(ov);
		_textCache[ov] = texture; // Cache the texture
	}
	return texture;
}

void Renderer::drawCachedText(TextOverlay &ov)
{
	SDL_Texture *texture = fetchCachedText(ov);

	SDL_Rect _tRect;
	SDL_QueryTexture(texture, NULL, NULL, &_tRect.w, &_tRect.h);
	_tRect.x = ov.pos.x - _tRect.w / 2;
	_tRect.y = ov.pos.y - _tRect.h / 2;

	SDL_RenderCopy(_renderer, texture, NULL, &_tRect);
}

void Renderer::drawDynamicText(TextOverlay &ov)
{
	SDL_Texture *texture = createTexture(ov);

	SDL_Rect _tRect;
	SDL_QueryTexture(texture, NULL, NULL, &_tRect.w, &_tRect.h);
	_tRect.x = ov.pos.x - _tRect.w / 2;
	_tRect.y = ov.pos.y - _tRect.h / 2;

	SDL_RenderCopy(_renderer, texture, NULL, &_tRect);
	SDL_DestroyTexture(texture);
}
