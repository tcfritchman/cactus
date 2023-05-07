#include "imgui/imgui_impl_sdl2.h"
#include "NES.h"
#include "UI.h"
#include "INesRom.h"
#include "Util.h"
#include <SDL.h>

enum OperatingMode {
	STEP_CYCLE,
	STEP_CPU_INSTR,
	REALTIME
};

int main(int argc, char* argv[])
{
	auto filename = argv[1];
	auto rom_bytes = nes::read_file_bytes(filename);
	INesRom rom(rom_bytes);

	auto prg_rom = rom.GetProgramRomData();
	auto chr_rom = rom.GetCharacterRomData();

	NES* nes = new NES(rom);
	OperatingMode mode = STEP_CPU_INSTR;
	bool quit = false;
	bool step = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Untitled NES Emulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	UI ui = UI::Init(window, renderer, nes);

	while (!quit)
	{
		SDL_Delay(10);

		while (SDL_PollEvent(&event))
		{
			ui.HandleEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SPACE) {
					step = true;
				}
				break;
			}
		}

		switch (mode)
		{
		case STEP_CYCLE:
			if (step) {
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

	delete (nes);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
