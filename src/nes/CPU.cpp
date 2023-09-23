#include <iostream>
#include "CPU.h"
#include "Util.h"

void CPU::Cycle()
{
	if (mCyclesRemaining == 0)
	{
		// Start executing the next instruction
		// Not cycle-accurate
		auto current_instr = mBus->read(mRegPC);
		Operation operation = OPERATIONS[current_instr];
		mCyclesRemaining = operation.cycles;
		ADDRESSING_MODE_MAP.find(operation.addressing_mode)->second();
		mRegPC += operation.bytes;
		INSTRUCTION_MAP.find(operation.instruction)->second();
	}
	else
	{
		mCyclesRemaining--;
		// TODO: Add additional cycles in conditional branch instructions
	}
}

void CPU::IRQ()
{
	// TODO
}

void CPU::Reset()
{
	mInterruptDisableFlag = true; // TODO: Is this correct?
	// TODO: Probably reset some registers as well?
	uint16_t jump_addr = nes::address(mBus->read(0xFFFC), mBus->read(0xFFFD));
	SetPC(jump_addr);
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

size_t CPU::GetRemainingCycles() const
{
	return mCyclesRemaining;
}

bool CPU::IsLastCycle() const
{
	return mCyclesRemaining == 0;
}

std::vector<uint8_t> CPU::GetCurrentOpcodes() const
{
	auto current_instr = mBus->read(mRegPC);
	Operation operation = OPERATIONS[current_instr];
	std::vector<uint8_t> current_opcodes;
	for (size_t byte_offset = 0; byte_offset < operation.bytes; byte_offset++)
	{
		current_opcodes.push_back(mBus->read(mRegPC + byte_offset));
	}
	return current_opcodes;
}

void CPU::SetPC(uint16_t pc)
{
	mRegPC = pc;
	auto current_instr = mBus->read(mRegPC);
	Operation operation = OPERATIONS[current_instr];
	mCyclesRemaining = operation.cycles;
}

CPU::CPU(std::shared_ptr<DataBus> mBus) : mBus(std::move(mBus))
{
	CPU::Reset();
	nes::log("Created CPU");
}

CPU::~CPU()
{
	nes::log("Destroyed CPU");
}

void CPU::Implicit()
{
	// Do nothing
}

void CPU::Accumulator()
{
	// Do nothing
	nes::log("Accumulator addressing mode not yet implemented!");
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
	mAddr = mRegPC + offset + 2; // Add offset plus current instruction TODO: Does it wrap around?
	// TODO: hi and lo addr bytes?
	mData = mBus->read(mAddr); // Not used
}

void CPU::Absolute()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::AbsoluteX()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo + mRegX;
	mData = mBus->read(mAddr);
}

void CPU::AbsoluteY()
{
	mAddrLo = mBus->read(mRegPC + 1);
	mAddrHi = mBus->read(mRegPC + 2);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo + mRegY;
	mData = mBus->read(mAddr);
}

void CPU::Indirect()
{
	auto indirect_lo = mBus->read(mRegPC + 1);
	auto indirect_hi = mBus->read(mRegPC + 2);
	auto indirect_addr = ((static_cast<uint16_t>(indirect_hi)) << 8) + indirect_lo;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo;
	mData = mBus->read(mAddr); // Not used
}

void CPU::IndexedIndirect()
{
	auto indirect_zero_page = mBus->read(mRegPC + 1);
	uint8_t indirect_addr = indirect_zero_page + mRegX;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo;
	mData = mBus->read(mAddr);
}

