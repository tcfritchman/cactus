#include "Frontend.h"
#include "Util.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer.h"
#include <SDL.h>
#include <utility>

Frontend Frontend::Init(SDL_Window* window, SDL_Renderer* renderer, std::shared_ptr<Emulator> emulator)
{
	// Initialize Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	return Frontend{ window, renderer, io, std::move(emulator) };
}

void Frontend::Redraw()
{
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Redraw UI Components
	DrawPPURender();
	DrawMainMenuBar();
	DrawCPUDebug();
	DrawMemoryDebug();
	DrawPPUDebug();

	// Rendering
	SDL_RenderSetScale(mRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(mRenderer,
		(Uint8)(CLEAR_COLOR.x * 255),
		(Uint8)(CLEAR_COLOR.y * 255),
		(Uint8)(CLEAR_COLOR.z * 255),
		(Uint8)(CLEAR_COLOR.w * 255));
	SDL_RenderClear(mRenderer);
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void Frontend::DrawMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Cmd+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("rom1.nes");
				ImGui::MenuItem("rom2.nes");
				ImGui::MenuItem("romN.nes");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Cmd+Q")) {
				mEmulator->Quit();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("CPU", NULL, shouldShowCpuWindow)) {
				shouldShowCpuWindow = !shouldShowCpuWindow;
			}
			if (ImGui::MenuItem("Memory", NULL, shouldShowMemoryWindow)) {
				shouldShowMemoryWindow = !shouldShowMemoryWindow;
			}
			if (ImGui::MenuItem("Pattern Table", NULL, shouldShowPPUDebugWindow)) {
				shouldShowPPUDebugWindow = !shouldShowPPUDebugWindow;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Machine"))
		{
			if (ImGui::MenuItem(mEmulator->IsPaused() ? "Unpause" : "Pause", "Cmd+P", false, true)) {
				mEmulator->IsPaused() ? mEmulator->Unpause() : mEmulator->Pause();
			}
			if (ImGui::MenuItem("Hard Reset", "Cmd+Shift+R")) {
				mEmulator->HardReset();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("Break", "Cmd+B", false, mEmulator->CanBreak())) {
				mEmulator->Break();
			}
			if (ImGui::MenuItem("Set Breakpoint", "Cmd+Shift+B", false, false)) {
				// TODO
			}
			if (ImGui::MenuItem("Step", "Spacebar", false, mEmulator->CanStep())) {
				mEmulator->Step();
			}
			if (ImGui::MenuItem("Continue", 			nullptr, false, mEmulator->CanContinue())) {
				mEmulator->Continue();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Frontend::DrawCPUDebug()
{
	if (!shouldShowCpuWindow) return;

	CPU::State state = mEmulator->mNes->cpu->GetState();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	bool* p_open = nullptr;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 20),
		ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("CPU Debugger", p_open, window_flags);

	// Left Column Sub-window
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
		ImGui::BeginChild("window_col_l", ImVec2(450, 360), false, window_flags);

		// Program Sub-window
		{
			ImGuiWindowFlags
				window_flags = ImGuiWindowFlags_NoDecoration;
			ImGui::BeginChild("window_program", ImVec2(450, 360), true, window_flags);

			ImGuiTableFlags flags = ImGuiTableFlags_None;

			if (ImGui::BeginTable("table_program", 4, flags))
			{

				uint16_t current_pc = state.PC;

				for (int row = 0; row < 20; row++)
				{
					uint8_t op_code = mEmulator->mNes->dataBus->read(current_pc);
					Operation operation = mEmulator->mNes->cpu->GetOperation(op_code);

					if (current_pc + operation.bytes - 1 < current_pc)
					{
						break;
					}

					ImGui::TableNextRow();

					// Pointer
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(row == 0 ? "        ->" : "");

					// Address
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%04x", current_pc);

					// Hex
					ImGui::TableSetColumnIndex(2);
					if (operation.bytes == 1)
					{
						ImGui::Text("%02x", op_code);
					}
					else if (operation.bytes == 2)
					{
						ImGui::Text("%02x %02x", op_code, mEmulator->mNes->dataBus->read(current_pc + 1));
					}
					else
					{
						ImGui::Text("%02x %02x %02x",
							op_code,
							mEmulator->mNes->dataBus->read(current_pc + 1),
							mEmulator->mNes->dataBus->read(current_pc + 2));
					}

					// Disassembled Instruction
					ImGui::TableSetColumnIndex(3);
					switch (operation.addressing_mode)
					{

					case Operation::AddressingMode::UNKNOWN:
					case Operation::AddressingMode::IMPLICIT:
						ImGui::Text("%s", operation.mnemonic.c_str());
						break;
					case Operation::AddressingMode::ACCUMULATOR:
						ImGui::Text("%s A", operation.mnemonic.c_str());
						break;
					case Operation::AddressingMode::IMMEDIATE:
						ImGui::Text("%s #%d", operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE:
						ImGui::Text("%s $%02x", operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE_X:
						ImGui::Text("%s $%02x,X", operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE_Y:
						ImGui::Text("%s $%02x,Y", operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::RELATIVE:
						ImGui::Text("%s *$%+d", operation.mnemonic.c_str(),
							static_cast<int8_t>(mEmulator->mNes->dataBus->read(current_pc + 1)));
						break;
					case Operation::AddressingMode::ABSOLUTE:
						ImGui::Text("%s $%02x%02x",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 2),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ABSOLUTE_X:
						ImGui::Text("%s $%02x%02x,X",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 2),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ABSOLUTE_Y:
						ImGui::Text("%s $%02x%02x,Y",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 2),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDIRECT:
						ImGui::Text("%s ($%02x%02x)",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 2),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDEXED_INDIRECT:
						ImGui::Text("%s ($%02x,X)",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDIRECT_INDEXED:
						ImGui::Text("%s ($%02x),Y",
							operation.mnemonic.c_str(),
							mEmulator->mNes->dataBus->read(current_pc + 1));
						break;
					}

					current_pc += operation.bytes;
				}
				ImGui::EndTable();
			}

			ImGui::EndChild();
		}

		ImGui::EndChild();
	}

	ImGui::SameLine();

	// Right Column Sub-window
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
		ImGui::BeginChild("window_col_r", ImVec2(160, 360), false, window_flags);

		// Registers Sub-window
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
			ImGui::BeginChild("window_registers", ImVec2(160, 120), true, window_flags);

			if (ImGui::BeginTable("table_registers", 2))
			{
				ImGui::TableNextColumn();
				ImGui::Text("A");
				ImGui::TableNextColumn();
				ImGui::Text("%02x", state.A);

				ImGui::TableNextColumn();
				ImGui::Text("X");
				ImGui::TableNextColumn();
				ImGui::Text("%02x", state.X);

				ImGui::TableNextColumn();
				ImGui::Text("Y");
				ImGui::TableNextColumn();
				ImGui::Text("%02x", state.Y);

				ImGui::TableNextColumn();
				ImGui::Text("P");
				ImGui::TableNextColumn();
				ImGui::Text("%02x", state.P);

				ImGui::TableNextColumn();
				ImGui::Text("PC");
				ImGui::TableNextColumn();
				ImGui::Text("%04x", state.PC);

				ImGui::TableNextColumn();
				ImGui::Text("S");
				ImGui::TableNextColumn();
				ImGui::Text("%04x", state.SP);

				ImGui::EndTable();
			}

			ImGui::EndChild();
		}

		// Flags Sub-window
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
			ImGui::BeginChild("window_flags", ImVec2(160, 120), true, window_flags);

			if (ImGui::BeginTable("table_flags", 2))
			{
				ImGui::TableNextColumn();
				ImGui::Text("C");
				ImGui::TableNextColumn();
				ImGui::Text("%x", state.C());

				ImGui::TableNextColumn();
				ImGui::Text("Z");
				ImGui::TableNextColumn();
				ImGui::Text("%x", state.Z());

				ImGui::TableNextColumn();
				ImGui::Text("I");
				ImGui::TableNextColumn();
				ImGui::Text("%x", state.I());

				ImGui::TableNextColumn();
				ImGui::Text("V");
				ImGui::TableNextColumn();
				ImGui::Text("%x", state.V());

				ImGui::TableNextColumn();
				ImGui::Text("N");
				ImGui::TableNextColumn();
				ImGui::Text("%x", state.N());

				ImGui::EndTable();
			}

			ImGui::EndChild();
		}

		// Stack Sub-window
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
			ImGui::BeginChild("window_stack", ImVec2(160, 0), true, window_flags);

			ImGui::Text("TODO");

			ImGui::EndChild();
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void Frontend::DrawMemoryDebug()
{
	if (!shouldShowMemoryWindow) return;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	bool* p_open = nullptr;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 700, main_viewport->WorkPos.y + 20),
		ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Memory", p_open, window_flags);

	static int start_addr = 0;
	static const int rows = 20;
	static const int step = 1;
	static const int step_fast = 32;

	ImGui::InputInt("##", &start_addr, step, step_fast, ImGuiInputTextFlags_CharsHexadecimal);

	if (start_addr < 0)
	{
		start_addr = 0;
	}

	if (start_addr > (0xFFFF - rows))
	{
		start_addr = 0xFFFF - rows;
	}

	if (ImGui::BeginTable("table_memory", 3))
	{
		for (int row = 0; row < rows; row++)
		{
			uint16_t addr = start_addr + row;

			if (addr < start_addr)
			{
				break;
			}

			ImGui::TableNextRow();

			uint8_t data = mEmulator->mNes->dataBus->read(addr);

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%04x", addr);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%02x", data);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text("%d", data);
		}

		ImGui::EndTable();
	}

	ImGui::End();
}

void Frontend::DrawPPUDebug()
{
	if (!shouldShowPPUDebugWindow) return;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	bool* p_open = nullptr;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 20, main_viewport->WorkPos.y + 430),
		ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("PPU Internal State", p_open, window_flags);

	// Test texture
	int texture_width = 128, texture_height = 128;
	static SDL_Texture* texture_left = nullptr;
	static SDL_Texture* texture_right = nullptr;

	// TODO: Update
	if (texture_left == nullptr)
	{
		uint32_t* pixel_buffer_left = new uint32_t[texture_width * texture_height] {};
		uint32_t* pixel_buffer_right = new uint32_t[texture_width * texture_height] {};

		texture_left = SDL_CreateTexture(mRenderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			texture_width,
			texture_height);
		texture_right = SDL_CreateTexture(mRenderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			texture_width,
			texture_height);

		// Render pattern data
		for (uint16_t addr = 0; addr < 0x2000; addr++) {
			bool isUpperPlane = (addr >> 0x3) & 0b1;
			uint16_t fineYOffset = addr & 0b111;
			uint16_t tileCol = (addr >> 0x4) & 0b1111;
			uint16_t tileRow = (addr >> 0x8) & 0b1111;
			bool isRightHalf = (addr >> 0xC) & 0b1;
			uint32_t* buffer = isRightHalf ? pixel_buffer_right : pixel_buffer_left;
			uint8_t val = mEmulator->mNes->cart->v_read(addr);
			for (int bit = 7; bit >= 0; bit--) {
				uint32_t pixel = (0x400 * tileRow) + (0x80 * fineYOffset) + (0x8 * tileCol) + (7 - bit);
				if (!isUpperPlane) {
					// overwrite the pixel - the lower plane value always comes first
					buffer[pixel] = nes::test_bit(val, bit) ? 0x003F3F3F : 0;
				} else {
					buffer[pixel] += nes::test_bit(val, bit) ? 0x007F7F7F : 0;
				}
			}
		}

		SDL_UpdateTexture(texture_left, nullptr, pixel_buffer_left, texture_width * sizeof(uint32_t));
		SDL_UpdateTexture(texture_right, nullptr, pixel_buffer_right, texture_width * sizeof(uint32_t));
	}

	ImGui::Image((void*)(intptr_t)texture_left, ImVec2(texture_width, texture_height));
	ImGui::SameLine();
	ImGui::Image((void*)(intptr_t)texture_right, ImVec2(texture_width, texture_height));

	ImGui::End();
}

void Frontend::DrawPPURender()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	bool* p_open = nullptr;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 860, main_viewport->WorkPos.y + 80),
		ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("PPU Render", p_open, window_flags);

	// Test texture
	static int texture_width = 256, texture_height = 240;
	static SDL_Texture* texture = nullptr;
	static uint32_t* pixel_buffer; // TODO: Replace placeholder with rendered pixel data

	if (texture == nullptr)
	{
		pixel_buffer = new uint32_t[texture_width * texture_height]{0};

		texture = SDL_CreateTexture(mRenderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			texture_width,
			texture_height);
	}

	SDL_UpdateTexture(texture, nullptr, pixel_buffer, texture_width * sizeof(uint32_t));

	ImGui::Image((void*)(intptr_t)texture, ImVec2(texture_width, texture_height));

	ImGui::End();
}

Frontend::Frontend(SDL_Window* mWindow, SDL_Renderer* mRenderer, ImGuiIO& io, std::shared_ptr<Emulator> emulator)
	: mWindow(mWindow), mRenderer(mRenderer), io(io), mEmulator(std::move(emulator))
{
}

void Frontend::HandleEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}



