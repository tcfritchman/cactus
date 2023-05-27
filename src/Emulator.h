#ifndef NES_EMULATOR_SRC_EMULATOR_H_
#define NES_EMULATOR_SRC_EMULATOR_H_

#include <memory>
#include "NES.h"
#include "EmulatorState.h"

class Emulator
{
 public:
	// TODO: Input handling for controls
	void Tick(uint64_t deltaTime, const std::shared_ptr<NES>& nes);

	void Pause();
	void Unpause();
	bool IsPaused();
	void Reset();
	void LoadRom(const std::string &filename);
	void Quit();

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
	EmulatorState mState;
};

#endif //NES_EMULATOR_SRC_EMULATOR_H_
