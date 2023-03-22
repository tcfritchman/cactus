#ifndef NES_EMULATOR_SRC_RAM_H_
#define NES_EMULATOR_SRC_RAM_H_

#include "MemoryDevice.h"

class RAM : MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;
 private:
	uint8_t m_test_ram[16] = {0xA9,0xEE,0x6D,0x00,0x08,0x00,0x00,0x00,
							  0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
};

#endif //NES_EMULATOR_SRC_RAM_H_
