#include "PPU.h"
#include <iostream>

uint8_t PPU::read(uint16_t address)
{
	return 0;
}

void PPU::write(uint8_t data, uint16_t address)
{

}

PPU::PPU()
{
	std::printf("Created PPU\n");
}

PPU::~PPU()
{
	std::printf("Destroyed PPU\n");
}
