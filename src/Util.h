#ifndef NES_EMULATOR_SRC_UTIL_H_
#define NES_EMULATOR_SRC_UTIL_H_

namespace nes
{
	uint8_t hi_byte(uint16_t value);

	uint8_t lo_byte(uint16_t value);

	uint16_t address(uint8_t lo_byte, uint8_t hi_byte);

	bool test_bit(uint8_t value, int bit);

	void set_bit(uint8_t* value, int bit);

	void clear_bit(uint8_t* value, int bit);

	std::vector<char> read_file_bytes(const char* filename);
}

#endif //NES_EMULATOR_SRC_UTIL_H_
