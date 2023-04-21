#ifndef NES_EMULATOR_SRC_DATABUS_H_
#define NES_EMULATOR_SRC_DATABUS_H_

#include <cstdint>
#include "RAM.h"

class DataBus
{
 public:
	uint8_t read(uint16_t address);
	void write(uint8_t data, uint16_t address);

	explicit DataBus(RAM* mRam);
	DataBus() = delete;
	virtual ~DataBus();

 private:
	RAM* mRam;
};

#endif //NES_EMULATOR_SRC_DATABUS_H_
