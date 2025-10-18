#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include "base_classes.h"

void callbackWrapper(void *userdata, Uint8 *stream, int len);

class AudioManager
{
      private:
	SDL_AudioSpec _spec;
	SDL_AudioDeviceID _audioDevice = 0;

	float _masterVolume;
	const int _sampleRate, _samples;

	int16_t *_buffer = nullptr;
	int16_t *_displayBuffer = nullptr;

	std::vector<IGenerator *> _generators;

	void _calcSample(size_t i);

      public:
	AudioManager(float volume = 0.5f, int sampleRate = 48000,
		     int samples = 1024);
	~AudioManager();

	void init();

	void addGenerator(IGenerator *generator);

	void playTone(uint s) { SDL_PauseAudioDevice(_audioDevice, s > 0); }

	void audioCallback(Uint8 *stream);

	const int16_t *getDisplayBuffer() const { return _displayBuffer; }
	size_t getBufferLen() const { return static_cast<size_t>(_samples); }
};

#endif
