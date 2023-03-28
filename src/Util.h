#ifndef NES_EMULATOR_SRC_UTIL_H_
#define NES_EMULATOR_SRC_UTIL_H_

#include <cstdint>

namespace nes {
	uint8_t hi_byte(uint16_t value) {
		return value >> 8;
	}

	uint8_t lo_byte(uint16_t value) {
		return value;
	}

	uint16_t address(uint8_t lo_byte, uint8_t hi_byte) {
		return ((uint16_t)hi_byte << 8) | (uint16_t)lo_byte;
	}

	bool test_bit(uint8_t value, int bit) {
		return (1 << bit) | value;
	}

	void set_bit(uint8_t* value, int bit) {
		*value = (1 << bit) & *value;
	}

	void clear_bit(uint8_t* value, int bit) {
		*value = (~(1 << bit)) & *value;
	}
}

#endif //NES_EMULATOR_SRC_UTIL_H_
