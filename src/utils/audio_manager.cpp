#include "audio_manager.h"

void callbackWrapper(void *userdata, Uint8 *stream, int len)
{
	auto *audioSystem = static_cast<AudioManager *>(userdata);
	audioSystem->audioCallback(stream);
}

AudioManager::AudioManager(float volume, int sampleRate, int samples)
    : _masterVolume(volume), _sampleRate(sampleRate), _samples(samples)
{
	_displayBuffer = new int16_t[_samples];
}

AudioManager::~AudioManager()
{
	SDL_CloseAudioDevice(_audioDevice);
	/*for (auto *g : _generators) {
		delete g;
		g = nullptr;
	}*/
	_generators.clear();
	delete[] _displayBuffer;
}

void AudioManager::_calcSample(size_t i)
{
	Sample s = {0.0f, 0.0f, i};
	for (auto &g : _generators) {
		g->generate(s);
	}

	_displayBuffer[i] = static_cast<int16_t>(
	    std::clamp(s.displayValue * _masterVolume, -1.0f, 1.0f) *
	    INT16_MAX);

	_buffer[i] = static_cast<int16_t>(
	    std::clamp(s.value * _masterVolume, -1.0f, 1.0f) * INT16_MAX);
}

void AudioManager::init()
{
	// Set up the SDL audio spec
	_spec.freq = _sampleRate;
	_spec.format = AUDIO_S16SYS;
	_spec.channels = 1;	  // Mono audio
	_spec.samples = _samples; // Buffer size
	_spec.callback = callbackWrapper;
	_spec.userdata = this;

	// Open audio device
	_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &_spec, nullptr, 0);

	if (!_audioDevice) {
		throw std::runtime_error("Failed to open audio device: " +
					 std::string(SDL_GetError()));
	}

	std::cout << "Audio Device initiated." << std::endl;

	// Start playback
	SDL_PauseAudioDevice(_audioDevice, 0);
}

void AudioManager::addGenerator(IGenerator *generator)
{
	generator->init(_sampleRate);
	_generators.push_back(generator);
}

void AudioManager::audioCallback(Uint8 *stream)
{
	_buffer = reinterpret_cast<int16_t *>(stream);
	for (int i = 0; i < _samples; ++i) {
		_calcSample(i);
	}
}
