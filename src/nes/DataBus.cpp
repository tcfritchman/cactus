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

uint8_t DataBus::peek(uint16_t address)
{
	return RouteToMemoryDevice(address)->peek(address);
}

DataBus::DataBus(std::shared_ptr<RAM> mRam,
	std::shared_ptr<PPU> mPPU,
	std::shared_ptr<APU> mAPU,
	std::shared_ptr<Cartridge> mCart)
	: mRAM(std::move(mRam)), mPPU(std::move(mPPU)), mAPU(std::move(mAPU)), mCart(std::move(mCart))
{
	std::printf("Created DataBus\n");
}

DataBus::~DataBus()
{
	std::printf("Destroyed DataBus\n");
}

std::shared_ptr<MemoryDevice> DataBus::RouteToMemoryDevice(uint16_t address)
{
	if (address >= RAM::START_ADDRESS && address < RAM::END_ADDRESS)
	{
		return mRAM;
	}
	else if (address >= PPU::START_ADDRESS && address < PPU::END_ADDRESS)
	{
		return mPPU;
	}
	else if (address >= APU::START_ADDRESS && address < APU::END_ADDRESS)
	{
		return mAPU;
	}
	else
	{
		return mCart;
	}
}
