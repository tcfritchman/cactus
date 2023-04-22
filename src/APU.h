#ifndef NES_EMULATOR_SRC_APU_H_
#define NES_EMULATOR_SRC_APU_H_

#include "MemoryDevice.h"

class APU : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	APU();
	virtual ~APU();
};

#endif //NES_EMULATOR_SRC_APU_H_
