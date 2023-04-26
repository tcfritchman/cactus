#ifndef NES_EMULATOR_SRC_DATABUS_H_
#define NES_EMULATOR_SRC_DATABUS_H_

#include <cstdint>
#include "RAM.h"
#include "PPU.h"
#include "APU.h"
#include "Cartridge.h"

class DataBus
{
 public:
	uint8_t read(uint16_t address);
	void write(uint8_t data, uint16_t address);

	explicit DataBus(RAM* mRam, PPU* mPPU, APU* mAPU, Cartridge* mCart);
	DataBus() = delete;
	virtual ~DataBus();

 private:
	RAM* mRAM;
	PPU* mPPU;
	APU* mAPU;
	Cartridge* mCart;

	MemoryDevice* RouteToMemoryDevice(uint16_t address);
};

#endif //NES_EMULATOR_SRC_DATABUS_H_
