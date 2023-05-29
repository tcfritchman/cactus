#ifndef NES_EMULATOR_SRC_DATABUS_H_
#define NES_EMULATOR_SRC_DATABUS_H_

#include <cstdint>
#include "RAM.h"
#include "PPU.h"
#include "APU.h"
#include "Cartridge.h"
#include "MemoryDevice.h"

class DataBus : public MemoryDevice
{
 public:
	uint8_t read(uint16_t address) override;
	void write(uint8_t data, uint16_t address) override;

	DataBus(std::shared_ptr<RAM> mRam,
		std::shared_ptr<PPU> mPPU,
		std::shared_ptr<APU> mAPU,
		std::shared_ptr<Cartridge> mCart);
	DataBus() = delete;
	virtual ~DataBus();

 private:
	std::shared_ptr<RAM> mRAM;
	std::shared_ptr<PPU> mPPU;
	std::shared_ptr<APU> mAPU;
	std::shared_ptr<Cartridge> mCart;

	std::shared_ptr<MemoryDevice> RouteToMemoryDevice(uint16_t address);
};

#endif //NES_EMULATOR_SRC_DATABUS_H_
