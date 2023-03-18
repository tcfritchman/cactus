#ifndef NES_EMULATOR__DATABUS_H_
#define NES_EMULATOR__DATABUS_H_

#include <cstdint>

class DataBus
{
 public:
	uint8_t read(uint16_t address);
	void write(uint8_t data, uint16_t address);

 private:
	uint8_t m_test_ram[16] = {0xA9,0xEE,0x6D,0x00,0x08,0x00,0x00,0x00,
							  0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
};

#endif //NES_EMULATOR__DATABUS_H_
