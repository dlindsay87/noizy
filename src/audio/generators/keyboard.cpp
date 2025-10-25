#include "keyboard.h"

Keyboard::Keyboard(int wave, int octave)
    : _wave(wave), _oldWave(wave), _octave(octave), _oldOctave(octave)
{
	_waveFunction = WaveArray[_wave].waveFunction;
	for (size_t i = 0; i < N_KEYS; i++) {
		_keyStates[i] = (ToneState){
		    .id = _scanCodes[i],
		    .idx = i,
		    .amp = 0.0f,
		    .peak = 0.0f,
		    .freq = _intonate(i),
		    .phase = 0.0f,
		    .state = EnvelopeState::Idle,
		};
	}
}

float Keyboard::_intonate(int i)
{
	return A_FREQ * std::powf(2.0f, i / 12.0f) * std::pow(2, _octave - 4);
}

void Keyboard::_applyIntonation()
{
	if (_octave != _oldOctave) {
		for (auto &ks : _keyStates) {
			ks.freq = _intonate(ks.idx);
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

float Keyboard::_advancePhase(float &phase, float amp, float freq)
{
	float output = _waveFunction(phase);
	phase = fmod(phase + _timeStep * freq, 2.0 * M_PI);
	return amp * output;
}

float Keyboard::_displayPhase(size_t idx, float amp, float freq)
{
	float phase = fmod(idx * _timeStep * freq, 2.0 * M_PI);
	return amp * _waveFunction(phase);
}

void Keyboard::processInput(Input *ip)
{
	for (auto &ks : _keyStates) {
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
	_applyIntonation();
	_applyWave();

	for (auto &m : _modifiers) {
		m->applySample(s);
	}
	for (auto &ks : _keyStates) {
		for (auto &m : _modifiers) {
			m->applyState(ks);
		}
		if (ks.state != EnvelopeState::Idle) {
			float totAmp = ks.amp * (1.0 + s.ampMod);
			float totFreq = ks.freq * (1.0 + s.freqMod);

			s.value += _advancePhase(ks.phase, totAmp, totFreq);
			s.displayValue += _displayPhase(s.idx, totAmp, totFreq);

		} else
			ks.phase = 0.0f;
	}

	return;
}
