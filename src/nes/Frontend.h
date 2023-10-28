#ifndef NES_EMULATOR_SRC_UI_H_
#define NES_EMULATOR_SRC_UI_H_

#include "NES.h"
#include "../imgui/imgui.h"
#include "Emulator.h"
#include <SDL.h>

class Frontend
{

 public:
	static Frontend Init(SDL_Window* window, SDL_Renderer* renderer, std::shared_ptr<Emulator> emulator);

	void Redraw();
	static void HandleEvent(SDL_Event* event);

	Frontend() = delete;

 private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	ImGuiIO& io;
	std::shared_ptr<Emulator> mEmulator;
	bool shouldShowCpuWindow = true;
	bool shouldShowMemoryWindow = true;
	bool shouldShowPPUDebugWindow = true;

	const ImVec4 CLEAR_COLOR = ImVec4(0.09f, 0.11f, 0.14f, 1.0f);

	Frontend(SDL_Window* mWindow, SDL_Renderer* mRenderer, ImGuiIO& io, std::shared_ptr<Emulator> emulator);

	void DrawCPUDebug();
	void DrawMemoryDebug();
	void DrawPPUDebug();
	void DrawMainMenuBar();
	void DrawPPURender();
};

#endif //NES_EMULATOR_SRC_UI_H_
