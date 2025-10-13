#ifndef __AUDIO_BASE_CLASSES_H__
#define __AUDIO_BASE_CLASSES_H__

#include <cstdlib>

struct Sample {
	float value;
	float displayValue;
	size_t idx;
};

enum struct EnvelopeState { Idle, Attack, Decay, Sustain, Release };

struct ToneState {
	unsigned int id;
	float freq = 1.0f, amp = 0.0f;
	float phase = 0.0f, peak = 0.0f;

	EnvelopeState state;
};

class IGenerator
{
      protected:
	float _timeStep;

      public:
	virtual void init(int sample_rate)
	{
		_timeStep = 2.0f * M_PI / sample_rate;
	}

	virtual void generate(Sample &s) = 0;
};

#endif