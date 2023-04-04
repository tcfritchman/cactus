#include "CPU.h"
#include "DataBus.h"
#include "RAM.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"
#include <SDL.h>

int main()
{
	auto dataBus = DataBus(RAM());
	CPU cpu(dataBus);
	CPU::State state = cpu.GetState();

	bool quit = false;
	bool step = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Untitled NES Emulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	// ImGui Stuff

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	// Background color
	ImVec4 clear_color = ImVec4(56.0f, 56.0f, 56.0f, 255.0f);

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	while (!quit)
	{
		SDL_Delay(10);

		while (SDL_PollEvent(&event))
		{
			// ImGui handler
			ImGui_ImplSDL2_ProcessEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				step = true;
				break;
			}
		}

		// Step the CPU if needed
		if (step)
		{
			cpu.Cycle();
			state = cpu.GetState();
			step = false;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Create CPU Debug window
		{
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
							uint8_t op_code = dataBus.read(current_pc);
							Operation operation = cpu.GetOperation(op_code);

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
								ImGui::Text("%02x %02x", op_code, dataBus.read(current_pc + 1));
							}
							else
							{
								ImGui::Text("%02x %02x %02x",
									op_code,
									dataBus.read(current_pc + 1),
									dataBus.read(current_pc + 2));
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
								ImGui::Text("%s #%d", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ZERO_PAGE:
								ImGui::Text("%s $%02x", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ZERO_PAGE_X:
								ImGui::Text("%s $%02x,X", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ZERO_PAGE_Y:
								ImGui::Text("%s $%02x,Y", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::RELATIVE:
								ImGui::Text("%s *$%+d", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ABSOLUTE:
								ImGui::Text("%s $%02x%02x",
									operation.mnemonic.c_str(),
									dataBus.read(current_pc + 2),
									dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ABSOLUTE_X:
								ImGui::Text("%s $%02x%02x,X",
									operation.mnemonic.c_str(),
									dataBus.read(current_pc + 2),
									dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::ABSOLUTE_Y:
								ImGui::Text("%s $%02x%02x,Y",
									operation.mnemonic.c_str(),
									dataBus.read(current_pc + 2),
									dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::INDIRECT:
								ImGui::Text("%s ($%02x%02x)",
									operation.mnemonic.c_str(),
									dataBus.read(current_pc + 2),
									dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::INDEXED_INDIRECT:
								ImGui::Text("%s ($%02x,X)", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
								break;
							case Operation::AddressingMode::INDIRECT_INDEXED:
								ImGui::Text("%s ($%02x),Y", operation.mnemonic.c_str(), dataBus.read(current_pc + 1));
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

		// Create Memory Debug Window
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
			bool* p_open = nullptr;

			const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 700, main_viewport->WorkPos.y + 20),
				ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);

			ImGui::Begin("Memory", p_open, window_flags);

			static int start_addr = 0x200;

			ImGui::InputInt("##", &start_addr);

			if (start_addr < 0) {
				start_addr = 0;
			}

			if (start_addr > 0x400) {
				start_addr = 0x400;
			}

			if (ImGui::BeginTable("table_memory", 3))
			{
				for (int row = 0; row < 20; row++)
				{
					uint16_t addr = start_addr + row;

					if (addr < start_addr) {
						break;
					}

					ImGui::TableNextRow();

					uint8_t data = dataBus.read(addr);

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


		// Rendering
		SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColor(renderer,
			(Uint8)(clear_color.x * 255),
			(Uint8)(clear_color.y * 255),
			(Uint8)(clear_color.z * 255),
			(Uint8)(clear_color.w * 255));
		SDL_RenderClear(renderer);
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
