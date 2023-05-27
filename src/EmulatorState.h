#ifndef NES_EMULATOR_SRC_EMULATORSTATE_H_
#define NES_EMULATOR_SRC_EMULATORSTATE_H_

#include <cstdint>

enum OperatingMode
{
	STEP_CYCLE,
	STEP_CPU_INSTR,
	REALTIME
};

struct EmulatorState
{
	const uint64_t cyclesPerSecond = 5369318; // PPU Clock Speed
	bool mIsPaused = false;
	OperatingMode mOperatingMode = STEP_CPU_INSTR;
	OperatingMode mStepType = STEP_CPU_INSTR;
	bool mIsStepped = false;
};

#endif //NES_EMULATOR_SRC_EMULATORSTATE_H_
