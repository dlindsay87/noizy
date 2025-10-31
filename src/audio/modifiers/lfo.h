#ifndef __LFO_H__
#define __LFO_H__

#include "base_classes.h"
#include "knob.hpp"
#include "wave_functions.hpp"

class LFO : public IModifier
{
      protected:
	float _depth, _rate;
	float _phase;

	int _wave, _oldWave;
	float (*_waveFunction)(float) = 0;

	virtual void _applyWave()
	{
		if (_wave != _oldWave) {
			_waveFunction = WaveArray[_wave].fn;
			_oldWave = _wave;
		}
	}

      public:
	LFO(int wave = 0)
	    : _depth(0.0f), _rate(0.0f), _phase(0.0f), _wave(wave),
	      _oldWave(wave)
	{
		_waveFunction = WaveArray[_wave].fn;

		_controls.emplace_back(
		    new CatKnob("Wave", &_wave, WaveArray, DS::SMALL,
				WaveForm::NUM_WAVES, {-60.0f, 180.0f}));
		_controls.emplace_back(new FloatKnob("Depth", &_depth,
						     DS::SMALL, {-8.0f, 8.0f},
						     {-60.0f, 240.0f}, 0.2f));
		_controls.emplace_back(new FloatKnob(
		    "Rate", &_rate, DS::SMALL, {0, 16}, {-60.0f, 180.0f}, 1));
	}

	virtual void processInput(Input *ip) {}
	virtual void applyState(ToneState &toneState) {}

	virtual void draw(Renderer *ren, float intp) {}
};

class FreqLFO : public LFO
{
      private:
	void applySample(Sample &s)
	{
		_applyWave();

		int r = std::pow(2.0f, _rate - 1.0f);

		bool _active = _rate > 0.0f;
		s.freqMod = _active * _depth * _waveFunction(_phase);
		_phase = _active * fmod(_phase + _timeStep * r, 2.0 * M_PI);
	}
};

class AmpLFO : public LFO
{
      private:
      public:
	void applySample(Sample &s)
	{
		_applyWave();

		int r = std::pow(2.0f, _rate - 1.0f);

		bool _active = _rate > 0.0f;
		s.ampMod = _active * _depth * _waveFunction(_phase);
		_phase = _active * fmod(_phase + _timeStep * r, 2.0 * M_PI);
	}
};

#endif
