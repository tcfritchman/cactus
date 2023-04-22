#ifndef NES_EMULATOR_SRC_CARTRIDGE_H_
#define NES_EMULATOR_SRC_CARTRIDGE_H_

#include "MemoryDevice.h"

class Cartridge : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	Cartridge();
	virtual ~Cartridge();
};

#endif //NES_EMULATOR_SRC_CARTRIDGE_H_
