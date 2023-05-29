#include "nes/NES.h"
#include "nes/Util.h"

int main(int argc, char* argv[])
{
	auto filename = argv[1];
	auto bytes = nes::read_file_bytes(filename);
	auto rom = std::make_unique<INesRom>(bytes);
	auto nes = std::make_unique<NES>(*rom);

	auto state = nes->cpu->GetState();

	nes::log("%d", state.A);
}