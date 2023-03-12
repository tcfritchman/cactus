#include <iostream>
#include "CPU.h"

const uint8_t LDA_IMMEDIATE = 0xA9;
const uint8_t ADC_IMMEDIATE = 0x69;
const uint8_t ADC_ABSOLUTE = 0x6D;

void CPU::Cycle()
{

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
		m_A_Reg, m_X_Reg, m_Y_Reg, m_SP_Reg, m_PC_Reg, m_Carry_Flag, m_Zero_Flag, m_InterruptDisable_Flag, m_Overflow_Flag, m_Negative_Flag);
}

CPU::CPU(const DataBus& mBus) : m_Bus(mBus)
{
}
