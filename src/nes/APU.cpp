#include "APU.h"
#include <iostream>

uint8_t APU::read(uint16_t address)
{
	return 0;
}

void APU::write(uint8_t data, uint16_t address)
{

}

uint8_t APU::peek(uint16_t address)
{
	return 0;
}

APU::APU()
{
	std::printf("Created APU\n");
}

APU::~APU()
{
	std::printf("Destroyed APU\n");
}
