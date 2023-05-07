#include "NES.h"
#include "INesRom.h"

NES::NES(const INesRom& rom)
{
	cart = new Cartridge(rom);
	vram = new VideoRAM();
	videoDataBus = new VideoDataBus(cart, vram);
	ppu = new PPU(videoDataBus);
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
	delete (ppu);
	delete (videoDataBus);
	delete (vram);
	delete (cart);
	std::printf("Destroyed NES\n");
}

void NES::Tick()
{
	cpu->Cycle();
	ppu->Cycle();
	ppu->Cycle();
	ppu->Cycle();
}
