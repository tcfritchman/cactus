#include "nes/NES.h"
#include "nes/Util.h"

#include <iomanip>
#include <sstream>

std::string GetOpCodesStr(const std::vector<uint8_t>& op_codes)
{
	std::stringstream stream;
	for (size_t i = 0; i < 3; i++)
	{
		if (i < op_codes.size())
		{
			// cast to uint16 to avoid char interpretation
			stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(op_codes[i]);
		}
		else
		{
			stream << "  ";
		}
		if (i != 2)
		{
			stream << " ";
		}
	}
	return stream.str();
}

void PrintCpuState(NES& nes) {
	auto state = nes.cpu->GetState();
	auto op = CPU::GetOperation(nes.dataBus->read(state.PC));
	auto op_codes = nes.cpu->GetCurrentOpcodes();

	std::printf("%04X  %s  %s %s A:%02X X:%02X Y:%02X P:%02X SP:%0X\n",
		state.PC,
		GetOpCodesStr(op_codes).c_str(),
		op.mnemonic.c_str(),
		"",
		state.A,
		state.X,
		state.Y,
		state.P,
		state.SP);
}

int main(int argc, char* argv[])
{
	auto filename = argv[1]; // nestest.nes
	auto bytes = nes::read_file_bytes(filename);
	auto rom = std::make_unique<INesRom>(bytes);
	auto nes = std::make_unique<NES>(*rom);
	auto cpu = nes->cpu;
	cpu->SetPC(0xC000);

	for (int i = 0; i < 100; i++) {
		cpu->Cycle();
		PrintCpuState(*nes);
	}
}
