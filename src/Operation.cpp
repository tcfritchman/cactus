#include "Operation.h"

Operation::Operation(Operation::AddressingMode addressingMode, Operation::Instruction instruction, size_t cycles)
	: addressing_mode(addressingMode), instruction(instruction), cycles(cycles)
{
	switch (addressing_mode)
	{
	case AddressingMode::UNKNOWN:
	case AddressingMode::IMPLICIT:
	case AddressingMode::ACCUMULATOR:
		bytes = 1;
		break;
	case AddressingMode::IMMEDIATE:
	case AddressingMode::ZERO_PAGE:
	case AddressingMode::ZERO_PAGE_X:
	case AddressingMode::ZERO_PAGE_Y:
	case AddressingMode::RELATIVE:
	case AddressingMode::INDEXED_INDIRECT:
	case AddressingMode::INDIRECT_INDEXED:
		bytes = 2;
		break;
	case AddressingMode::ABSOLUTE:
	case AddressingMode::ABSOLUTE_X:
	case AddressingMode::ABSOLUTE_Y:
	case AddressingMode::INDIRECT:
		bytes = 3;
		break;
	}

	switch (instruction)
	{
	case Instruction::UNKNOWN:
		mnemonic = "???";
		break;
	case Instruction::LDA:
		mnemonic = "LDA";
		break;
	case Instruction::LDX:
		mnemonic = "LDX";
		break;
	case Instruction::LDY:
		mnemonic = "LDY";
		break;
	case Instruction::STA:
		mnemonic = "STA";
		break;
	case Instruction::STX:
		mnemonic = "STX";
		break;
	case Instruction::STY:
		mnemonic = "STY";
		break;
	case Instruction::TAX:
		mnemonic = "TAX";
		break;
	case Instruction::TAY:
		mnemonic = "TAY";
		break;
	case Instruction::TXA:
		mnemonic = "TXA";
		break;
	case Instruction::TYA:
		mnemonic = "TYA";
		break;
	case Instruction::TSX:
		mnemonic = "TSX";
		break;
	case Instruction::TXS:
		mnemonic = "TXS";
		break;
	case Instruction::PHA:
		mnemonic = "PHA";
		break;
	case Instruction::PHP:
		mnemonic = "PHP";
		break;
	case Instruction::PLA:
		mnemonic = "PLA";
		break;
	case Instruction::PLP:
		mnemonic = "PLP";
		break;
	case Instruction::AND:
		mnemonic = "AND";
		break;
	case Instruction::EOR:
		mnemonic = "EOR";
		break;
	case Instruction::ORA:
		mnemonic = "ORA";
		break;
	case Instruction::BIT:
		mnemonic = "BIT";
		break;
	case Instruction::ADC:
		mnemonic = "ADC";
		break;
	case Instruction::SBC:
		mnemonic = "SBC";
		break;
	case Instruction::CMP:
		mnemonic = "CMP";
		break;
	case Instruction::CPX:
		mnemonic = "CPX";
		break;
	case Instruction::CPY:
		mnemonic = "CPY";
		break;
	case Instruction::INC:
		mnemonic = "INC";
		break;
	case Instruction::INX:
		mnemonic = "INX";
		break;
	case Instruction::INY:
		mnemonic = "INY";
		break;
	case Instruction::DEC:
		mnemonic = "DEC";
		break;
	case Instruction::DEX:
		mnemonic = "DEX";
		break;
	case Instruction::DEY:
		mnemonic = "DEY";
		break;
	case Instruction::ASL:
		mnemonic = "ASL";
		break;
	case Instruction::LSR:
		mnemonic = "LSR";
		break;
	case Instruction::ROL:
		mnemonic = "ROL";
		break;
	case Instruction::ROR:
		mnemonic = "ROR";
		break;
	case Instruction::JMP:
		mnemonic = "JMP";
		break;
	case Instruction::JSR:
		mnemonic = "JSR";
		break;
	case Instruction::RTS:
		mnemonic = "RTS";
		break;
	case Instruction::BCC:
		mnemonic = "BCC";
		break;
	case Instruction::BCS:
		mnemonic = "BCS";
		break;
	case Instruction::BEQ:
		mnemonic = "BEQ";
		break;
	case Instruction::BMI:
		mnemonic = "BMI";
		break;
	case Instruction::BNE:
		mnemonic = "BNE";
		break;
	case Instruction::BPL:
		mnemonic = "BPL";
		break;
	case Instruction::BVC:
		mnemonic = "BVC";
		break;
	case Instruction::BVS:
		mnemonic = "BVS";
		break;
	case Instruction::CLC:
		mnemonic = "CLC";
		break;
	case Instruction::CLD:
		mnemonic = "CLD";
		break;
	case Instruction::CLI:
		mnemonic = "CLI";
		break;
	case Instruction::CLV:
		mnemonic = "CLV";
		break;
	case Instruction::SEC:
		mnemonic = "SEC";
		break;
	case Instruction::SED:
		mnemonic = "SED";
		break;
	case Instruction::SEI:
		mnemonic = "SEI";
		break;
	case Instruction::BRK:
		mnemonic = "BRK";
		break;
	case Instruction::NOP:
		mnemonic = "NOP";
		break;
	case Instruction::RTI:
		mnemonic = "RTI";
		break;
	}
}
