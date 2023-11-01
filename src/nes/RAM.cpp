#include "RAM.h"
#include <iostream>

uint8_t RAM::read(uint16_t address)
{
	// Internal ram is 2KB, mirrored
	return mRamInternal[address % 0x800];
}

void RAM::write(uint8_t data, uint16_t address)
{
	mRamInternal[address % 0x800] = data;
}

uint8_t RAM::peek(uint16_t address)
{
	return mRamInternal[address % 0x800];
}

RAM::RAM()
{
	std::printf("Created RAM\n");
}

RAM::~RAM()
{
	std::printf("Destroyed RAM\n");
}
