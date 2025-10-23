#include "input_manager.h"
#include "renderer.h"
#include "sdl_init.h"
#include "time_manager.h"
#include "window.h"

#include "audio_manager.h"
#include "envelope.h"
#include "keyboard.h"
#include "oscilloscope.h"

#include "knob.h"
#include "spin_box.h"

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
	Envelope envelope;

	IKnob<int> knob;

	SpinBox spinbox;

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
		keyboard.addModifier(&envelope);

		osc.init({512, 160}, {480, 240}, audioManager.getBufferLen());

		knob.init("Wave", 0, {256, 160}, DS::MID, {-60.0f, 180.0f},
			  {0, WaveForm::NUM_WAVES - 1}, 1);

		spinbox.init("Octave", 4, {0, 8}, {640, 380},
			     {DS::MID, DS::SMALL});
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
		// envelope.processInput(&input);

		knob.processInput(&input);
		spinbox.processInput(&input);

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);

		knob.update();
		knob.applyCat(WaveArray[knob.getValue()].label);
		keyboard.selectWave(knob.getValue());

		spinbox.update();
		keyboard.intonate(spinbox.getValue());

		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();
		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);

		knob.draw(&renderer, intp);

		spinbox.draw(&renderer, intp);

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