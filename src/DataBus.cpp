#include <cstdio>
#include "DataBus.h"

uint8_t DataBus::read(uint16_t address)
{
	return mRam->read(address);
}

void DataBus::write(uint8_t data, uint16_t address)
{
	return mRam->write(data, address);
}

DataBus::DataBus(RAM* mRam) : mRam(mRam)
{
	std::printf("Created DataBus\n");
}

DataBus::~DataBus()
{
	std::printf("Destroyed DataBus\n");
}
