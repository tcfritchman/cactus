#ifndef NES_EMULATOR_SRC_UTIL_H_
#define NES_EMULATOR_SRC_UTIL_H_

namespace nes
{
	uint8_t hi_byte(uint16_t value);

	uint8_t lo_byte(uint16_t value);

	uint16_t address(uint8_t lo_byte, uint8_t hi_byte);

	bool test_bit(uint8_t value, int bit);

	void set_bit(std::unique_ptr<uint8_t> value, int bit);

	void clear_bit(std::unique_ptr<uint8_t> value, int bit);

	std::vector<uint8_t> read_file_bytes(const std::string& filename);

	std::vector<std::string> read_file_lines(const std::string& filename);

	std::string hex(uint8_t uint8);

	std::string hex(uint16_t uint16);

	void log(const char* format, ...);
}

#endif //NES_EMULATOR_SRC_UTIL_H_
