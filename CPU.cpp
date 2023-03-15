#include <iostream>
#include "CPU.h"

void CPU::Cycle()
{
	mCurrentInstruction = mBus.read(mRegPC);
	Instruction* instruction;

	switch (mCurrentInstruction)
	{
	case 0xA9:
		instruction = &LDA_IMMEDIATE;
		break;
	case 0x6D:
		instruction = &ADC_ABSOLUTE;
		break;
	default:
		std::cout << "Illegal instruction" << std::endl;
		exit(1);
	}

	instruction->addressing_mode();
	instruction->operation();
	mRegPC += instruction->cycles;
}
void CPU::IRQ()
{
	// TODO
}
void CPU::Reset()
{
	// TODO
}
void CPU::NMI()
{
	// TODO
}

void CPU::Print() const
{
	std::printf("Registers:\nA: 0x%x, X: 0x%x, Y: 0x%x, SP: 0x%x, PC: 0x%x\nFlags:\nC: 0x%x, Z: 0x%x, I: 0x%x, V: 0x%x, N: 0x%x\n",
		mRegA, mRegX, mRegY, mRegSP, mRegPC, mCarryFlag, mZeroFlag, mInterruptDisableFlag, mOverflowFlag, mNegativeFlag);
}

CPU::CPU(DataBus& mBus) : mBus(mBus)
{
}

void CPU::Implicit()
{
	// Do nothing
}

void CPU::Accumulator()
{
	// Do nothing
}

void CPU::Immediate()
{
	mData = mBus.read(mRegPC + 1);
}

void CPU::ZeroPage()
{
	mAddrHi = 0;
	mAddrLo = mRegPC + 1;
	mAddr = mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::ZeroPageX()
{
	// TODO
}

void CPU::ZeroPageY()
{
	// TODO
}

void CPU::Relative()
{
	// TODO
}

void CPU::Absolute()
{
	mAddrHi = mBus.read(mRegPC + 1);
	mAddrLo = mBus.read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::AbsoluteX()
{
	// TODO
}

void CPU::AbsoluteY()
{
	// TODO
}

void CPU::Indirect()
{
	// TODO
}

void CPU::IndexedIndirect()
{
	// TODO
}

void CPU::IndirectIndexed()
{
	// TODO
}

void CPU::ADC()
{
	mRegA += mData;
	// TODO: Flags
}

void CPU::LDA()
{
	mRegA = mData;
 	// TODO: Flags
}
