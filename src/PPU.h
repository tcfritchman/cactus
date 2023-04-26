#ifndef NES_EMULATOR_SRC_PPU_H_
#define NES_EMULATOR_SRC_PPU_H_

#include "MemoryDevice.h"

class PPU : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	PPU();
	virtual ~PPU();

	static const uint16_t START_ADDRESS = 0x2000;
	static const uint16_t PHYSICAL_SIZE = 0x8;
	static const uint16_t ADDRESS_COUNT = 0x2000;
	static const uint16_t END_ADDRESS = START_ADDRESS + ADDRESS_COUNT;
};

#endif //NES_EMULATOR_SRC_PPU_H_
