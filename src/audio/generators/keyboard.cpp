#include "keyboard.h"

Keyboard::Keyboard(int octave)
{
	intonate(octave);
	for (int i = 0; i < N_KEYS; i++) {
		_keyStates[i].id = _scanCodes[i];
		_keyStates[i].amp = 0.0f;
		_keyStates[i].phase = 0.0f;
		_keyStates[i].state = EnvelopeState::Idle;
	}
	//_wave = WaveArray[SINE];
}

void Keyboard::intonate(int o)
{
	if (o != _octave) {
		for (int i = 0; i < N_KEYS; i++) {
			_keyStates[i].freq = A_FREQ *
					     std::powf(2.0f, i / 12.0f) *
					     std::pow(2, o - 4);
		}
		_octave = o;
	}
}

void Keyboard::selectWave(int w) { _wave = WaveArray[w]; }

void Keyboard::processInput(Input *ip)
{
	for (auto &ks : _keyStates) {
		/*if (ip->isKeyDown(ks.id)) {
			ks.state = EnvelopeState::Attack;
			ks.amp = 1.0f;
		} else {
			ks.state = EnvelopeState::Idle;
			ks.amp = 0.0f;
		}*/
		if (ip->isKeyPressed(ks.id)) {
			ks.state = EnvelopeState::Attack;
		} else if (!ip->isKeyDown(ks.id) &&
			   ks.state != EnvelopeState::Idle) {
			ks.state = EnvelopeState::Release;
		}
	}
};

void Keyboard::generate(Sample &s)
{
	for (auto &ks : _keyStates) {
		for (auto &m : _modifiers) {
			m->apply(ks);
		}
		if (ks.state != EnvelopeState::Idle) {
			s.value += ks.amp * _wave.waveFunction(ks.phase);

			s.displayValue +=
			    ks.amp *
			    _wave.waveFunction(
				fmod(s.idx * _timeStep * ks.freq, 2.0 * M_PI));

			ks.phase =
			    fmod(ks.phase + _timeStep * ks.freq, 2.0 * M_PI);
		} else
			ks.phase = 0.0f;
	}

	return;
}
