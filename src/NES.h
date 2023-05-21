#ifndef NES_EMULATOR_SRC_NES_H_
#define NES_EMULATOR_SRC_NES_H_

#include "DataBus.h"
#include "CPU.h"
#include "VideoDataBus.h"

class NES
{
 public:
	std::shared_ptr<RAM> ram;
	std::shared_ptr<PPU> ppu;
	std::shared_ptr<APU> apu;
	std::shared_ptr<Cartridge> cart;
	std::shared_ptr<DataBus> dataBus;
	std::shared_ptr<VideoRAM> vram;
	std::shared_ptr<VideoDataBus> videoDataBus;
	std::shared_ptr<CPU> cpu;

	explicit NES(const INesRom& rom);
	NES() = delete;
	~NES();

	void Cycle();

 private:
	long mCycleCount = 0;
};

#endif //NES_EMULATOR_SRC_NES_H_
