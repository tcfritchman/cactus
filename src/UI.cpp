#include "UI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer.h"

UI UI::Init(SDL_Window* window, SDL_Renderer* renderer, NES* nes)
{
	// Initialize Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	return UI{ window, renderer, io, nes };
}

void UI::Redraw()
{
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Redraw UI Components
	DrawCPUDebug();
	DrawMemoryDebug();

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

void UI::DrawCPUDebug()
{
	CPU::State state = mNes->cpu->GetState();

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
					uint8_t op_code = mNes->dataBus->read(current_pc);
					Operation operation = mNes->cpu->GetOperation(op_code);

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
						ImGui::Text("%02x %02x", op_code, mNes->dataBus->read(current_pc + 1));
					}
					else
					{
						ImGui::Text("%02x %02x %02x",
							op_code,
							mNes->dataBus->read(current_pc + 1),
							mNes->dataBus->read(current_pc + 2));
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
						ImGui::Text("%s #%d", operation.mnemonic.c_str(), mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE:
						ImGui::Text("%s $%02x", operation.mnemonic.c_str(), mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE_X:
						ImGui::Text("%s $%02x,X", operation.mnemonic.c_str(), mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ZERO_PAGE_Y:
						ImGui::Text("%s $%02x,Y", operation.mnemonic.c_str(), mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::RELATIVE:
						ImGui::Text("%s *$%+d", operation.mnemonic.c_str(), mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ABSOLUTE:
						ImGui::Text("%s $%02x%02x",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 2),
							mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ABSOLUTE_X:
						ImGui::Text("%s $%02x%02x,X",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 2),
							mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::ABSOLUTE_Y:
						ImGui::Text("%s $%02x%02x,Y",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 2),
							mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDIRECT:
						ImGui::Text("%s ($%02x%02x)",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 2),
							mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDEXED_INDIRECT:
						ImGui::Text("%s ($%02x,X)",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 1));
						break;
					case Operation::AddressingMode::INDIRECT_INDEXED:
						ImGui::Text("%s ($%02x),Y",
							operation.mnemonic.c_str(),
							mNes->dataBus->read(current_pc + 1));
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

void UI::DrawMemoryDebug()
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

	if (start_addr < 0)
	{
		start_addr = 0;
	}

	if (start_addr > 0x400)
	{
		start_addr = 0x400;
	}

	if (ImGui::BeginTable("table_memory", 3))
	{
		for (int row = 0; row < 20; row++)
		{
			uint16_t addr = start_addr + row;

			if (addr < start_addr)
			{
				break;
			}

			ImGui::TableNextRow();

			uint8_t data = mNes->dataBus->read(addr);

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

UI::UI(SDL_Window* mWindow, SDL_Renderer* mRenderer, ImGuiIO& io, NES* mNes)
	: mWindow(mWindow), mRenderer(mRenderer), io(io), mNes(mNes)
{
}

void UI::HandleEvent(SDL_Event* event)
{
	ImGui_ImplSDL2_ProcessEvent(event);
}
