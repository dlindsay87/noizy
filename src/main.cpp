#include "input_manager.h"
#include "renderer.h"
#include "sdl_init.h"
#include "time_manager.h"
#include "window.h"

#include "audio_manager.h"
#include "keyboard.h"
#include "oscilloscope.h"

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

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);
		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();
		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);
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