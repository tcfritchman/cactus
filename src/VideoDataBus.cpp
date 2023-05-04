#include <iostream>
#include "VideoDataBus.h"



void VideoDataBus::v_write(uint8_t data, uint16_t address)
{

}

uint8_t VideoDataBus::v_read(uint16_t address)
{
	return 0;
}

VideoDataBus::VideoDataBus(Cartridge* cart) : cart(cart)
{
	std::printf("Created VideoDataBus\n");
}

VideoDataBus::~VideoDataBus()
{
	std::printf("Destroyed VideoDataBus\n");
}
