#include "RAM.h"
#include <iostream>

uint8_t RAM::read(uint16_t address)
{
	if (address >= 0 && address < 0x2000)
	{
		// Internal ram is 2KB, mirrored
		return mRamInternal[address % 0x800];
	}
	else
	{
		std::cout << "Illegal read - RAM" << std::endl;
		exit(1);
	}
}

void RAM::write(uint8_t data, uint16_t address)
{
	if (address >= 0 && address < 0x2000)
	{
		mRamInternal[address % 0x800] = data;
	}
	else
	{
		std::cout << "Illegal write - RAM" << std::endl;
		exit(1);
	}
}

RAM::RAM()
{
	std::printf("Created RAM\n");
}

RAM::~RAM()
{
	std::printf("Destroyed RAM\n");
}
