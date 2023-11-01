#ifndef NES_EMULATOR_SRC_RAM_H_
#define NES_EMULATOR_SRC_RAM_H_

#include "MemoryDevice.h"

class RAM : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;
	uint8_t peek(uint16_t address) override;

	RAM();
	virtual ~RAM();

	static const uint16_t START_ADDRESS = 0;
	static const uint16_t PHYSICAL_SIZE = 0x800;
	static const uint16_t ADDRESS_COUNT = 0x2000;
	static const uint16_t END_ADDRESS = START_ADDRESS + ADDRESS_COUNT;

 private:
	uint8_t mRamInternal[PHYSICAL_SIZE] = { 0 };
};

#endif //NES_EMULATOR_SRC_RAM_H_
