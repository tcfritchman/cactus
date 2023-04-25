#include <iostream>
#include "INesRom.h"
#include "Util.h"

INesRom::INesRom(const std::vector<uint8_t>& bytes) : bytes(bytes)
{
	prg_rom_start = HEADER_SIZE
		+ (HasTrainer() ? TRAINER_SIZE : 0);
	chr_rom_start = prg_rom_start + GetProgramRomSize();

	std::printf("Created INesRom - Mapper: %d, PrgRomsize: %d, ChrRomSize %d\n",
		GetMapperNumber(),
		GetProgramRomSize(),
		GetCharacterRomSize());
}

INesRom::~INesRom()
{
	std::printf("Destroyed INesRom\n");
}

int INesRom::GetProgramRomSize() const
{
	return bytes[PRG_ROM_SIZE_BYTE] * PRG_ROM_BLOCK_SIZE;
}

int INesRom::GetCharacterRomSize() const
{
	return bytes[CHR_ROM_SIZE_BYTE] * CHR_ROM_BLOCK_SIZE;
}

int INesRom::GetMapperNumber() const
{
	return (bytes[FLAGS_7_BYTE] & 0xF0) | (bytes[FLAGS_6_BYTE] >> 4);
}

bool INesRom::HasTrainer() const
{
	return nes::test_bit(bytes[FLAGS_6_BYTE], TRAINER_BIT);
}

std::vector<uint8_t> INesRom::GetProgramRomData() const
{
	auto start = bytes.begin() + prg_rom_start;
	auto end = start + GetProgramRomSize();
	return std::vector<uint8_t>{ start, end };
}

std::vector<uint8_t> INesRom::GetCharacterRomData() const
{
	auto start = bytes.begin() + chr_rom_start;
	auto end = start + GetCharacterRomSize();
	return std::vector<uint8_t>{ start, end };
}
