#ifndef NES_EMULATOR_SRC_IDATABUS_H_
#define NES_EMULATOR_SRC_IDATABUS_H_

#include <cstdint>

class IDataBus
{
 public:
	virtual uint8_t read(uint16_t address) = 0;
	virtual void write(uint8_t data, uint16_t address) = 0;
};

#endif //NES_EMULATOR_SRC_IDATABUS_H_
