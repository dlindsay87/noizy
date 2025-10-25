#ifndef __LFO_H__
#define __LFO_H__

#include "base_classes.h"
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
			_waveFunction = WaveArray[_wave].waveFunction;
			_oldWave = _wave;
		}
	}

      public:
	LFO(int wave = 0)
	    : _depth(0.0f), _rate(0.0f), _phase(0.0f), _wave(wave),
	      _oldWave(wave)
	{
		_waveFunction = WaveArray[_wave].waveFunction;
	}

	virtual void processInput(Input *ip) {}
	virtual void applyState(ToneState &toneState) {}

	virtual void draw(Renderer *ren, float intp) {}

	virtual int &referenceWave() { return _wave; }
	virtual float &referenceDepth() { return _depth; }
	virtual float &referenceRate() { return _rate; }
};

class fLFO : public LFO
{
      private:
      public:
	void applySample(Sample &s)
	{
		_applyWave();

		int r = std::pow(2.0f, _rate - 1.0f);

		bool _active = _rate > 0.0f;
		s.freqMod = _active * _depth * _waveFunction(_phase);
		_phase = _active * fmod(_phase + _timeStep * r, 2.0 * M_PI);
	}
};

class aLFO : public LFO
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
