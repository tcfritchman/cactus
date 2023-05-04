#ifndef NES_EMULATOR_SRC_VIDEOMEMORYDEVICE_H_
#define NES_EMULATOR_SRC_VIDEOMEMORYDEVICE_H_

#include <cstdint>

class VideoMemoryDevice
{
 public:
	virtual uint8_t v_read(uint16_t address) = 0;
	virtual void v_write(uint8_t data, uint16_t address) = 0;
};

#endif //NES_EMULATOR_SRC_VIDEOMEMORYDEVICE_H_
