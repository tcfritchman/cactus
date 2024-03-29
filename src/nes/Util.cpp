#include <fstream>
#include <vector>
#include <sstream>
#include "Util.h"

uint8_t nes::hi_byte(uint16_t value)
{
	return value >> 8;
}

uint8_t nes::lo_byte(uint16_t value)
{
	return value;
}

uint16_t nes::address(uint8_t lo_byte, uint8_t hi_byte)
{
	return (static_cast<uint16_t>(hi_byte) << 8) | (uint16_t)lo_byte;
}

bool nes::test_bit(uint8_t value, int bit)
{
	return (1 << bit) & value;
}

void nes::set_bit(std::unique_ptr<uint8_t> value, int bit)
{
	*value = (1 << bit) & *value;
}

void nes::clear_bit(std::unique_ptr<uint8_t> value, int bit)
{
	*value = (~(1 << bit)) & *value;
}

std::vector<uint8_t> nes::read_file_bytes(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	if (pos == 0)
	{
		return std::vector<uint8_t>{};
	}

	std::vector<char> result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(result.data(), pos);

	// Convert to vector<uint8_t>
	auto s = reinterpret_cast<uint8_t*>(result.data());
	return std::vector<uint8_t>{ s, s + pos };
}

std::vector<std::string> nes::read_file_lines(const std::string& filename)
{
	std::fstream file(filename);
	std::vector<std::string> vec;
	std::string str;

	while (getline(file, str))
	{
		vec.push_back(str);
	}

	return vec;
}

std::string nes::hex(uint8_t uint8)
{
	std::stringstream stream;
	stream << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(uint8);
	return stream.str();
}

std::string nes::hex(uint16_t uint16)
{
	std::stringstream stream;
	stream << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << uint16;
	return stream.str();
}

void nes::log(const char* format, ...)
{
	va_list argp;
	va_start(argp, format);
	vprintf(format, argp);
	putchar('\n');
	va_end(argp);
}
