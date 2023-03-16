#ifndef NES_EMULATOR__CPU_H_
#define NES_EMULATOR__CPU_H_

#include <cstdint>
#include <functional>
#include "DataBus.h"

class CPU
{
 public:
	void Cycle();
	void IRQ();
	void Reset();
	void NMI();

	void Print() const;

	explicit CPU(DataBus& mBus);
	CPU() = delete;

 private:
	struct Instruction
	{
	 public:
		std::function<void()> addressing_mode;
		std::function<void()> operation;
		uint16_t cycles;
	};

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

	uint8_t mCurrentInstruction;
	uint8_t mAddrHi;
	uint8_t mAddrLo;
	uint16_t mAddr;
	uint8_t mData;

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

	CPU::Instruction LDA_IMMEDIATE = {[this] { Immediate(); }, [this] { LDA(); }, 2};
	CPU::Instruction ADC_ABSOLUTE = {[this] { Absolute(); }, [this] { ADC(); }, 3};
};

#endif //NES_EMULATOR__CPU_H_
