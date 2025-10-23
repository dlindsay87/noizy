#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include <SDL2/SDL.h>
#include <chrono>
#include <iomanip>
#include <iostream>

typedef std::chrono::steady_clock sc;
typedef std::chrono::time_point<sc> tp_sc;
typedef std::chrono::duration<double, std::milli> dd;

const int TARGET_FPS = 60;
const double FRAME_DURATION = 1000.0 / TARGET_FPS; // 16.67 ms

const int TICKS_PER_SECOND = 20;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = TICKS_PER_SECOND / 5;

class Timer
{
      private:
	// ticks
	double _interpolation, _accumulator;
	int _loops, _frameCount, _currentFrame;

	// frames
	tp_sc _startTime, _frameStartTime;
	double _frameDuration, _fps;

	double _frameTimes[TARGET_FPS];

	void _calcFrameTime();

      public:
	Timer();

	void setLoop();

	bool willCatchUp();
	void update();

	void interpolate();
	void printOut();
	void limit();

	double getDelta(tp_sc t) const { return dd(sc::now() - t).count(); }
	double getRunTime() const { return getDelta(_startTime); }
	double getFPS() const { return _fps; }
	double getInterpolation() const { return _interpolation; }
};

#endif
