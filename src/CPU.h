#ifndef NES_EMULATOR_SRC_CPU_H_
#define NES_EMULATOR_SRC_CPU_H_

#include <string>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>
#include <unordered_map>
#include "DataBus.h"
#include "Operation.h"

class CPU
{
 public:
	struct State
	{
		uint8_t A;
		uint8_t X;
		uint8_t Y;
		uint8_t P;
		uint16_t PC;
		uint16_t SP;

		bool C() const
		{
			return P & 0b00000001;
		}
		bool Z() const
		{
			return P & 0b00000010;
		}
		bool I() const
		{
			return P & 0b00000100;
		}
		bool V() const
		{
			return P & 0b01000000;
		}
		bool N() const
		{
			return P & 0b10000000;
		}
	};

	void Cycle();
	void IRQ();
	void Reset();
	void NMI();

	void Print() const;
	State GetState();
	Operation GetOperation(uint8_t op_code);

	explicit CPU(DataBus& mBus);
	CPU() = delete;

 private:
	DataBus& mBus;

	uint8_t mRegA = 0;
	uint8_t mRegX = 0;
	uint8_t mRegY = 0;
	uint16_t mRegPC = 0;
	uint8_t mRegSP = 0xFF;
	bool mCarryFlag = false;
	bool mZeroFlag = false;
	bool mInterruptDisableFlag = false;
	bool mOverflowFlag = false;
	bool mNegativeFlag = false;

	uint8_t mCurrentOpCode = 0;
	uint8_t mAddrHi = 0;
	uint8_t mAddrLo = 0;
	uint16_t mAddr = 0;
	uint8_t mData = 0;

	// Addressing modes
	void Implicit();
	void Accumulator();
	void Immediate();
	void ZeroPage();
	void ZeroPageX();
	void ZeroPageY();
	void Relative();
	void Absolute();
	void AbsoluteX();
	void AbsoluteY();
	void Indirect();
	void IndexedIndirect();
	void IndirectIndexed();

	// OperationA operations
	// Load/Store/Transfer
	void LDA();
	void LDX();
	void LDY();
	void STA();
	void STX();
	void STY();
	void TAX();
	void TAY();
	void TXA();
	void TYA();

	// Stack
	void TSX();
	void TXS();
	void PHA();
	void PHP();
	void PLA();
	void PLP();

	// Arithmetic
	void ADC();
	void SBC();
	void CMP();
	void CPX();
	void CPY();

	// Inc/Dec
	void INC();
	void INC_A();
	void INX();
	void INY();
	void DEC();
	void DEC_A();
	void DEX();
	void DEY();

	// Shift
	void ASL();
	void ASL_A();
	void LSR();
	void LSR_A();
	void ROL();
	void ROL_A();
	void ROR();
	void ROR_A();

	// Logic
	void AND();
	void EOR();
	void ORA();
	void BIT();

	// Jump/Call
	void JMP();
	void JSR();
	void RTS();

	// Branch
	void BCC();
	void BCS();
	void BEQ();
	void BMI();
	void BNE();
	void BPL();
	void BVC();
	void BVS();

	// Flags
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void SEC();
	void SED();
	void SEI();

	// Misc
	void BRK();
	void NOP();
	void RTI();

	// Unknown OperationA
	void OOPS();

	inline void ComputeZ(uint8_t value);
	inline void ComputeN(uint8_t value);
	inline uint8_t GetFlags() const;
	inline void SetFlags(uint8_t status);

	const std::unordered_map<Operation::AddressingMode, std::function<void()>> ADDRESSING_MODE_MAP = {
		{ Operation::AddressingMode::UNKNOWN, [this] { Implicit(); } },
		{ Operation::AddressingMode::IMPLICIT, [this] { Implicit(); } },
		{ Operation::AddressingMode::ACCUMULATOR, [this] { Accumulator(); } },
		{ Operation::AddressingMode::IMMEDIATE, [this] { Immediate(); } },
		{ Operation::AddressingMode::ZERO_PAGE, [this] { ZeroPage(); } },
		{ Operation::AddressingMode::ZERO_PAGE_X, [this] { ZeroPageX(); } },
		{ Operation::AddressingMode::ZERO_PAGE_Y, [this] { ZeroPageY(); } },
		{ Operation::AddressingMode::RELATIVE, [this] { Relative(); } },
		{ Operation::AddressingMode::ABSOLUTE, [this] { Absolute(); } },
		{ Operation::AddressingMode::ABSOLUTE_X, [this] { AbsoluteX(); } },
		{ Operation::AddressingMode::ABSOLUTE_Y, [this] { AbsoluteY(); } },
		{ Operation::AddressingMode::INDIRECT, [this] { Indirect(); } },
		{ Operation::AddressingMode::INDEXED_INDIRECT, [this] { IndexedIndirect(); } },
		{ Operation::AddressingMode::INDIRECT_INDEXED, [this] { IndirectIndexed(); } }
	};

