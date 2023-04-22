#include "Cartridge.h"
#include <iostream>

uint8_t Cartridge::read(uint16_t address)
{
	return 0;
}

void Cartridge::write(uint8_t data, uint16_t address)
{

}

Cartridge::Cartridge()
{
	std::printf("Created Cartridge\n");
}

Cartridge::~Cartridge()
{
	std::printf("Destroyed Cartridge\n");
}