void CPU::IndirectIndexed()
{
	auto indirect_zero_page = mBus->read(mRegPC + 1);
	auto indirect_lo = mBus->read(indirect_zero_page + 1);
	auto indirect_hi = mBus->read(indirect_zero_page + 2);
	auto indirect_addr = ((static_cast<uint16_t>(indirect_hi)) << 8) + indirect_lo + mRegY;
	mAddrLo = mBus->read(indirect_addr);
	mAddrHi = mBus->read(indirect_addr + 1);
	mAddr = ((static_cast<uint16_t>(mAddrHi)) << 8) + mAddrLo;
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
		| (mZeroFlag << 1)
		| (mInterruptDisableFlag << 2)
		| (mDecimalFlag << 3)
		| (mOverflowFlag << 6)
		| (mNegativeFlag << 7)
		| 0b00100000; // Always set bit 5 to 1
}

inline void CPU::SetFlags(uint8_t status)
{
	mCarryFlag = status & 0x1;
	mZeroFlag = status & 0x2;
	mInterruptDisableFlag = status & 0x4;
	mDecimalFlag = status & 0x8;
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
	status |= 0b00010000; // Set status bit 4
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
	uint8_t result = mRegA + mData + mCarryFlag;

	bool isCarry = result < mRegA;
	bool isOverflow = ((mRegA ^ result) & 0x80) && ((mData ^ result) & 0x80);

	mRegA = result;

	mCarryFlag = isCarry;
	mOverflowFlag = isOverflow;
	ComputeZ(mRegA);
	ComputeN(mRegA);
}

void CPU::SBC()
{
	uint8_t result = mRegA - mData - (!mCarryFlag);

	bool isCarry = result > mRegA;
	bool isOverflow = ((mRegA ^ result) & 0x80) && ((mData ^ result) & 0x80);

	mRegA = result;

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
	ComputeN(mData);
	mOverflowFlag = mData & 0x40; // bit 6 is set
}

void CPU::JMP()
{
	mRegPC = mAddr;
}

