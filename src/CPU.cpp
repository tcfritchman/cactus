#include <iostream>
#include "CPU.h"
#include "Util.h"

void CPU::Cycle()
{
	mCurrentOpCode = mBus->read(mRegPC);
	Operation operation = OPERATIONS[mCurrentOpCode];
	ADDRESSING_MODE_MAP.find(operation.addressing_mode)->second();
	INSTRUCTION_MAP.find(operation.instruction)->second();
	mRegPC += operation.bytes;
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

CPU::State CPU::GetState()
{
	uint8_t flags = CPU::GetFlags();

	return CPU::State{ mRegA, mRegX, mRegY, flags, mRegPC, mRegSP };
}

Operation CPU::GetOperation(uint8_t op_code)
{
	return OPERATIONS[op_code];
}

CPU::CPU(DataBus* mBus) : mBus(mBus)
{
	std::printf("Created CPU\n");
}

CPU::~CPU()
{
	std::printf("Destroyed CPU\n");
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
	mData = mBus->read(mRegPC + 1);
}

void CPU::ZeroPage()
{
	mAddrHi = 0;
	mAddrLo = mBus->read(mRegPC + 1);
	mAddr = mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::ZeroPageX()
{
	mAddrHi = 0;
	mAddrLo = mBus->read(mRegPC + 1) + mRegX;
	mAddr = mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::ZeroPageY()
{
	mAddrHi = 0;
	mAddrLo = mBus->read(mRegPC + 1) + mRegY;
	mAddr = mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::Relative()
{
	auto offset = static_cast<int8_t>(mBus->read(mRegPC + 1)); // convert to signed value
	mAddr = mRegPC + offset; // TODO: Does it wrap around?
	// TODO: hi and lo addr bytes?
	mData = mBus->read(mAddr); // Not used
}

void CPU::Absolute()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::AbsoluteX()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo + mRegX;
	mData = mBus->read(mAddr);
}

void CPU::AbsoluteY()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo + mRegY;
	mData = mBus->read(mAddr);
}

void CPU::Indirect()
{
	auto indirect_lo = mBus->read(mRegPC + 1);
	auto indirect_hi = mBus->read(mRegPC + 2);
	auto indirect_addr = (((uint16_t)indirect_hi) << 8) + indirect_lo;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus->read(mAddr); // Not used
}

void CPU::IndexedIndirect()
{
	auto indirect_zero_page = mBus->read(mRegPC + 1);
	uint8_t indirect_addr = indirect_zero_page + mRegX;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::IndirectIndexed()
{
	auto indirect_zero_page = mBus->read(mRegPC + 1);
	auto indirect_lo = mBus->read(indirect_zero_page + 1);
	auto indirect_hi = mBus->read(indirect_zero_page + 2);
	auto indirect_addr = (((uint16_t)indirect_hi) << 8) + indirect_lo + mRegY;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = (((uint16_t)mAddrHi) << 8) + mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::OOPS()
{
	std::cout << "Illegal instruction" << std::endl;
	exit(1);
}

void CPU::LDA()
{
	mRegA = mData;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::LDX()
{
	mRegX = mData;
	ComputeZ(mRegX);
	ComputeN(mRegX);
}

void CPU::LDY()
{
	mRegX = mData;
	ComputeZ(mRegY);
	ComputeN(mRegY);
}

inline void CPU::ComputeZ(uint8_t value)
{
	mZeroFlag = value == 0;
}

inline void CPU::ComputeN(uint8_t value)
{
	mNegativeFlag = value & 0x80;
}

inline uint8_t CPU::GetFlags() const
{
	return (mCarryFlag)
		& (mZeroFlag << 1)
		& (mInterruptDisableFlag << 2)
		& (mOverflowFlag << 6)
		& (mNegativeFlag << 7);
}

inline void CPU::SetFlags(uint8_t status)
{
	mCarryFlag = status & 0x1;
	mZeroFlag = status & 0x2;
	mInterruptDisableFlag = status & 0x4;
	mOverflowFlag = status & 0x40;
	mNegativeFlag = status & 0x80;
}

void CPU::STA()
{
	mBus->write(mRegA, mAddr);
}

void CPU::STX()
{
	mBus->write(mRegX, mAddr);
}

void CPU::STY()
{
	mBus->write(mRegY, mAddr);
}

void CPU::TAX()
{
	mRegX = mRegA;
	ComputeZ(mRegX);
	ComputeN(mRegX);
}

void CPU::TAY()
{
	mRegY = mRegA;
	ComputeZ(mRegY);
	ComputeN(mRegY);
}

void CPU::TXA()
{
	mRegA = mRegX;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::TYA()
{
	mRegA = mRegY;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::TSX()
{
	mRegX = mRegSP;
	ComputeZ(mRegX);
	ComputeN(mRegX);
}

void CPU::TXS()
{
	mRegSP = mRegX;
}

void CPU::PHA()
{
	mBus->write(mRegA, mRegSP);
	mRegSP--;
}

void CPU::PHP()
{
	uint8_t status = GetFlags();
	mBus->write(status, mRegSP);
	mRegSP--;
}

void CPU::PLA()
{
	mRegSP++;
	mRegA = mBus->read(mRegSP);
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::PLP()
{
	mRegSP++;
	uint8_t status = mBus->read(mRegSP);
	SetFlags(status);
}

void CPU::ADC()
{
	bool isCarry = (uint16_t)mRegA + mData + mCarryFlag > 0xFF;
	bool isOverflow = 0; // TODO

	mRegA += mData;
	mRegA += mCarryFlag;

	mCarryFlag = isCarry;
	mOverflowFlag = isOverflow;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::SBC()
{
	bool isCarry = (uint16_t)mRegA - mData - (1 - mCarryFlag) > 0xFF;
	bool isOverflow = 0; // TODO

	mRegA -= mData;
	mRegA -= 1 - mCarryFlag;

	mCarryFlag = isCarry;
	mOverflowFlag = isOverflow;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::CMP()
{
	uint8_t result = mRegA - mData;
	ComputeZ(result);
	ComputeN(result);
	mCarryFlag = mRegA >= mData;
}

void CPU::CPX()
{
	uint8_t result = mRegX - mData;
	ComputeZ(result);
	ComputeN(result);
	mCarryFlag = mRegX >= mData;
}

void CPU::CPY()
{
	uint8_t result = mRegY - mData;
	ComputeZ(result);
	ComputeN(result);
	mCarryFlag = mRegY >= mData;
}

void CPU::INC()
{
	uint8_t result = mData + 1;
	mBus->write(result, mAddr);
	ComputeZ(result);
	ComputeN(result);
}

void CPU::INX()
{
	mRegX++;
	ComputeZ(mRegX);
	ComputeN(mRegX);
}

void CPU::INY()
{
	mRegY++;
	ComputeZ(mRegY);
	ComputeN(mRegY);
}

void CPU::DEC()
{
	uint8_t result = mData - 1;
	mBus->write(result, mAddr);
	ComputeZ(result);
	ComputeN(result);
}

void CPU::DEX()
{
	mRegX--;
	ComputeZ(mRegX);
	ComputeN(mRegX);
}

void CPU::DEY()
{
	mRegY--;
	ComputeZ(mRegY);
	ComputeN(mRegY);
}

void CPU::ASL()
{
	bool isCarry = mData & 0x80;
	uint8_t result = mData << 1;
	mBus->write(result, mAddr);
	mCarryFlag = isCarry;
	ComputeZ(result);
	ComputeN(result);
}

void CPU::LSR()
{
	bool isCarry = mData & 0x1;
	uint8_t result = mData >> 1;
	mBus->write(result, mAddr);
	mCarryFlag = isCarry;
	ComputeZ(result);
	ComputeN(result);
}

void CPU::ROL()
{
	uint8_t hiBit = mData & 0x80;
	uint8_t result = (mData << 1) | (hiBit >> 7);
	mBus->write(result, mAddr);
	mCarryFlag = hiBit != 0;
	ComputeZ(result);
	ComputeN(result);
}

void CPU::ROR()
{
	uint8_t loBit = mData & 0x1;
	uint8_t result = (mData >> 1) | (loBit << 7);
	mBus->write(result, mAddr);
	mCarryFlag = loBit != 0;
	ComputeZ(result);
	ComputeN(result);
}

void CPU::AND()
{
	mRegA &= mData;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::EOR()
{
	mRegA ^= mData;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::ORA()
{
	mRegA |= mData;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::BIT()
{
	uint8_t result = mRegA & mData;
	ComputeZ(result);
	ComputeN(result);
	mOverflowFlag = result & 0x40; // bit 6 is set
}

void CPU::JMP()
{
	mRegPC = mAddr;
}

void CPU::JSR()
{
	uint16_t return_addr = mRegPC - 1; // Assuming PC is advanced before this function call!
	uint8_t return_addr_hi = nes::hi_byte(return_addr);
	uint8_t return_addr_lo = nes::lo_byte(return_addr);
	mBus->write(return_addr_hi, mRegSP);
	mRegSP--;
	mBus->write(return_addr_lo, mRegSP);
	mRegSP--;
	mRegPC = mAddr;
}

void CPU::RTS()
{
	mRegSP++;
	uint8_t return_addr_lo = mBus->read(mRegSP);
	mRegSP++;
	uint8_t return_addr_hi = mBus->read(mRegSP);
	uint16_t return_addr = nes::address(return_addr_lo, return_addr_hi);
	mRegPC = return_addr;
}

void CPU::BCC()
{
	if (!mCarryFlag)
	{
		// Assuming PC is advanced before this function call!
		mRegPC = mAddr;
	}
}

void CPU::BCS()
{
	if (mCarryFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BEQ()
{
	if (mZeroFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BMI()
{
	if (mNegativeFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BNE()
{
	if (!mZeroFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BPL()
{
	if (!mNegativeFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BVC()
{
	if (!mOverflowFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::BVS()
{
	if (mOverflowFlag)
	{
		mRegPC = mAddr;
	}
}

void CPU::CLC()
{
	mCarryFlag = false;
}

void CPU::CLD()
{
	// No decimal mode
}

void CPU::CLI()
{
	mInterruptDisableFlag = false;
}

void CPU::CLV()
{
	mOverflowFlag = false;
}

void CPU::SEC()
{
	mCarryFlag = true;
}

void CPU::SED()
{
	// No decimal mode
}

void CPU::SEI()
{
	mInterruptDisableFlag = true;
}

void CPU::BRK()
{
	uint16_t return_addr = mRegPC - 1; // Assuming PC is advanced before this function call!
	uint8_t return_addr_hi = nes::hi_byte(return_addr);
	uint8_t return_addr_lo = nes::lo_byte(return_addr);
	uint8_t status = GetFlags();
	nes::set_bit(&status, 4); // How to handle IRQ/NMI not setting this bit
	nes::set_bit(&status, 5);
	mBus->write(return_addr_hi, mRegSP);
	mRegSP--;
	mBus->write(return_addr_lo, mRegSP);
	mRegSP--;
	mBus->write(status, mRegSP);
	mRegSP--;
	mRegPC = mAddr;
}

void CPU::NOP()
{
	// Literally do nothing
}

void CPU::RTI()
{
	mRegSP++;
	uint8_t status = mBus->read(mRegSP);
	mRegSP++;
	uint8_t return_addr_lo = mBus->read(mRegSP);
	mRegSP++;
	uint8_t return_addr_hi = mBus->read(mRegSP);
	uint16_t return_addr = nes::address(return_addr_lo, return_addr_hi);
	mRegPC = return_addr;
	SetFlags(status);
}

bool CPU::State::C() const
{
	return nes::test_bit(P, 0);
}

bool CPU::State::Z() const
{
	return nes::test_bit(P, 1);
}

bool CPU::State::I() const
{
	return nes::test_bit(P, 2);
}

bool CPU::State::V() const
{
	return nes::test_bit(P, 6);
}

bool CPU::State::N() const
{
	return nes::test_bit(P, 7);
}
