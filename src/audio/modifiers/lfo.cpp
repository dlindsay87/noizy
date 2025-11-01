#include "lfo.h"

void LFO::_applyWave()
{
	if (_wave != _oldWave) {
		_waveFunction = WaveArray[_wave].fn;
		_oldWave = _wave;
	}
}

LFO::LFO(int wave)
    : _depth(0.0f), _rate(0.0f), _phase(0.0f), _wave(wave), _oldWave(wave)
{
	_waveFunction = WaveArray[_wave].fn;

	_controls.emplace_back(new CatKnob("Wave", &_wave, WaveArray, DS::SMALL,
					   WaveForm::NUM_WAVES,
					   {-60.0f, 180.0f}));
	_controls.emplace_back(new FloatKnob("Depth", &_depth, DS::SMALL,
					     {-8.0f, 8.0f}, {-60.0f, 240.0f},
					     0.2f));
	_controls.emplace_back(new FloatKnob("Rate", &_rate, DS::SMALL, {0, 16},
					     {-60.0f, 180.0f}, 1));
}

void FreqLFO::applySample(Sample &s)
{
	int r = std::pow(2.0f, _rate - 1.0f);

	bool _active = _rate > 0.0f;
	s.freqMod = _active * _depth * _waveFunction(_phase);
	_phase = _active * fmod(_phase + _timeStep * r, 2.0 * M_PI);
}

void AmpLFO::applySample(Sample &s)
{
	int r = std::pow(2.0f, _rate - 1.0f);

	bool _active = _rate > 0.0f;
	s.ampMod = _active * _depth * _waveFunction(_phase);
	_phase = _active * fmod(_phase + _timeStep * r, 2.0 * M_PI);
}