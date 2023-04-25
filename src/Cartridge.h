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

 private:
	std::vector<uint8_t> prg_rom;
	std::vector<uint8_t> chr_rom;
};

#endif //NES_EMULATOR_SRC_CARTRIDGE_H_
