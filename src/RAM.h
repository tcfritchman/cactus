#ifndef NES_EMULATOR_SRC_RAM_H_
#define NES_EMULATOR_SRC_RAM_H_

#include "MemoryDevice.h"

class RAM : MemoryDevice
{
 public:
	RAM();
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;
	virtual ~RAM();

 private:
	uint8_t m_test_ram[0x400] =
		{ 0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d, 0x01, 0x02, 0xa9, 0x08, 0x8d, 0x02, 0x02, 0xff };
};

#endif //NES_EMULATOR_SRC_RAM_H_
