#ifndef __WAVE_FUNCTIONS_HPP__
#define __WAVE_FUNCTIONS_HPP__

#include <random>
#include <unordered_map>

inline std::random_device rd;
inline std::mt19937 mt_engine(rd());
inline std::uniform_real_distribution<float> d_dist(-1.0, 1.0);

// float (*WaveFunctions[])(float) = {sine, square, triangle, sawtooth, noise};

enum WaveForm { SINE = 0, SQUARE, TRIANGLE, SAWTOOTH, NOISE, NUM_WAVES };

struct Wave {
	const char *label;
	float (*waveFunction)(float);
};

inline float sine(float phase) { return std::sin(phase); }

inline float square(float phase)
{
	return 2.0 * std::signbit(-sine(phase)) - 1.0;
}

inline float triangle(float phase)
{
	return 2.0 / M_PI * std::asin(sine(phase));
}

inline float sawtooth(float phase)
{
	return 2.0 / M_PI * std::atan(std::tan(phase / 2.0));
}

inline float noise(float phase) { return d_dist(mt_engine); }

const inline Wave WaveArray[WaveForm::NUM_WAVES] = {{"Sine", &sine},
						    {"Square", &square},
						    {"Triangle", &triangle},
						    {"Sawtooth", &sawtooth},
						    {"Noise", &noise}};

#endif