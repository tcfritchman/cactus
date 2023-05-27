#include <SDL.h>
#include "imgui/imgui_impl_sdl2.h"
#include "UI.h"
#include "Emulator.h"

int main(int argc, char* argv[])
{
	auto filename = argv[1];
	auto emulator = Emulator(filename);

	auto quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Untitled NES Emulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	Uint64 ticksElapsed = SDL_GetTicks64();

	// TODO: Pass emulator only
	auto ui = UI::Init(window, renderer, emulator.mNes);

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
				if (event.key.keysym.sym == SDLK_SPACE && emulator.CanStep())
				{
					emulator.Step();
				}
				break;
			}
		}

		Uint64 ticks = SDL_GetTicks64();
		Uint64 deltaTicks = ticks - ticksElapsed;
		ticksElapsed = ticks;

		emulator.Tick(deltaTicks);

		ui.Redraw();

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
