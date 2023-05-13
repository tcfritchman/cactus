#ifndef NES_EMULATOR_SRC_VIDEORAM_H_
#define NES_EMULATOR_SRC_VIDEORAM_H_

#include "VideoMemoryDevice.h"
#include <vector>

class VideoRAM : public VideoMemoryDevice
{

 public:
	uint8_t v_read(uint16_t address) override;
	void v_write(uint8_t data, uint16_t address) override;

	VideoRAM();
	virtual ~VideoRAM();

	const uint16_t NAMETABLE_RAM_ADDRESS_COUNT = 0x1F00;
	const uint16_t NAMETABLE_RAM_PHYSICAL_SIZE = 0x1000;
	const uint16_t NAMETABLE_RAM_START_ADDRESS = 0x2000;
	const uint16_t NAMETABLE_RAM_END_ADDRESS = NAMETABLE_RAM_START_ADDRESS + NAMETABLE_RAM_ADDRESS_COUNT;

	const uint16_t PALETTE_RAM_ADDRESS_COUNT = 0x100;
	const uint16_t PALETTE_RAM_PHYSICAL_SIZE = 0x0020;
	const uint16_t PALETTE_RAM_START_ADDRESS = 0x3F00;
	const uint16_t PALETTE_RAM_END_ADDRESS = PALETTE_RAM_START_ADDRESS + PALETTE_RAM_ADDRESS_COUNT;

 private:
	std::vector<uint8_t> nametable_ram = std::vector<uint8_t>(NAMETABLE_RAM_PHYSICAL_SIZE);
	std::vector<uint8_t> palette_ram = std::vector<uint8_t>(PALETTE_RAM_PHYSICAL_SIZE);
};

#endif //NES_EMULATOR_SRC_VIDEORAM_H_
