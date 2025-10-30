#include "input_manager.h"
#include "renderer.h"
#include "sdl_init.h"
#include "time_manager.h"
#include "window.h"

#include "audio_manager.h"
#include "envelope.h"
#include "keyboard.h"
// #include "lfo.h"
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
	// fLFO f_LFO;
	// aLFO a_LFO;
	Oscilloscope osc;
	Envelope envelope;

	CPanel kPanel;
	CPanel ePanel;

	/*TextOverlay fOV = {
	    .text = "FreqMod:",
	    .font =
		{
		    .size = DS::SMALL,
		    .color = ColorSelection[DC::NEUTRAL_W],
		},
	    .pos = {480, 340},

	};
	IKnob<int> f_lfowave;
	IKnob<float> f_lfodepth;
	IKnob<float> f_lforate;

	TextOverlay aOV = {
	    .text = "Amp Mod:",
	    .font =
		{
		    .size = DS::SMALL,
		    .color = ColorSelection[DC::NEUTRAL_W],
		},
	    .pos = {480, 450},

	};
	IKnob<int> a_lfowave;
	IKnob<float> a_lfodepth;
	IKnob<float> a_lforate;*/

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
		// keyboard.addModifier(&f_LFO);
		// keyboard.addModifier(&a_LFO);

		osc.init({512, 150}, {480, 240}, audioManager.getBufferLen());

		kPanel.addControls(&audioManager.getControls());
		kPanel.addControls(&keyboard.getControls());
		kPanel.layout({2, 2}, {0, 30}, {240, 240}, false); // 174, 150

		ePanel.addControls(&envelope.getControls());
		ePanel.layout({2, 2}, {240, 30}, {272, 240});

		/*f_lfowave.init("Wave", &f_LFO.referenceWave(), {576, 340},
			       DS::SMALL, {-60.0f, 180.0f},
			       {0, WaveForm::NUM_WAVES - 1}, 1);
		f_lfodepth.init("Depth", &f_LFO.referenceDepth(), {752, 340},
				DS::SMALL, {-60.0f, 240.0f}, {-8.0f, 8.0f},
				0.2f);
		f_lforate.init("Rate", &f_LFO.referenceRate(), {928, 340},
			       DS::SMALL, {-60.0f, 180.0f}, {0, 16}, 1);

		a_lfowave.init("Wave", &a_LFO.referenceWave(), {576, 450},
			       DS::SMALL, {-60.0f, 180.0f},
			       {0, WaveForm::NUM_WAVES - 1}, 1);
		a_lfodepth.init("Depth", &a_LFO.referenceDepth(), {752, 450},
				DS::SMALL, {-60.0f, 240.0f}, {-8.0f, 8.0f},
				0.5f);
		a_lforate.init("Rate", &a_LFO.referenceRate(), {928, 450},
			       DS::SMALL, {-60.0f, 180.0f}, {0, 16}, 1);*/
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

		/*f_lfowave.processInput(&input);
		f_lfodepth.processInput(&input);
		f_lforate.processInput(&input);

		a_lfowave.processInput(&input);
		a_lfodepth.processInput(&input);
		a_lforate.processInput(&input);*/

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);

		kPanel.update();
		ePanel.update();

		keyboard.update();

		/*f_lfowave.applyCat(WaveArray[f_lfowave.getValue()].label);
		f_lfodepth.applyNum();
		f_lforate.applyNum();

		a_lfowave.applyCat(WaveArray[a_lfowave.getValue()].label);
		a_lfodepth.applyNum();
		a_lforate.applyNum();*/

		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();

		SDL_SetRenderDrawColor(renderer.getRenderer(), 255, 255, 255,
				       255);
		// just a decal
		SDL_RenderDrawLine(renderer.getRenderer(), 240, 40, 240, 260);

		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);

		keyboard.draw(&renderer, intp);

		kPanel.draw(&renderer, intp);
		ePanel.draw(&renderer, intp);

		/*renderer.drawCachedText(fOV);
		f_lfowave.draw(&renderer, intp);
		f_lfodepth.draw(&renderer, intp);
		f_lforate.draw(&renderer, intp);

		renderer.drawCachedText(aOV);
		a_lfowave.draw(&renderer, intp);
		a_lfodepth.draw(&renderer, intp);
		a_lforate.draw(&renderer, intp);*/

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