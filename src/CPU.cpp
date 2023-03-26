#include <iostream>
#include "CPU.h"

void CPU::Cycle()
{
	mCurrentInstruction = mBus.read(mRegPC);
	Instruction* instruction = &INSTRUCTIONS[mCurrentInstruction];
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

CPU::State CPU::GetState()
{
	uint8_t flags = (mCarryFlag)
		& (mZeroFlag << 1)
		& (mInterruptDisableFlag << 2)
		& (mOverflowFlag << 6)
		& (mNegativeFlag << 7);

	return CPU::State {mRegA, mRegX, mRegY, flags, mRegPC, mRegSP};
}

CPU::Instruction CPU::GetInstruction(uint8_t op_code)
{
	return INSTRUCTIONS[op_code];
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
	mAddrLo = mBus.read(mRegPC + 1);
	mAddr = mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::ZeroPageX()
{
	mAddrHi = 0;
	mAddrLo = mBus.read(mRegPC + 1) + mRegX;
	mAddr = mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::ZeroPageY()
{
	mAddrHi = 0;
	mAddrLo = mBus.read(mRegPC + 1) + mRegY;
	mAddr = mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::Relative()
{
	auto offset = static_cast<int8_t>(mBus.read(mRegPC + 1)); // convert to signed value
	mAddr = mRegPC + offset; // TODO: Does it wrap around?
	// TODO: hi and lo addr bytes?
	mData = mBus.read(mAddr); // Not used
}

void CPU::Absolute()
{
	mAddrLo = mBus.read(mRegPC + 1);
	mAddrHi = mBus.read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::AbsoluteX()
{
	mAddrLo = mBus.read(mRegPC + 1);
	mAddrHi = mBus.read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo + mRegX;
	mData = mBus.read(mAddr);
}

void CPU::AbsoluteY()
{
	mAddrLo = mBus.read(mRegPC + 1);
	mAddrHi = mBus.read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo + mRegY;
	mData = mBus.read(mAddr);
}

void CPU::Indirect()
{
	auto indirect_lo = mBus.read(mRegPC + 1);
	auto indirect_hi = mBus.read(mRegPC + 2);
	auto indirect_addr = (((uint16_t)indirect_hi) << 8) + indirect_lo;
	mAddrLo = mBus.read(indirect_addr);
	mAddrHi = mBus.read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus.read(mAddr); // Not used
}

void CPU::IndexedIndirect()
{
	auto indirect_zero_page = mBus.read(mRegPC + 1);
	uint8_t indirect_addr = indirect_zero_page + mRegX;
	mAddrLo = mBus.read(indirect_addr);
	mAddrHi = mBus.read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus.read(mAddr);
}

void CPU::IndirectIndexed()
{
	auto indirect_zero_page = mBus.read(mRegPC + 1);
	auto indirect_lo = mBus.read(indirect_zero_page + 1);
	auto indirect_hi = mBus.read(indirect_zero_page + 2);
	auto indirect_addr = (((uint16_t)indirect_hi) << 8) + indirect_lo + mRegY;
	mAddrLo = mBus.read(indirect_addr);
	mAddrHi = mBus.read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus.read(mAddr);
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

void CPU::OOPS()
{
	std::cout << "Illegal instruction" << std::endl;
	exit(1);
}
