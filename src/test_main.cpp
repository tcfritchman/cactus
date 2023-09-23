#include "nes/NES.h"
#include "nes/Util.h"

#include <iomanip>
#include <iostream>
#include <sstream>

std::string GetOpCodesStr(const std::vector<uint8_t>& op_codes)
{
	std::stringstream stream;
	for (size_t i = 0; i < 3; i++)
	{
		if (i < op_codes.size())
		{
			// cast to uint16 to avoid char interpretation
			stream << std::uppercase << std::setfill('0') << std::setw(2)
				   << std::hex << static_cast<uint16_t>(op_codes[i]);
		}
		else
		{
			stream << "  ";
		}
		if (i != 2)
		{
			stream << " ";
		}
	}
	return stream.str();
}

std::string GetInstructionStr(uint8_t op_code)
{
	auto op = CPU::GetOperation(op_code);
	return op.mnemonic;
}

std::string GetAddrModeStr(uint8_t op_code)
{
	auto op = CPU::GetOperation(op_code);
	return op.addressing_mode_name;
}

class TestLog
{
	uint16_t pc;
	std::vector<uint8_t> op_codes;
	uint8_t a;
	uint8_t x;
	uint8_t y;
	uint8_t p;
	uint8_t sp;

 public:
	TestLog(uint16_t pc, const std::vector<uint8_t>& opCodes, uint8_t a, uint8_t x, uint8_t y, uint8_t p, uint8_t sp)
		: pc(pc), op_codes(opCodes), a(a), x(x), y(y), p(p), sp(sp)
	{
	}

	bool operator==(const TestLog& rhs) const
	{
		return pc == rhs.pc &&
			op_codes == rhs.op_codes &&
			a == rhs.a &&
			x == rhs.x &&
			y == rhs.y &&
			p == rhs.p &&
			sp == rhs.sp;
	}

	bool operator!=(const TestLog& rhs) const
	{
		return !(rhs == *this);
	}

	friend std::ostream& operator<<(std::ostream& os, const TestLog& log)
	{
		os << "PC: " << nes::hex(log.pc)
		   << " |"
		   << " INST: " << GetInstructionStr(log.op_codes[0])
		   << " |"
		   << " MODE: " << std::setw(16) << GetAddrModeStr(log.op_codes[0])
		   << " |"
		   << " OPS: " << GetOpCodesStr(log.op_codes)
		   << " |"
		   << " A: " << nes::hex(log.a)
		   << " X: " << nes::hex(log.x)
		   << " Y: " << nes::hex(log.y)
		   << " |"
		   << " C: " << nes::test_bit(log.p, 0)
		   << " Z: " << nes::test_bit(log.p, 1)
		   << " I: " << nes::test_bit(log.p, 2)
		   << " D: " << nes::test_bit(log.p, 3)
		   << " B: " << nes::test_bit(log.p, 4)
		   << " V: " << nes::test_bit(log.p, 6)
		   << " N: " << nes::test_bit(log.p, 7)
		   << " |"
		   << " SP: " << nes::hex(log.sp);
		return os;
	}
};

TestLog ParseLogLine(std::string line)
{
	auto pc_substr = line.substr(0, 4);
	auto a_substr = line.substr(50, 2);
	auto x_substr = line.substr(55, 2);
	auto y_substr = line.substr(60, 2);
	auto p_substr = line.substr(65, 2);
	auto sp_substr = line.substr(71, 2);
	auto op_code_1_substr = line.substr(6, 2);
	auto op_code_2_substr = line.substr(9, 2);
	auto op_code_3_substr = line.substr(12, 2);

	auto pc = static_cast<uint16_t>(std::stoi(pc_substr, nullptr, 16));
	auto a = static_cast<uint8_t>(std::stoi(a_substr, nullptr, 16));
	auto x = static_cast<uint8_t>(std::stoi(x_substr, nullptr, 16));
	auto y = static_cast<uint8_t>(std::stoi(y_substr, nullptr, 16));
	auto p = static_cast<uint8_t>(std::stoi(p_substr, nullptr, 16));
	auto sp = static_cast<uint8_t>(std::stoi(sp_substr, nullptr, 16));

	auto op_codes = std::vector<uint8_t>{};
	op_codes.push_back(static_cast<uint8_t>(std::stoi(op_code_1_substr, nullptr, 16)));
	if (op_code_2_substr != "  ")
	{
		op_codes.push_back(static_cast<uint8_t>(std::stoi(op_code_2_substr, nullptr, 16)));
	}
	if (op_code_3_substr != "  ")
	{
		op_codes.push_back(static_cast<uint8_t>(std::stoi(op_code_3_substr, nullptr, 16)));
	}

	return TestLog{ pc, op_codes, a, x, y, p, sp };
}

TestLog GetLogFromCpuState(const NES& nes)
{
	auto state = nes.cpu->GetState();
	auto op_codes = nes.cpu->GetCurrentOpcodes();

	return TestLog{ state.PC, op_codes, state.A, state.X, state.Y, state.P, state.SP };
}

int main(int argc, char* argv[])
{
	auto program_filename = argv[1]; // nestest.nes
	auto bytes = nes::read_file_bytes(program_filename);
	auto rom = std::make_unique<INesRom>(bytes);
	auto nes = std::make_unique<NES>(*rom);
	auto cpu = nes->cpu;
	cpu->SetPC(0xC000);

	auto log_filename = argv[2]; // nestest.log
	auto log_lines = nes::read_file_lines(log_filename);

	int cycle_count = 0; // TODO: Verify cycle count matches
	int log_line_num = 1;
	for (const std::string& log_line : log_lines)
	{
		do
		{
			cpu->Cycle();
			cycle_count++;
		} while (cpu->GetRemainingCycles() > 0);
		auto expected = ParseLogLine(log_line);
		auto actual = GetLogFromCpuState(*nes);
		std::cout << actual << std::endl;
		if (actual != expected)
		{
			std::cout << "Mismatch on line " << log_line_num << std::endl;
			std::cout << "Expected: " << expected << std::endl;
			std::cout << "Was:      " << actual << std::endl;
			break;
		}
		log_line_num++;
	}
}
