#include "VideoRAM.h"
#include <iostream>

uint8_t VideoRAM::v_read(uint16_t address)
{
	if (address >= NAMETABLE_RAM_START_ADDRESS && address < NAMETABLE_RAM_END_ADDRESS)
	{
		return nametable_ram[(address - NAMETABLE_RAM_START_ADDRESS) % NAMETABLE_RAM_PHYSICAL_SIZE];
	}
	else if (address >= PALETTE_RAM_START_ADDRESS && address < PALETTE_RAM_END_ADDRESS)
	{
		return palette_ram[(address - PALETTE_RAM_START_ADDRESS) % PALETTE_RAM_PHYSICAL_SIZE];
	} else {
		std::cout << "Illegal read - VRAM" << std::endl;
		exit(1);
	}

}

void VideoRAM::v_write(uint8_t data, uint16_t address)
{
	if (address >= NAMETABLE_RAM_START_ADDRESS && address < NAMETABLE_RAM_END_ADDRESS)
	{
		nametable_ram[(address - NAMETABLE_RAM_START_ADDRESS) % NAMETABLE_RAM_PHYSICAL_SIZE] = data;
	}
	else if (address >= PALETTE_RAM_START_ADDRESS && address < PALETTE_RAM_END_ADDRESS)
	{
		palette_ram[(address - PALETTE_RAM_START_ADDRESS) % PALETTE_RAM_PHYSICAL_SIZE] = data;
	} else {
		std::cout << "Illegal write - VRAM" << std::endl;
		exit(1);
	}
}

VideoRAM::VideoRAM()
{
	std::printf("Created VRAM\n");
}

VideoRAM::~VideoRAM()
{
	std::printf("Destroyed VRAM\n");
}
