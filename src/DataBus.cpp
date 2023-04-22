#include <cstdio>
#include "DataBus.h"

uint8_t DataBus::read(uint16_t address)
{
	return RouteToMemoryDevice(address)->read(address);
}

void DataBus::write(uint8_t data, uint16_t address)
{
	return RouteToMemoryDevice(address)->write(data, address);
}

DataBus::DataBus(RAM* mRam, PPU* mPPU, APU* mAPU, Cartridge* mCart) : mRAM(mRam), mPPU(mPPU), mAPU(mAPU), mCart(mCart)
{
	std::printf("Created DataBus\n");
}

DataBus::~DataBus()
{
	std::printf("Destroyed DataBus\n");
}

MemoryDevice* DataBus::RouteToMemoryDevice(uint16_t address)
{
	if (address >= RAM_START_ADDRESS && address < RAM_END_ADDRESS) {
		return mRAM;
	} else if (address >= PPU_START_ADDRESS && address < PPU_END_ADDRESS) {
		return mPPU;
	} else if (address >= APU_START_ADDRESS && address < APU_END_ADDRESS) {
		return mAPU;
	} else {
		return mCart;
	}
}
