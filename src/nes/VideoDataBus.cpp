#include <iostream>
#include <utility>
#include "VideoDataBus.h"
#include "Util.h"

void VideoDataBus::v_write(uint8_t data, uint16_t address)
{
	return RouteToMemoryDevice(address)->v_write(data, address);
}

uint8_t VideoDataBus::v_read(uint16_t address)
{
	return RouteToMemoryDevice(address)->v_read(address);
}

VideoDataBus::VideoDataBus(std::shared_ptr<Cartridge> cart, std::shared_ptr<VideoRAM> vram)
	: cart(std::move(cart)), vram(std::move(vram))
{
	std::printf("Created VideoDataBus\n");
}

VideoDataBus::~VideoDataBus()
{
	std::printf("Destroyed VideoDataBus\n");
}

std::shared_ptr<VideoMemoryDevice> VideoDataBus::RouteToMemoryDevice(uint16_t address)
{
	uint16_t wrapped_address = address % VideoRAM::END_ADDRESS;
	if (wrapped_address >= Cartridge::CHR_ROM_START_ADDRESS && wrapped_address < Cartridge::CHR_ROM_END_ADDRESS)
	{
		return cart;
	}
	else if (wrapped_address >= VideoRAM::START_ADDRESS && wrapped_address < VideoRAM::END_ADDRESS)
	{
		return vram;
	}
	else
	{
		nes::log("Invalid Video Memory Address 0x%x", wrapped_address);
		exit(1);
	}
}
