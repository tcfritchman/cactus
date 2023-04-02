#ifndef NES_EMULATOR_SRC_RAM_H_
#define NES_EMULATOR_SRC_RAM_H_

#include "MemoryDevice.h"

class RAM : MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;
 private:
	uint8_t m_test_ram[16] = {0xA5,0x60,0x65,0x61,0x85,0x62,0xFF,0xFF,
							  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,};
};

#endif //NES_EMULATOR_SRC_RAM_H_
