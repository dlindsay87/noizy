#ifndef __BASE_CLASSES_H__
#define __BASE_CLASSES_H__

#include <cmath>
#include <cstdlib>
#include <vector>

#include "input_manager.h"
#include "renderer.h"

struct Sample {
	float value, displayValue;
	float freqMod, ampMod;

	size_t idx;
};

enum EnvelopeState { Idle = 0, Attack, Decay, Sustain, Release };

struct ToneState {
	unsigned int id;
	size_t idx;

	float amp, peak;
	float freq, phase;

	EnvelopeState state;
};

class IComponent
{
      protected:
	int _sampleRate;
	float _timeStep;

      public:
	virtual ~IComponent() {}

	virtual void init(int sampleRate)
	{
		_sampleRate = sampleRate;
		_timeStep = 2.0f * M_PI / _sampleRate;
	}

	virtual void processInput(Input *ip) = 0;

	virtual void draw(Renderer *ren, float intp) = 0;
};

class IModifier : public IComponent
{
      public:
	virtual void applyState(ToneState &ts) = 0;
	virtual void applySample(Sample &s) = 0;
};

class IGenerator : public IComponent
{
      protected:
	std::vector<IModifier *> _modifiers;

      public:
	virtual ~IGenerator()
	{
		/*for (auto *m : _modifiers) {
			delete m;
			m = nullptr;
		}*/
		_modifiers.clear();
	}

	virtual void generate(Sample &s) = 0;

	virtual void addModifier(IModifier *modifier)
	{
		modifier->init(_sampleRate);
		_modifiers.push_back(modifier);
	}
};

#endif
