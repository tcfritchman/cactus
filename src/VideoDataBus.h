#ifndef NES_EMULATOR_SRC_VIDEODATABUS_H_
#define NES_EMULATOR_SRC_VIDEODATABUS_H_

#include "VideoMemoryDevice.h"
#include "Cartridge.h"

class VideoDataBus : public VideoMemoryDevice
{
 public:
	uint8_t v_read(uint16_t address) override;
	void v_write(uint8_t data, uint16_t address) override;

	explicit VideoDataBus(Cartridge* cart);
	VideoDataBus() = delete;
	~VideoDataBus();

 private:
	Cartridge* cart;
};

#endif //NES_EMULATOR_SRC_VIDEODATABUS_H_
