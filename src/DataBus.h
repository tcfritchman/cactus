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

	static const uint16_t RAM_START_ADDRESS = 0;
	static const uint16_t RAM_PHYSICAL_SIZE = 0x800;
	static const uint16_t RAM_ADDRESS_COUNT = 0x2000;
	static const uint16_t RAM_END_ADDRESS = RAM_START_ADDRESS + RAM_ADDRESS_COUNT;

	static const uint16_t PPU_START_ADDRESS = 0x2000;
	static const uint16_t PPU_PHYSICAL_SIZE = 0x8;
	static const uint16_t PPU_ADDRESS_COUNT = 0x2000;
	static const uint16_t PPU_END_ADDRESS = PPU_START_ADDRESS + PPU_ADDRESS_COUNT;

	static const uint16_t APU_START_ADDRESS = 0x4000;
	static const uint16_t APU_PHYSICAL_SIZE = 0x20;
	static const uint16_t APU_ADDRESS_COUNT = APU_PHYSICAL_SIZE;
	static const uint16_t APU_END_ADDRESS = APU_START_ADDRESS + APU_ADDRESS_COUNT;

	static const uint16_t CART_START_ADDRESS = 0x4020;
	static const uint16_t CART_ADDRESS_COUNT = 0xBFE0;


 private:
	RAM* mRAM;
	PPU* mPPU;
	APU* mAPU;
	Cartridge* mCart;

	MemoryDevice* RouteToMemoryDevice(uint16_t address);
};

#endif //NES_EMULATOR_SRC_DATABUS_H_
