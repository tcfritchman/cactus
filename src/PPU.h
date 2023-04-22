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
};

#endif //NES_EMULATOR_SRC_PPU_H_
