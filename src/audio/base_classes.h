#ifndef __BASE_CLASSES_H__
#define __BASE_CLASSES_H__

#include <algorithm>
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

class IControl
{
      public:
	virtual ~IControl() {}

	virtual void processInput(Input *ip) = 0;
	virtual void update() = 0;
	virtual void draw(Renderer *ren, float intp) = 0;

	virtual void setPosition(glm::ivec2 p) = 0;
	virtual glm::ivec2 getSize(Renderer *ren) const = 0;
};

class IComponent
{
      protected:
	int _sampleRate;
	float _timeStep;
	std::vector<IControl *> _controls;

      public:
	virtual ~IComponent() { _controls.clear(); }

	virtual void init(int sampleRate)
	{
		_sampleRate = sampleRate;
		_timeStep = 2.0f * M_PI / _sampleRate;
	}

	virtual void processInput(Input *ip) = 0;

	virtual void draw(Renderer *ren, float intp) = 0;
	std::vector<IControl *> &getControls() { return _controls; }
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
