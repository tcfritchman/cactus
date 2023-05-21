#ifndef NES_EMULATOR_SRC_EMULATOR_H_
#define NES_EMULATOR_SRC_EMULATOR_H_

#include <memory>
#include "NES.h"

enum OperatingMode
{
	STEP_CYCLE,
	STEP_CPU_INSTR,
	REALTIME
};

class Emulator
{
 public:
	// TODO: Input handling for controls
	void Tick(uint64_t deltaTime, const std::shared_ptr<NES>& nes);

	void Pause();
	void Unpause();
	bool IsPaused();

	void Break();
	bool CanBreak();
	void Step();
	bool CanStep();
	void Continue();
	bool CanContinue();
	void SetStepTypeCycle();
	void SetStepTypeCpuInstr();
	OperatingMode GetStepType();

 private:
	const uint64_t cyclesPerSecond = 5369318; // PPU Clock Speed

	bool mIsPaused = false;
	OperatingMode mOperatingMode = STEP_CYCLE;
	OperatingMode mStepType = STEP_CYCLE;
	bool mIsStepped = false;
};

#endif //NES_EMULATOR_SRC_EMULATOR_H_
