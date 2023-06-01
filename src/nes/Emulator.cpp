#include "Emulator.h"
#include "Util.h"

void Emulator::Tick(uint64_t deltaTime)
{
	if (mState->mIsPaused) return;

	switch (mState->mOperatingMode)
	{
	case STEP_CYCLE:
		if (mState->mIsStepped)
		{
			mNes->Cycle();
			mState->mIsStepped = false;
		}
		break;
	case STEP_CPU_INSTR:
		if (mState->mIsStepped)
		{
			do
			{
				mNes->Cycle();
			} while (!mNes->cpu->IsLastCycle());
			mState->mIsStepped = false;
			// Dumb hack
			mNes->Cycle();
			mNes->Cycle();
		}
		break;
	case REALTIME:
		uint64_t cycles = (deltaTime * mState->cyclesPerSecond) / 1000;
		for (int i = 0; i < cycles; i++)
		{
			mNes->Cycle();
		}
		break;
	}
}

void Emulator::Pause()
{
	mState->mIsPaused = true;
}

void Emulator::Unpause()
{
	mState->mIsPaused = false;
}

bool Emulator::IsPaused()
{
	return mState->mIsPaused;
}

void Emulator::HardReset()
{
	*mNes = NES{ *mRom };
	*mState = EmulatorState{};
}

void Emulator::LoadRom(const std::string& filename)
{
	mRom = CreateRomFromFile(filename);
}

void Emulator::Quit()
{
	mShouldQuit = true;
}

bool Emulator::ShouldQuit()
{
	return mShouldQuit;
}

void Emulator::Break()
{
	mState->mOperatingMode = mState->mStepType;
}

bool Emulator::CanBreak()
{
	return mState->mOperatingMode == REALTIME;
}

void Emulator::Step()
{
	mState->mIsStepped = true;
}

bool Emulator::CanStep()
{
	return mState->mOperatingMode == STEP_CYCLE || mState->mOperatingMode == STEP_CPU_INSTR;
}

void Emulator::Continue()
{
	mState->mOperatingMode = REALTIME;
}

bool Emulator::CanContinue()
{
	return mState->mOperatingMode != REALTIME;
}

void Emulator::SetStepTypeCycle()
{
	mState->mStepType = STEP_CYCLE;
	if (mState->mOperatingMode != REALTIME)
	{
		mState->mOperatingMode = STEP_CYCLE;
	}
}

void Emulator::SetStepTypeCpuInstr()
{
	mState->mStepType = STEP_CPU_INSTR;
	if (mState->mOperatingMode != REALTIME)
	{
		mState->mOperatingMode = STEP_CPU_INSTR;
	}
}

OperatingMode Emulator::GetStepType()
{
	return mState->mStepType;
}

Emulator::Emulator(const std::string& filename) :
	mRom{ CreateRomFromFile(filename) },
	mNes{ std::make_shared<NES>(*mRom) },
	mState{ std::make_unique<EmulatorState>() }
{
	mNes->cpu->SetPC(0xC000);
}

std::unique_ptr<INesRom> Emulator::CreateRomFromFile(const std::string& filename)
{
	auto rom_bytes = nes::read_file_bytes(filename);
	return std::make_unique<INesRom>(rom_bytes);
}
