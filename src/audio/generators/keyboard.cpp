#include "keyboard.h"

Keyboard::Keyboard(int wave, int octave) : _wave(wave), _octave(octave)
{
	for (size_t i = 0; i < N_KEYS; i++) {
		_keyStates[i].id = _scanCodes[i];
		_keyStates[i].idx = i;
		_keyStates[i].amp = 0.0f;
		_keyStates[i].phase = 0.0f;
		_keyStates[i].state = EnvelopeState::Idle;
	}
}

void Keyboard::_intonate()
{
	if (_octave != _oldOctave) {
		for (auto &ks : _keyStates) {
			ks.freq = A_FREQ * std::powf(2.0f, ks.idx / 12.0f) *
				  std::pow(2, _octave - 4);
		}
		_oldOctave = _octave;
	}
}

void Keyboard::_applyWave()
{
	if (_wave != _oldWave) {
		_waveFunction = WaveArray[_wave].waveFunction;
		_oldWave = _wave;
	}
}

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
	_intonate();
	_applyWave();

	for (auto &ks : _keyStates) {
		for (auto &m : _modifiers) {
			m->apply(ks);
		}
		if (ks.state != EnvelopeState::Idle) {
			s.value += ks.amp * _waveFunction(ks.phase);

			s.displayValue +=
			    ks.amp *
			    _waveFunction(
				fmod(s.idx * _timeStep * ks.freq, 2.0 * M_PI));

			ks.phase =
			    fmod(ks.phase + _timeStep * ks.freq, 2.0 * M_PI);
		} else
			ks.phase = 0.0f;
	}

	return;
}
