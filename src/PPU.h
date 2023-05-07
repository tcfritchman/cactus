#ifndef NES_EMULATOR_SRC_PPU_H_
#define NES_EMULATOR_SRC_PPU_H_

#include "MemoryDevice.h"
#include "VideoDataBus.h"

class PPU : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	PPU(VideoDataBus* dataBus);
	virtual ~PPU();

	// PPU Register CPU Memory Map
	static const uint16_t START_ADDRESS = 0x2000;
	static const uint16_t PHYSICAL_SIZE = 0x8;
	static const uint16_t ADDRESS_COUNT = 0x2000;
	static const uint16_t END_ADDRESS = START_ADDRESS + ADDRESS_COUNT;

	// PPU Registers
	static const uint16_t PPUCTRL = 0x2000;
	static const uint16_t PPUMASK = 0x2001;
	static const uint16_t PPUSTATUS = 0x2002;
	static const uint16_t OAMADDR = 0x2003;
	static const uint16_t OAMDATA = 0x2004;
	static const uint16_t PPUSCROLL = 0x2005;
	static const uint16_t PPUADDR = 0x2006;
	static const uint16_t PPUDATA = 0x2007;
	static const uint16_t OAMDMA = 0x4014;

 private:
	VideoDataBus* mDataBus;

	uint8_t ReadPPUSTATUS();
	uint8_t ReadOAMDATA();
	uint8_t ReadPPUDATA();

	void WritePPUCTRL(uint8_t value);
	void WritePPUMASK(uint8_t value);
	void WriteOAMADDR(uint8_t value);
	void WriteOAMDATA(uint8_t value);
	void WritePPUSCROLL(uint8_t value);
	void WritePPUADDR(uint8_t value);
	void WritePPUDATA(uint8_t value);
	void WritePPUDMA(uint8_t value);

	// TODO: NMI HANDLER
	// TODO: Include render completed handler
};

#endif //NES_EMULATOR_SRC_PPU_H_
