#ifndef __BASE_CLASSES_H__
#define __BASE_CLASSES_H__

#include <cmath>
#include <cstdlib>
#include <vector>

#include "input_manager.h"
#include "renderer.h"

struct Sample {
	float value;
	float displayValue;
	size_t idx;
};

enum EnvelopeState { Idle = 0, Attack, Decay, Sustain, Release, NUM_STATES };

struct ToneState {
	unsigned int id;
	float freq = 1.0f, amp = 0.0f;
	float phase = 0.0f, peak = 0.0f;

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
	virtual void apply(ToneState &ts) = 0;
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
