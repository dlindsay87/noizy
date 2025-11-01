#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <SDL2/SDL.h>

#include "base_classes.h"
#include "knob.hpp"
#include "spin_box.h"
#include "wave_functions.hpp"

#include <cmath>
#include <map>

#ifndef N_KEYS
#define N_KEYS 18
#endif

#ifndef N_OCTAVES
#define N_OCTAVES 9
#endif

#ifndef A_FREQ
#define A_FREQ 440.0f
#endif

class Keyboard : public IGenerator
{
      private:
	const unsigned int _scanCodes[N_KEYS] = {
	    SDL_SCANCODE_A,	      // A4
	    SDL_SCANCODE_W,	      // Bb4
	    SDL_SCANCODE_S,	      // B4
	    SDL_SCANCODE_D,	      // C4
	    SDL_SCANCODE_R,	      // C#4
	    SDL_SCANCODE_F,	      // D4
	    SDL_SCANCODE_T,	      // Db4
	    SDL_SCANCODE_G,	      // E4
	    SDL_SCANCODE_H,	      // F4
	    SDL_SCANCODE_U,	      // F#4
	    SDL_SCANCODE_J,	      // G4
	    SDL_SCANCODE_I,	      // G#4
	    SDL_SCANCODE_K,	      // A5
	    SDL_SCANCODE_O,	      // Bb5
	    SDL_SCANCODE_L,	      // B5
	    SDL_SCANCODE_SEMICOLON,   // C5
	    SDL_SCANCODE_LEFTBRACKET, // C#5
	    SDL_SCANCODE_APOSTROPHE   // D5
	};

	ToneState _keyStates[N_KEYS];
	int _wave, _oldWave;
	int _octave, _oldOctave;

	float _intonate(int i);
	float (*_waveFunction)(float) = 0;

	void _applyIntonation();
	void _applyWave();

	float _advancePhase(float &phase, float amp, float freq);
	float _displayPhase(size_t idx, float amp, float freq);

      public:
	Keyboard(int wave = 0, int octave = 4);

	void processInput(Input *ip) override;
	void update() override
	{
		_applyIntonation();
		_applyWave();
	}
	void generate(Sample &s) override;
};

#endif
