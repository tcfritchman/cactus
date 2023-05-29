#ifndef NES_EMULATOR_SRC_EMULATOR_H_
#define NES_EMULATOR_SRC_EMULATOR_H_

#include <memory>
#include "NES.h"
#include "EmulatorState.h"

class Emulator
{
 public:
	// TODO: Input handling for controls
	void Tick(uint64_t deltaTime);

	void Pause();
	void Unpause();
	bool IsPaused();
	void HardReset();
	void LoadRom(const std::string &filename);
	void Quit();
	bool ShouldQuit();

	void Break();
	bool CanBreak();
	void Step();
	bool CanStep();
	void Continue();
	bool CanContinue();
	void SetStepTypeCycle();
	void SetStepTypeCpuInstr();
	OperatingMode GetStepType();

	explicit Emulator(const std::string &filename);

 private:
	std::unique_ptr<INesRom> mRom;
 public:
	std::shared_ptr<NES> mNes;
 private:
	std::unique_ptr<EmulatorState> mState;
	bool mShouldQuit = false;

	std::unique_ptr<INesRom> CreateRomFromFile(const std::string &filename);
};

#endif //NES_EMULATOR_SRC_EMULATOR_H_
