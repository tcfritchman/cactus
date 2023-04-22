#include <iostream>
#include "INesRom.h"
#include "Util.h"

INesRom::INesRom(const std::vector<char>& bytes) : bytes(bytes)
{
	prg_rom_start = HEADER_SIZE
		+ (HasTrainer() ? TRAINER_SIZE : 0);
	chr_rom_start = prg_rom_start + GetProgramRomSize();

	std::printf("Created INesRom - Mapper: %d, PrgRomsize: %d, ChrRomSize %d\n",
		GetMapperNumber(),
		GetProgramRomSize(),
		GetCharacterRomSize());
}

int INesRom::GetProgramRomSize()
{
	return bytes[PRG_ROM_SIZE_BYTE] * PRG_ROM_BLOCK_SIZE;
}

int INesRom::GetCharacterRomSize()
{
	return bytes[CHR_ROM_SIZE_BYTE] * CHR_ROM_BLOCK_SIZE;
}

int INesRom::GetMapperNumber()
{
	return (bytes[FLAGS_7_BYTE] & 0xF0) | (bytes[FLAGS_6_BYTE] >> 4);
}

bool INesRom::HasTrainer()
{
	return nes::test_bit(bytes[FLAGS_6_BYTE], TRAINER_BIT);
}

std::vector<char> INesRom::GetProgramRomData()
{
	auto start = bytes.begin() + prg_rom_start;
	auto end = start + GetProgramRomSize();
	return std::vector<char>{ start, end };
}

std::vector<char> INesRom::GetCharacterRomData()
{
	auto start = bytes.begin() + chr_rom_start;
	auto end = start + GetCharacterRomSize();
	return std::vector<char>{ start, end };
}
