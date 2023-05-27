#include "Emulator.h"

void Emulator::Tick(uint64_t deltaTime, const std::shared_ptr<NES>& nes)
{
	if (mState.mIsPaused) return;

	switch (mState.mOperatingMode)
	{
	case STEP_CYCLE:
		if (mState.mIsStepped)
		{
			nes->Cycle();
			mState.mIsStepped = false;
		}
		break;
	case STEP_CPU_INSTR:
		if (mState.mIsStepped)
		{
			do
			{
				nes->Cycle();
			} while (nes->cpu->GetRemainingCycles() > 0);
			mState.mIsStepped = false;
		}
		break;
	case REALTIME:
		uint64_t cycles = (deltaTime * mState.cyclesPerSecond) / 1000;
		for (int i = 0; i < cycles; i++)
		{
			nes->Cycle();
		}
		break;
	}
}

void Emulator::Pause()
{
	mState.mIsPaused = true;
}

void Emulator::Unpause()
{
	mState.mIsPaused = false;
}

bool Emulator::IsPaused()
{
	return mState.mIsPaused;
}


void Emulator::Reset()
{
// TODO
}

void Emulator::LoadRom(const std::string& filename)
{
// TODO
}

void Emulator::Quit()
{
// TODO
}

void Emulator::Break()
{
	mState.mOperatingMode = mState.mStepType;
}

bool Emulator::CanBreak()
{
	return mState.mOperatingMode == REALTIME;
}

void Emulator::Step()
{
	mState.mIsStepped = true;
}

bool Emulator::CanStep()
{
	return mState.mOperatingMode == STEP_CYCLE || mState.mOperatingMode == STEP_CPU_INSTR;
}

void Emulator::Continue()
{
	mState.mOperatingMode = REALTIME;
}

bool Emulator::CanContinue()
{
	return mState.mOperatingMode != REALTIME;
}

void Emulator::SetStepTypeCycle()
{
	mState.mStepType = STEP_CYCLE;
	if (mState.mOperatingMode != REALTIME)
	{
		mState.mOperatingMode = STEP_CYCLE;
	}
}

void Emulator::SetStepTypeCpuInstr()
{
	mState.mStepType = STEP_CPU_INSTR;
	if (mState.mOperatingMode != REALTIME)
	{
		mState.mOperatingMode = STEP_CPU_INSTR;
	}
}

OperatingMode Emulator::GetStepType()
{
	return mState.mStepType;
}
