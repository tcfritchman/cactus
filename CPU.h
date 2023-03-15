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

	void ADC();
	void LDA();
	// TODO: Etc...

	CPU::Instruction LDA_IMMEDIATE = {[this] { Immediate(); }, [this] { LDA(); }, 2};
	CPU::Instruction ADC_ABSOLUTE = {[this] { Absolute(); }, [this] { ADC(); }, 3};
};

#endif //NES_EMULATOR__CPU_H_
