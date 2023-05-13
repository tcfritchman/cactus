#include "Cartridge.h"
#include <iostream>

// NROM Cart

uint8_t Cartridge::read(uint16_t address)
{
	if (address >= PRG_ROM_START)
	{
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

uint8_t Cartridge::v_read(uint16_t address)
{
	if (address < CHR_ROM_ADDRESS_COUNT)
	{
		int index = address % chr_rom_size;
		return chr_rom[index];
	}
	std::printf("Illegal video read - Cartridge\n");
	return 0;
}

void Cartridge::v_write(uint8_t data, uint16_t address)
{
	std::printf("Illegal video write - Cartridge\n");
}

Cartridge::~Cartridge()
{
	std::printf("Destroyed Cartridge\n");
}

Cartridge::Cartridge(const INesRom& rom)
{
	// NROM boards
	if (rom.GetMapperNumber() != 0)
	{
		std::printf("Unsupported mapper %d\n", rom.GetMapperNumber());
		exit(1);
	}
	prg_rom = rom.GetProgramRomData();
	chr_rom = rom.GetCharacterRomData();
	prg_rom_size = rom.GetProgramRomSize();
	chr_rom_size = rom.GetCharacterRomSize();
	std::printf("Created Cartridge\n");
}
