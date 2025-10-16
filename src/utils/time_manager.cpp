#include "time_manager.h"

Timer::Timer()
    : _interpolation(0.0), _accumulator(0.0), _loops(0), _frameCount(0),
      _currentFrame(0), _frameDuration(0.0), _fps(0.0)
{
	_startTime = _frameStartTime = sc::now();
}

void Timer::_calcFrameTime()
{
	double frameTimeTotal = 0.0f;

	_frameTimes[_currentFrame] = _frameDuration;

	if (++_currentFrame >= TARGET_FPS)
		_currentFrame = 0;
	if (_frameCount < TARGET_FPS)
		_frameCount++;

	for (int i = 0; i < _frameCount; ++i)
		frameTimeTotal += _frameTimes[i];

	_fps = 1000.0f * _frameCount / frameTimeTotal;
}

void Timer::setLoop()
{
	_frameDuration = getDelta(_frameStartTime);
	_frameStartTime = sc::now();
	_accumulator += _frameDuration;
	_loops = 0;
}

bool Timer::willCatchUp()
{
	// if actual time taken is greater than ideal (slow)
	bool isBehind = _accumulator >= SKIP_TICKS;
	// if we haven't done too many catch up updates
	bool canCatchUp = _loops < MAX_FRAMESKIP;
	return isBehind && canCatchUp;
}

void Timer::update()
{
	_accumulator -= SKIP_TICKS;
	_loops++;
}

void Timer::interpolate()
{
	// calculates betweenness of last update and next expected one.
	_interpolation = static_cast<double>(_accumulator / SKIP_TICKS);
}

void Timer::printOut()
{
	_calcFrameTime();
	std::cout << std::fixed << std::setprecision(3)
		  << "\033[2K\rFPS: " << _fps
		  << " | Interpolation: " << _interpolation
		  << " | Run Time: " << getRunTime() << "s";
}

void Timer::limit()
{
	if (SDL_GL_GetSwapInterval() == 0) {
		double sleepTime = FRAME_DURATION - getDelta(_frameStartTime);
		if (sleepTime > 0)
			SDL_Delay(sleepTime);
	}
}
