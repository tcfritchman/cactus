#include <iostream>
#include "DataBus.h"

uint8_t DataBus::read(uint16_t address)
{
	if (address >= 0 && address < 16)
	{
		return m_test_ram[address];
	} else
	{
		std::cout << "Illegal read" << std::endl;
		return 0;
	}
}

void DataBus::write(uint8_t data, uint16_t address)
{
	if (address >= 0 && address < 16)
	{
		m_test_ram[address] = data;
	} else
	{
		std::cout << "Illegal write" << std::endl;
	}
}
