#include "input_manager.h"
#include "renderer.h"
#include "sdl_init.h"
#include "time_manager.h"
#include "window.h"

#include "audio_manager.h"
#include "envelope.h"
#include "keyboard.h"
#include "lfo.h"
#include "oscilloscope.h"

#include "control_panel.h"
#include "knob.hpp"
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
	FreqLFO fLFO;
	AmpLFO aLFO;
	Oscilloscope osc;
	Envelope envelope;

	CPanel kPanel;
	CPanel ePanel;
	CPanel lfoPanel;

	TextOverlay fOV = {
	    .text = "Freq Mod:",
	    .font =
		{
		    .size = DS::SMALL,
		    .color = ColorSelection[DC::NEUTRAL_W],
		},
	    .pos = {480, 338},

	};

	TextOverlay aOV = {
	    .text = "Ampt Mod:",
	    .font =
		{
		    .size = DS::SMALL,
		    .color = ColorSelection[DC::NEUTRAL_W],
		},
	    .pos = {480, 444},

	};

	SDL_Event e;
	bool game_on;

      public:
	Game() : game_on(true)
	{
		initSDL();

		window.init("Test", 1024, 512);
		renderer.init(window.getWindow());

		audioManager.init();
		audioManager.addGenerator(&keyboard);
		keyboard.addModifier(&envelope);
		keyboard.addModifier(&fLFO);
		keyboard.addModifier(&aLFO);

		osc.init({512, 150}, {480, 240}, audioManager.getBufferLen());

		kPanel.addControls(&audioManager.getControls());
		kPanel.addControls(&keyboard.getControls());
		kPanel.layout({2, 2}, {0, 30}, {240, 240}, false);

		ePanel.addControls(&envelope.getControls());
		ePanel.layout({2, 2}, {240, 30}, {272, 240});

		lfoPanel.addControls(&fLFO.getControls());
		lfoPanel.addControls(&aLFO.getControls());
		lfoPanel.layout({3, 2}, {512, 285}, {480, 212});
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

		kPanel.processInput(&input);
		ePanel.processInput(&input);
		lfoPanel.processInput(&input);

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);

		kPanel.update();
		ePanel.update();
		lfoPanel.update();

		keyboard.update();

		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();

		SDL_SetRenderDrawColor(renderer.getRenderer(), 255, 255, 255,
				       255);
		// just a decal
		SDL_RenderDrawLine(renderer.getRenderer(), 240, 40, 240, 260);
		renderer.drawCachedText(fOV);
		renderer.drawCachedText(aOV);

		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);

		keyboard.draw(&renderer, intp);

		kPanel.draw(&renderer, intp);
		ePanel.draw(&renderer, intp);
		lfoPanel.draw(&renderer, intp);

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