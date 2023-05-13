#include "NES.h"
#include "INesRom.h"

NES::NES(const INesRom& rom)
{
	cart = std::make_shared<Cartridge>(rom);
	vram = std::make_shared<VideoRAM>();
	videoDataBus = std::make_shared<VideoDataBus>(cart, vram);
	ppu = std::make_shared<PPU>(videoDataBus);
	ram = std::make_shared<RAM>();
	apu = std::make_shared<APU>();
	dataBus = std::make_shared<DataBus>(ram, ppu, apu, cart);
	cpu = std::make_shared<CPU>(dataBus);
	std::printf("Created NES\n");
}

NES::~NES()
{
	std::printf("Destroyed NES\n");
}

void NES::Tick()
{
	cpu->Cycle();
	ppu->Cycle();
	ppu->Cycle();
	ppu->Cycle();
}
