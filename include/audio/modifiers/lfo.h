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

	virtual void _applyWave();

      public:
	LFO(int wave = 0);

	void update() override { _applyWave(); }
};

class FreqLFO : public LFO
{
      private:
	void applySample(Sample &s) override;
};

class AmpLFO : public LFO
{
      private:
      public:
	void applySample(Sample &s) override;
};

#endif
