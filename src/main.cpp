#include <memory>
#include <SDL.h>
#include "imgui/imgui_impl_sdl2.h"
#include "NES.h"
#include "UI.h"
#include "INesRom.h"
#include "Util.h"

enum OperatingMode
{
	STEP_CYCLE,
	STEP_CPU_INSTR,
	REALTIME
};

int main(int argc, char* argv[])
{
	auto filename = argv[1];
	auto rom_bytes = nes::read_file_bytes(filename);
	INesRom rom(rom_bytes);
	auto nes = std::make_shared<NES>(rom);

	auto mode = STEP_CYCLE;
	auto quit = false;
	auto step = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Untitled NES Emulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	auto ui = UI::Init(window, renderer, nes);

	while (!quit)
	{
		SDL_Delay(10);

		while (SDL_PollEvent(&event))
		{
			UI::HandleEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SPACE)
				{
					step = true;
				}
				break;
			}
		}

		switch (mode)
		{
		case STEP_CYCLE:
			if (step)
			{
				nes->Tick();
				step = false;
			}
			break;
		case STEP_CPU_INSTR:
			if (step)
			{
				do
				{
					nes->Tick();
				} while (nes->cpu->GetRemainingCycles() > 0);
				step = false;
			}
			break;
		case REALTIME:
			// TODO: Fixed timing
			nes->Tick();
			break;
		}

		ui.Redraw();

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
