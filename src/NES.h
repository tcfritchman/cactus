#ifndef NES_EMULATOR_SRC_NES_H_
#define NES_EMULATOR_SRC_NES_H_

#include "DataBus.h"
#include "CPU.h"

class NES
{
 public:
	RAM* ram;
	PPU* ppu;
	APU* apu;
	Cartridge* cart;
	DataBus* dataBus;
	CPU* cpu;

	NES();
	~NES();
};

#endif //NES_EMULATOR_SRC_NES_H_