	const std::unordered_map<Operation::Instruction, std::function<void()>> INSTRUCTION_MAP = {
		{ Operation::Instruction::UNKNOWN, [this] { OOPS(); } },
		{ Operation::Instruction::LDA, [this] { LDA(); } },
		{ Operation::Instruction::LDX, [this] { LDX(); } },
		{ Operation::Instruction::LDY, [this] { LDY(); } },
		{ Operation::Instruction::STA, [this] { STA(); } },
		{ Operation::Instruction::STX, [this] { STX(); } },
		{ Operation::Instruction::STY, [this] { STY(); } },
		{ Operation::Instruction::TAX, [this] { TAX(); } },
		{ Operation::Instruction::TAY, [this] { TAY(); } },
		{ Operation::Instruction::TXA, [this] { TXA(); } },
		{ Operation::Instruction::TYA, [this] { TYA(); } },
		{ Operation::Instruction::TSX, [this] { TSX(); } },
		{ Operation::Instruction::TXS, [this] { TXS(); } },
		{ Operation::Instruction::PHA, [this] { PHA(); } },
		{ Operation::Instruction::PHP, [this] { PHP(); } },
		{ Operation::Instruction::PLA, [this] { PLA(); } },
		{ Operation::Instruction::PLP, [this] { PLP(); } },
		{ Operation::Instruction::AND, [this] { AND(); } },
		{ Operation::Instruction::EOR, [this] { EOR(); } },
		{ Operation::Instruction::ORA, [this] { ORA(); } },
		{ Operation::Instruction::BIT, [this] { BIT(); } },
		{ Operation::Instruction::ADC, [this] { ADC(); } },
		{ Operation::Instruction::SBC, [this] { SBC(); } },
		{ Operation::Instruction::CMP, [this] { CMP(); } },
		{ Operation::Instruction::CPX, [this] { CPX(); } },
		{ Operation::Instruction::CPY, [this] { CPY(); } },
		{ Operation::Instruction::INC, [this] { INC(); } },
		{ Operation::Instruction::INX, [this] { INX(); } },
		{ Operation::Instruction::INY, [this] { INY(); } },
		{ Operation::Instruction::DEC, [this] { DEC(); } },
		{ Operation::Instruction::DEX, [this] { DEX(); } },
		{ Operation::Instruction::DEY, [this] { DEY(); } },
		{ Operation::Instruction::ASL, [this] { ASL(); } },
		{ Operation::Instruction::LSR, [this] { LSR(); } },
		{ Operation::Instruction::ROL, [this] { ROL(); } },
		{ Operation::Instruction::ROR, [this] { ROR(); } },
		{ Operation::Instruction::JMP, [this] { JMP(); } },
		{ Operation::Instruction::JSR, [this] { JSR(); } },
		{ Operation::Instruction::RTS, [this] { RTS(); } },
		{ Operation::Instruction::BCC, [this] { BCC(); } },
		{ Operation::Instruction::BCS, [this] { BCS(); } },
		{ Operation::Instruction::BEQ, [this] { BEQ(); } },
		{ Operation::Instruction::BMI, [this] { BMI(); } },
		{ Operation::Instruction::BNE, [this] { BNE(); } },
		{ Operation::Instruction::BPL, [this] { BPL(); } },
		{ Operation::Instruction::BVC, [this] { BVC(); } },
		{ Operation::Instruction::BVS, [this] { BVS(); } },
		{ Operation::Instruction::CLC, [this] { CLC(); } },
		{ Operation::Instruction::CLD, [this] { CLD(); } },
		{ Operation::Instruction::CLI, [this] { CLI(); } },
		{ Operation::Instruction::CLV, [this] { CLV(); } },
		{ Operation::Instruction::SEC, [this] { SEC(); } },
		{ Operation::Instruction::SED, [this] { SED(); } },
		{ Operation::Instruction::SEI, [this] { SEI(); } },
		{ Operation::Instruction::BRK, [this] { BRK(); } },
		{ Operation::Instruction::NOP, [this] { NOP(); } },
		{ Operation::Instruction::RTI, [this] { RTI(); } }
	};

	const Operation UNKNOWN_INSTR = { Operation::AddressingMode::UNKNOWN, Operation::Instruction::UNKNOWN, 1 };

	const std::vector<Operation> OPERATIONS = {

		// 0x00
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x10
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x20
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x30
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x40
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x50
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x60
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ADC, 3},
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x70
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x80
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0x90
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xA0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::LDA, 2},
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xB0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xC0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xD0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xE0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,

		// 0xF0
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
		UNKNOWN_INSTR,
	};
};

#endif //NES_EMULATOR_SRC_CPU_H_
