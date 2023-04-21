#ifndef NES_EMULATOR_SRC_UI_H_
#define NES_EMULATOR_SRC_UI_H_

#include "NES.h"
#include "imgui/imgui.h"
#include <SDL.h>

class UI
{

 public:
	static UI Init(SDL_Window* window, SDL_Renderer* renderer, NES* nes);

	void Redraw();
	void HandleEvent(SDL_Event* event);

	UI() = delete;

 private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	ImGuiIO& io;
	NES* mNes;

	const ImVec4 CLEAR_COLOR = ImVec4(56.0f, 56.0f, 56.0f, 255.0f);

	UI(SDL_Window* mWindow, SDL_Renderer* mRenderer, ImGuiIO& io, NES* mNes);

	void DrawCPUDebug();
	void DrawMemoryDebug();
};

#endif //NES_EMULATOR_SRC_UI_H_
