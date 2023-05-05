#ifndef NES_EMULATOR_SRC_VIDEODATABUS_H_
#define NES_EMULATOR_SRC_VIDEODATABUS_H_

#include "VideoMemoryDevice.h"
#include "Cartridge.h"
#include "VideoRAM.h"

class VideoDataBus : public VideoMemoryDevice
{
 public:
	uint8_t v_read(uint16_t address) override;
	void v_write(uint8_t data, uint16_t address) override;

	explicit VideoDataBus(Cartridge* cart, VideoRAM* vram);
	VideoDataBus() = delete;
	~VideoDataBus();

 private:
	Cartridge* cart;
	VideoRAM* vram;
};

#endif //NES_EMULATOR_SRC_VIDEODATABUS_H_
