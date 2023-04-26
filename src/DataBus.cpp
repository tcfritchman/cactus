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
	if (address >= RAM::START_ADDRESS && address < RAM::END_ADDRESS) {
		return mRAM;
	} else if (address >= PPU::START_ADDRESS && address < PPU::END_ADDRESS) {
		return mPPU;
	} else if (address >= APU::START_ADDRESS && address < APU::END_ADDRESS) {
		return mAPU;
	} else {
		return mCart;
	}
}
