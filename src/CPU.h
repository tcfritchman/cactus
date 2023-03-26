#ifndef NES_EMULATOR__CPU_H_
#define NES_EMULATOR__CPU_H_

#include <string>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>
#include "DataBus.h"

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

	struct Instruction
	{
	 public:
		std::function<void()> addressing_mode;
		std::function<void()> operation;
		size_t cycles;
		std::string mnemonic;
		size_t bytes;
	};

	void Cycle();
	void IRQ();
	void Reset();
	void NMI();

	void Print() const;
	State GetState();
	Instruction GetInstruction(uint8_t op_code);

	explicit CPU(DataBus& mBus);
	CPU() = delete;

 private:
	DataBus& mBus;

	uint8_t mRegA = 0;
	uint8_t mRegX = 0;
	uint8_t mRegY = 0;
	uint16_t mRegPC = 0;
	uint8_t mRegSP = 0;
	bool mCarryFlag = false;
	bool mZeroFlag = false;
	bool mInterruptDisableFlag = false;
	bool mOverflowFlag = false;
	bool mNegativeFlag = false;

	uint8_t mCurrentInstruction = 0;
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

	// Instruction operations
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
	void LSR();
	void ROL();
	void ROR();

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

	// Unknown Instruction
	void OOPS();

	CPU::Instruction UNKNOWN_INSTR = {[this] { Implicit(); }, [this] { OOPS(); }, 1, "OOPS", 1};

	std::vector<CPU::Instruction> INSTRUCTIONS = {

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
		{[this] { Absolute(); }, [this] { ADC(); }, 3, "ADC", 3},
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
		{[this] { Immediate(); }, [this] { LDA(); }, 2, "LDA", 2},
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

#endif //NES_EMULATOR__CPU_H_
