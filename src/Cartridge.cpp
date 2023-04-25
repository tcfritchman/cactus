#include "Cartridge.h"
#include <iostream>

uint8_t Cartridge::read(uint16_t address)
{
	return 0;
}

void Cartridge::write(uint8_t data, uint16_t address)
{

}

Cartridge::~Cartridge()
{
	std::printf("Destroyed Cartridge\n");
}

Cartridge::Cartridge(const INesRom& rom)
{
	// NROM boards
	if (rom.GetMapperNumber() != 0) {
		std::printf("Unsupported mapper %d\n", rom.GetMapperNumber());
		exit(1);
	}
	prg_rom = rom.GetProgramRomData();
	chr_rom = rom.GetCharacterRomData();
	std::printf("Created Cartridge\n");
}