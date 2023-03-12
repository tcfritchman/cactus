#ifndef NES_EMULATOR__CPU_H_
#define NES_EMULATOR__CPU_H_

#include <cstdint>
#include "DataBus.h"

class CPU
{
 public:
	void Cycle();
	void IRQ();
	void Reset();
	void NMI();

	void Print() const;

	explicit CPU(const DataBus& mBus);
	CPU() = delete;

 private:
	const DataBus& m_Bus;
	uint8_t m_A_Reg = 0;
	uint8_t m_X_Reg = 0;
	uint8_t m_Y_Reg = 0;
	uint16_t m_PC_Reg = 0;
	uint8_t m_SP_Reg = 0;
	bool m_Carry_Flag = false;
	bool m_Zero_Flag = false;
	bool m_InterruptDisable_Flag = false;
	bool m_Overflow_Flag = false;
	bool m_Negative_Flag = false;
};

#endif //NES_EMULATOR__CPU_H_
