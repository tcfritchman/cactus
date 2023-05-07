#include "PPU.h"
#include <iostream>

uint8_t PPU::read(uint16_t address)
{
	return 0;
}

void PPU::write(uint8_t data, uint16_t address)
{

}

PPU::PPU(VideoDataBus* dataBus) : mDataBus(dataBus)
{
	std::printf("Created PPU\n");
}

PPU::~PPU()
{
	std::printf("Destroyed PPU\n");
}

void PPU::Cycle()
{
	// TODO
}
