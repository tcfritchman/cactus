#include "Cartridge.h"
#include <iostream>

// NROM Cart

uint8_t Cartridge::read(uint16_t address)
{
	int index = (address - PRG_ROM_START) % prg_rom_size;
	return prg_rom[index];
}

void Cartridge::write(uint8_t data, uint16_t address)
{
	std::printf("Illegal write 0x%x - Cartridge\n", address);
}

uint8_t Cartridge::peek(uint16_t address)
{
	int index = (address - PRG_ROM_START) % prg_rom_size;
	return prg_rom[index];
}

uint8_t Cartridge::v_read(uint16_t address)
{
	if (address < CHR_ROM_ADDRESS_COUNT)
	{
		int index = address % chr_rom_size;
		return chr_rom[index];
	}
	std::printf("Illegal video read 0x%x - Cartridge\n", address);
	return 0;
}

void Cartridge::v_write(uint8_t data, uint16_t address)
{
	std::printf("Illegal video write 0x%x - Cartridge\n", address);
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
