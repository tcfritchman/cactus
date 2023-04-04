#include "DataBus.h"

#include <utility>

uint8_t DataBus::read(uint16_t address)
{
	return mRam.read(address);
}

void DataBus::write(uint8_t data, uint16_t address)
{
	return mRam.write(data, address);
}

DataBus::DataBus(RAM mRam) : mRam(std::move(mRam))
{
}
