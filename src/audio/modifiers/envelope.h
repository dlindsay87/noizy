#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

#include "base_classes.h"

class Envelope : public IModifier
{
      private:
	float _attack, _decay, _sustain, _release;

      public:
	Envelope() : _attack(0.5f), _decay(0.2f), _sustain(0.5f), _release(0.7f)
	{
	}

	void processInput(Input *ip) {}

	void draw(Renderer *ren, float intp) {}

	void apply(ToneState &toneState);

	float &referenceAttack() { return _attack; }
	float &referenceDecay() { return _decay; }
	float &referenceSustain() { return _sustain; }
	float &referenceRelease() { return _release; }
};

#endif
