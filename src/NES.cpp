#include "NES.h"

NES::NES()
{
	ram = new RAM();
	ppu = new PPU();
	apu = new APU();
	cart = new Cartridge();
	dataBus = new DataBus(ram, ppu, apu, cart);
	cpu = new CPU(dataBus);
	std::printf("Created NES\n");
}

NES::~NES()
{
	delete (ram);
	delete (ppu);
	delete (apu);
	delete (cart);
	delete (dataBus);
	delete (cpu);
	std::printf("Destroyed NES\n");
}
