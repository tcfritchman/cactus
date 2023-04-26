#ifndef NES_EMULATOR_SRC_RAM_H_
#define NES_EMULATOR_SRC_RAM_H_

#include "MemoryDevice.h"

class RAM : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	RAM();
	virtual ~RAM();

	static const uint16_t START_ADDRESS = 0;
	static const uint16_t PHYSICAL_SIZE = 0x800;
	static const uint16_t ADDRESS_COUNT = 0x2000;
	static const uint16_t END_ADDRESS = START_ADDRESS + ADDRESS_COUNT;

 private:
	uint8_t mRamInternal[0x800] = { 0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d, 0x01, 0x02, 0xa9, 0x08, 0x8d, 0x02, 0x02, 0xff };
	uint8_t mRamPPU[0x8] = { 0 };
	uint8_t mRamAPU[0x20] = { 0 }; // Includes Test Mode space

};

#endif //NES_EMULATOR_SRC_RAM_H_
