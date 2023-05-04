#include "NES.h"
#include "INesRom.h"

NES::NES(const INesRom& rom)
{
	cart = new Cartridge(rom);
	ppu = new PPU();
	videoDataBus = new VideoDataBus(cart);
	ram = new RAM();
	apu = new APU();
	dataBus = new DataBus(ram, ppu, apu, cart);
	cpu = new CPU(dataBus);
	std::printf("Created NES\n");
}

NES::~NES()
{
	delete (cpu);
	delete (dataBus);
	delete (apu);
	delete (ram);
	delete (videoDataBus);
	delete (ppu);
	delete (cart);
	std::printf("Destroyed NES\n");
}
