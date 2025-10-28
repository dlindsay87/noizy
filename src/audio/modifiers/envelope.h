#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

#include "base_classes.h"
#include "knob.hpp"

class Envelope : public IModifier
{
      private:
	float _attack, _decay, _sustain, _release;

      public:
	Envelope() : _attack(0.5f), _decay(0.2f), _sustain(0.5f), _release(0.7f)
	{
		_controls.emplace_back(new FloatKnob("Attack", &_attack,
						     DS::SMALL, {0.0f, 4.0f},
						     {-60.0f, 180.0f}, 0.2f));
		_controls.emplace_back(new FloatKnob("Decay", &_decay,
						     DS::SMALL, {0.0f, 4.0f},
						     {-60.0f, 180.0f}, 0.2f));
		_controls.emplace_back(new FloatKnob("Sustain", &_sustain,
						     DS::SMALL, {0.0f, 1.0f},
						     {-60.0f, 180.0f}, 0.1f));
		_controls.emplace_back(new FloatKnob("Release", &_release,
						     DS::SMALL, {0.0f, 4.0f},
						     {-60.0f, 180.0f}, 0.2f));
	}

	void processInput(Input *ip) {}

	void draw(Renderer *ren, float intp) {}

	void applyState(ToneState &toneState);
	void applySample(Sample &s) {};
};

#endif
