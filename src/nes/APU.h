#ifndef NES_EMULATOR_SRC_APU_H_
#define NES_EMULATOR_SRC_APU_H_

#include "MemoryDevice.h"

class APU : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;
	uint8_t peek(uint16_t address) override;

	APU();
	virtual ~APU();

	static const uint16_t START_ADDRESS = 0x4000;
	static const uint16_t PHYSICAL_SIZE = 0x20;
	static const uint16_t ADDRESS_COUNT = PHYSICAL_SIZE;
	static const uint16_t END_ADDRESS = START_ADDRESS + ADDRESS_COUNT;

};

#endif //NES_EMULATOR_SRC_APU_H_
