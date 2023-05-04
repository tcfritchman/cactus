#ifndef NES_EMULATOR_SRC_NES_H_
#define NES_EMULATOR_SRC_NES_H_

#include "DataBus.h"
#include "CPU.h"
#include "VideoDataBus.h"

class NES
{
 public:
	RAM* ram;
	PPU* ppu;
	APU* apu;
	Cartridge* cart;
	DataBus* dataBus;
	VideoDataBus* videoDataBus;
	CPU* cpu;

	NES(const INesRom& rom);
	NES() = delete;
	~NES();
};

#endif //NES_EMULATOR_SRC_NES_H_
