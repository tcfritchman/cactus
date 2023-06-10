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
		uint8_t SP;

		bool C() const;
		bool Z() const;
		bool I() const;
		bool V() const;
		bool N() const;

		bool operator==(const State& rhs) const;
		bool operator!=(const State& rhs) const;
	};

	void Cycle();
	void IRQ();
	void Reset();
	void NMI();

	State GetState();
	static Operation GetOperation(uint8_t op_code);
	size_t GetRemainingCycles() const;
	bool IsLastCycle() const;
	std::vector<uint8_t> GetCurrentOpcodes() const;
	void SetPC(uint16_t pc);

	explicit CPU(std::shared_ptr<DataBus> mBus);
	CPU() = delete;
	virtual ~CPU();

 private:
	std::shared_ptr<DataBus> mBus;

	uint8_t mRegA = 0;
	uint8_t mRegX = 0;
	uint8_t mRegY = 0;
	uint16_t mRegPC = 0;
	uint8_t mRegSP = 0xFD;
	bool mCarryFlag = false;
	bool mZeroFlag = false;
	bool mInterruptDisableFlag = true;
	bool mOverflowFlag = false;
	bool mNegativeFlag = false;

	// TODO
	bool mIsInterrupted = false;
	uint16_t mInterruptAddress = 0;

	size_t mCyclesRemaining = 0;
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

	static const Operation UNKNOWN_INSTR;
	static const std::vector<Operation> OPERATIONS;
};

#endif //NES_EMULATOR_SRC_CPU_H_
