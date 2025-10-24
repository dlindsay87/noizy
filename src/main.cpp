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

	IKnob<int> wknob;
	IKnob<float> vknob;

	IKnob<float> aknob;
	IKnob<float> dknob;
	IKnob<float> sknob;
	IKnob<float> rknob;

	SpinBox obox;

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

		wknob.init("Wave", &keyboard.referenceWave(), {66, 100},
			   DS::SMALL, {-60.0f, 180.0f},
			   {0, WaveForm::NUM_WAVES - 1}, 1);

		vknob.init("Volume", &audioManager.referenceVolume(), {66, 220},
			   DS::SMALL, {-60.0f, 180.0f}, {-40.0f, 0.0f}, 1.0f);

		aknob.init("Attack", &envelope.referenceAttack(), {316, 100},
			   DS::SMALL, {-60.0f, 180.0f}, {0.0f, 4.0f}, 0.2f);
		dknob.init("Decay", &envelope.referenceDecay(), {437, 100},
			   DS::SMALL, {-60.0f, 180.0f}, {0.0f, 4.0f}, 0.2f);
		sknob.init("Sustain", &envelope.referenceSustain(), {316, 220},
			   DS::SMALL, {-60.0f, 180.0f}, {0.0f, 1.0f}, 0.1f);
		rknob.init("Release", &envelope.referenceRelease(), {437, 220},
			   DS::SMALL, {-60.0f, 180.0f}, {0.0f, 4.0f}, 0.2f);

		obox.init("Octave", &keyboard.referenceOctave(), {0, 8},
			  {174, 160}, {DS::MID, DS::SMALL});
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

		wknob.processInput(&input);
		vknob.processInput(&input);

		aknob.processInput(&input);
		dknob.processInput(&input);
		sknob.processInput(&input);
		rknob.processInput(&input);

		obox.processInput(&input);

		if (input.isKeyPressed(SDL_SCANCODE_Z) ||
		    input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.willExit())
			game_on = false;
	}

	void update()
	{
		window.update(0);

		wknob.applyCat(WaveArray[wknob.getValue()].label);
		vknob.applyNum();

		aknob.applyNum();
		dknob.applyNum();
		sknob.applyNum();
		rknob.applyNum();

		timer.update();
	}

	void render(float intp)
	{
		renderer.clear();

		SDL_SetRenderDrawColor(renderer.getRenderer(), 255, 255, 255,
				       255);
		SDL_RenderDrawLine(renderer.getRenderer(), 240, 40, 240, 280);

		osc.draw(audioManager.getDisplayBuffer(), &renderer, intp);

		wknob.draw(&renderer, intp);
		vknob.draw(&renderer, intp);

		aknob.draw(&renderer, intp);
		dknob.draw(&renderer, intp);
		sknob.draw(&renderer, intp);
		rknob.draw(&renderer, intp);

		obox.draw(&renderer, intp);

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