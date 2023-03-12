#ifndef NES_EMULATOR__DATABUS_H_
#define NES_EMULATOR__DATABUS_H_

#include <cstdint>

class DataBus
{
	uint8_t read(uint16_t address);
	void write(uint8_t data, uint16_t address);

};

#endif //NES_EMULATOR__DATABUS_H_
