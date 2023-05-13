#include <iostream>
#include <utility>
#include "VideoDataBus.h"

void VideoDataBus::v_write(uint8_t data, uint16_t address)
{

}

uint8_t VideoDataBus::v_read(uint16_t address)
{
	return 0;
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
