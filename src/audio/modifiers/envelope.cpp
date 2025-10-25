#include "envelope.h"

void Envelope::applyState(ToneState &toneState)
{
	EnvelopeState &state = toneState.state;
	float &level = toneState.amp;
	float &peak = toneState.peak;

	switch (state) {
	case EnvelopeState::Idle:
		level = 0.0;
		peak = level;
		break;

	case EnvelopeState::Attack:
		if (_attack <= 0.0f || level >= 1.0) {
			level = 1.0;
			state = EnvelopeState::Decay;
		} else
			level += 1.0 / (_attack * _sampleRate + 10e-6);
		peak = level;
		break;

	case EnvelopeState::Decay:
		if (_sustain > 0.0f) {
			if (_decay <= 0.0f || level <= _sustain) {

				level = _sustain;
				state = EnvelopeState::Sustain;

			} else
				level -= (1.0 - _sustain) /
					 (_decay * _sampleRate + 10e-6);
		} else
			state = EnvelopeState::Release;
		peak = level;
		break;

	case EnvelopeState::Sustain:
		level = _sustain;
		peak = level;
		break;

	case EnvelopeState::Release:
		if (_release <= 0.0f || level <= 0.0f) {
			level = 0.0f;
			state = EnvelopeState::Idle;
		} else
			level -= peak / (_release * _sampleRate + 10e-6);
		break;
	}
}