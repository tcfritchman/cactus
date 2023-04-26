#ifndef NES_EMULATOR_SRC_CARTRIDGE_H_
#define NES_EMULATOR_SRC_CARTRIDGE_H_

#include "MemoryDevice.h"
#include "INesRom.h"

class Cartridge : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	explicit Cartridge(const INesRom& rom);
	Cartridge() = delete;
	virtual ~Cartridge();

	static const uint16_t START_ADDRESS = 0x4020;
	static const uint16_t ADDRESS_COUNT = 0xBFE0;
	static const int PRG_ROM_START = 0x8000;

 private:
	std::vector<uint8_t> prg_rom;
	std::vector<uint8_t> chr_rom;
	int prg_rom_size;
};

#endif //NES_EMULATOR_SRC_CARTRIDGE_H_
