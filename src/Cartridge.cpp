#include "Cartridge.h"
#include <iostream>

uint8_t Cartridge::read(uint16_t address)
{
	if (address >= PRG_ROM_START) {
		int index = (address - PRG_ROM_START) % prg_rom_size;
		return prg_rom[index];
	}
	std::printf("Illegal read - Cartridge\n");
	return 0;
}

void Cartridge::write(uint8_t data, uint16_t address)
{
	std::printf("Illegal write - Cartridge\n");
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
	prg_rom_size = rom.GetProgramRomSize();
	std::printf("Created Cartridge\n");
}