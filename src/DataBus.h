#ifndef NES_EMULATOR__DATABUS_H_
#define NES_EMULATOR__DATABUS_H_

#include <cstdint>
#include "RAM.h"

class DataBus
{
 public:
	uint8_t read(uint16_t address);
	void write(uint8_t data, uint16_t address);

	explicit DataBus(const RAM& mRam);

 private:
	RAM mRam;
};

#endif //NES_EMULATOR__DATABUS_H_
