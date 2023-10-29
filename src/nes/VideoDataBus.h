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

	explicit VideoDataBus(std::shared_ptr<Cartridge> cart, std::shared_ptr<VideoRAM> vram);
	VideoDataBus() = delete;
	~VideoDataBus();

 private:
	std::shared_ptr<Cartridge> cart;
	std::shared_ptr<VideoRAM> vram;

	std::shared_ptr<VideoMemoryDevice> RouteToMemoryDevice(uint16_t address);
};

#endif //NES_EMULATOR_SRC_VIDEODATABUS_H_
