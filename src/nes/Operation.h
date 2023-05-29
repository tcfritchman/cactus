#ifndef NES_EMULATOR_SRC_OPERATION_H_
#define NES_EMULATOR_SRC_OPERATION_H_

#include <string>
#include <cstdint>
#include <vector>

class Operation
{
 public:
	enum class AddressingMode
	{
		UNKNOWN,
		IMPLICIT,
		ACCUMULATOR,
		IMMEDIATE,
		ZERO_PAGE,
		ZERO_PAGE_X,
		ZERO_PAGE_Y,
		RELATIVE,
		ABSOLUTE,
		ABSOLUTE_X,
		ABSOLUTE_Y,
		INDIRECT,
		INDEXED_INDIRECT,
		INDIRECT_INDEXED
	};

	enum class Instruction
	{
		UNKNOWN,
		LDA,
		LDX,
		LDY,
		STA,
		STX,
		STY,
		TAX,
		TAY,
		TXA,
		TYA,
		TSX,
		TXS,
		PHA,
		PHP,
		PLA,
		PLP,
		AND,
		EOR,
		ORA,
		BIT,
		ADC,
		SBC,
		CMP,
		CPX,
		CPY,
		INC,
		INX,
		INY,
		DEC,
		DEX,
		DEY,
		ASL,
		LSR,
		ROL,
		ROR,
		JMP,
		JSR,
		RTS,
		BCC,
		BCS,
		BEQ,
		BMI,
		BNE,
		BPL,
		BVC,
		BVS,
		CLC,
		CLD,
		CLI,
		CLV,
		SEC,
		SED,
		SEI,
		BRK,
		NOP,
		RTI
	};

	AddressingMode addressing_mode;
	Instruction instruction;
	size_t cycles;
	std::string mnemonic;
	size_t bytes;

	Operation(AddressingMode addressingMode, Instruction instruction, size_t cycles);
};

#endif //NES_EMULATOR_SRC_OPERATION_H_