void CPU::JSR()
{
	uint16_t return_addr = mRegPC;
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
	mDecimalFlag = false;
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
	mDecimalFlag = true;
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
	nes::set_bit(std::make_unique<uint8_t>(status), 4); // How to handle IRQ/NMI not setting this bit
	mBus->write(return_addr_hi, mRegSP);
	mRegSP--;
	mBus->write(return_addr_lo, mRegSP);
	mRegSP--;
	mBus->write(status, mRegSP);
	mRegSP--;
	uint16_t jump_addr = nes::address(mBus->read(0xFFFE), mBus->read(0xFFFF));
	mRegPC = jump_addr;
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

const Operation CPU::UNKNOWN_INSTR = { Operation::AddressingMode::UNKNOWN, Operation::Instruction::UNKNOWN, 1 };

const std::vector<Operation> CPU::OPERATIONS = {

	// 0x00
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::BRK, 7},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::ORA, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::ORA, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::ASL, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::PHP, 3},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::ORA, 2},
	{Operation::AddressingMode::ACCUMULATOR, Operation::Instruction::ASL, 2},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ORA, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ASL, 6},
	UNKNOWN_INSTR,

	// 0x10
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BPL, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::ORA, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::ORA, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::ASL, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::CLC, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::ORA, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::ORA, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::ASL, 7},
	UNKNOWN_INSTR,

	// 0x20
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::JSR, 6},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::AND, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::BIT, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::AND, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::ROL, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::PLP, 4},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::AND, 2},
	{Operation::AddressingMode::ACCUMULATOR, Operation::Instruction::ROL, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::BIT, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::AND, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ROL, 6},
	UNKNOWN_INSTR,

	// 0x30
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BMI, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::AND, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::AND, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::ROL, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::SEC, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::AND, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::AND, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::ROL, 7},
	UNKNOWN_INSTR,

	// 0x40
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::RTI, 6},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::EOR, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::EOR, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::LSR, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::PHA, 3},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::EOR, 2},
	{Operation::AddressingMode::ACCUMULATOR, Operation::Instruction::LSR, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::JMP, 3},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::EOR, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::LSR, 6},
	UNKNOWN_INSTR,

	// 0x50
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BVC, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::EOR, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::EOR, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::LSR, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::CLI, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::EOR, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::EOR, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::LSR, 7},
	UNKNOWN_INSTR,

	// 0x60
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::RTS, 6},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::ADC, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::ADC, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::ROR, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::PLA, 4},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::ADC, 2},
	{Operation::AddressingMode::ACCUMULATOR, Operation::Instruction::ROR, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::JMP, 5},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ADC, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::ROR, 6},
	UNKNOWN_INSTR,

	// 0x70
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BVS, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::ADC, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::ADC, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::ROR, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::SEI, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::ADC, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::ADC, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::ROR, 7},
	UNKNOWN_INSTR,

	// 0x80
	UNKNOWN_INSTR,
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::STA, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::STY, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::STA, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::STX, 3},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::DEY, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TXA, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::STY, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::STA, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::STX, 4},
	UNKNOWN_INSTR,

	// 0x90
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BCC, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::STA, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::STY, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::STA, 4},
	{Operation::AddressingMode::ZERO_PAGE_Y, Operation::Instruction::STX, 4},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TYA, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::STA, 5},
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TXS, 2},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::STA, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,

	// 0xA0
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::LDY, 2},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::LDA, 6},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::LDX, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::LDY, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::LDA, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::LDX, 3},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TAY, 2},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::LDA, 2},
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TAX, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::LDY, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::LDA, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::LDX, 4},
	UNKNOWN_INSTR,

	// 0xB0
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BCS, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::LDA, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::LDY, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::LDA, 4},
	{Operation::AddressingMode::ZERO_PAGE_Y, Operation::Instruction::LDX, 4},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::CLV, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::LDA, 4},
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::TSX, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::LDY, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::LDA, 4},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::LDX, 4},
	UNKNOWN_INSTR,

	// 0xC0
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::CPY, 2},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::CMP, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::CPY, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::CMP, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::DEC, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::INY, 2},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::CMP, 2},
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::DEX, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::CPY, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::CMP, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::DEC, 6},
	UNKNOWN_INSTR,

	// 0xD0
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BNE, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::CMP, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::CMP, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::DEC, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::CLD, 2},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::CMP, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::CMP, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::DEC, 7},
	UNKNOWN_INSTR,

	// 0xE0
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::CPX, 2},
	{Operation::AddressingMode::INDEXED_INDIRECT, Operation::Instruction::SBC, 6},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::CPX, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::SBC, 3},
	{Operation::AddressingMode::ZERO_PAGE, Operation::Instruction::INC, 5},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::INX, 2},
	{Operation::AddressingMode::IMMEDIATE, Operation::Instruction::SBC, 2},
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::NOP, 2},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::CPX, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::SBC, 4},
	{Operation::AddressingMode::ABSOLUTE, Operation::Instruction::INC, 6},
	UNKNOWN_INSTR,

	// 0xF0
	{Operation::AddressingMode::RELATIVE, Operation::Instruction::BEQ, 2},
	{Operation::AddressingMode::INDIRECT_INDEXED, Operation::Instruction::SBC, 5},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::SBC, 4},
	{Operation::AddressingMode::ZERO_PAGE_X, Operation::Instruction::INC, 6},
	UNKNOWN_INSTR,
	{Operation::AddressingMode::IMPLICIT, Operation::Instruction::SED, 2},
	{Operation::AddressingMode::ABSOLUTE_Y, Operation::Instruction::SBC, 4},
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	UNKNOWN_INSTR,
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::SBC, 4},
	{Operation::AddressingMode::ABSOLUTE_X, Operation::Instruction::INC, 7},
	UNKNOWN_INSTR,
};

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

bool CPU::State::operator==(const CPU::State& rhs) const
{
	return A == rhs.A &&
		X == rhs.X &&
		Y == rhs.Y &&
		P == rhs.P &&
		PC == rhs.PC &&
		SP == rhs.SP;
}

bool CPU::State::operator!=(const CPU::State& rhs) const
{
	return !(rhs == *this);
}
