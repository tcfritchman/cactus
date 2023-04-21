#include "RAM.h"
#include <iostream>

uint8_t RAM::read(uint16_t address)
{
	if (address >= 0 && address < 0x400)
	{
		return m_test_ram[address];
	} else
	{
		return 0;
	}
}

void RAM::write(uint8_t data, uint16_t address)
{
	if (address >= 0 && address < 0x400)
	{
		m_test_ram[address] = data;
	} else
	{
		std::cout << "Illegal write" << std::endl;
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
