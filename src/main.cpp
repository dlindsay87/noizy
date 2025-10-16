#include "input_manager.h"
#include "renderer.h"
#include "sdl_init.h"
#include "time_manager.h"
#include "window.h"

#include "audio_manager.h"
#include "keyboard.h"
#include "oscilloscope.h"

#include "base_knob.h"

class Game
{
      private:
	Timer timer;
	Window window;
	Renderer renderer;
	Input input;

	AudioManager audioManager;
	Keyboard keyboard;
	Oscilloscope osc;

	IKnob<float> knob;
	IKnob<int> knob1;
	IKnob<int> knob2;

	SDL_Event e;
	bool game_on;

      public:
	Game() : game_on(true)
	{
		initSDL();

		window.init("Test", 1024, 480);
		renderer.init(window.getWindow());

		audioManager.init();
		audioManager.addGenerator(&keyboard);
		osc.init({512, 160}, {480, 240}, audioManager.getBufferLen());

		knob.init("Test", 2.3f, {256, 160}, DS::MID, {-60.0f, 180.0f},
			  {0.0f, 26.1f}, 0.4f);
		knob1.init("Test1", 0, {192, 380}, DS::SMALL, {-60.0f, 180.0f},
			   {0, 5}, 1);
		knob2.init("Test2", 0, {320, 380}, DS::SMALL, {-60.0f, 180.0f},
			   {0, 5}, 1);
	}

	~Game()
	{
		std::cout << "\nGame Over!" << std::endl;
		SDL_Quit();
	}

	void startLoop() { timer.setLoop(); }

	void processInput()
	{
		input.reset();
		while (SDL_PollEvent(&e) != 0) {
			input.processEvents(e);
		}

		window.processInput(&input);
		keyboard.processInput(&input);

		knob.processInput(&input);
		knob1.processInput(&input);
		knob2.processInput(&input);

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);

		knob.update();
		knob1.update();
		knob2.update();

		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();
		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);

		knob.draw(&renderer, intp);
		knob1.draw(&renderer, intp);
		knob2.draw(&renderer, intp);

		renderer.present();
	}

	void endLoop()
	{
		timer.printOut();
		std::cout << std::flush;
		timer.limit();
	}

	void run()
	{
		while (game_on) {
			startLoop();
			processInput();
			while (timer.willCatchUp()) {
				update();
				audioManager.playTone(static_cast<uint>(0));
			}
			timer.interpolate();
			render(timer.getInterpolation());
			endLoop();
		}
	}
};

int main()
{
	Game game;
	game.run();
	return 0;
}