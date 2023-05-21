#include "Emulator.h"

void Emulator::Tick(uint64_t deltaTime, const std::shared_ptr<NES>& nes)
{
	if (mIsPaused) return;

	switch (mOperatingMode)
	{
	case STEP_CYCLE:
		if (mIsStepped)
		{
			nes->Cycle();
			mIsStepped = false;
		}
		break;
	case STEP_CPU_INSTR:
		if (mIsStepped)
		{
			do
			{
				nes->Cycle();
			} while (nes->cpu->GetRemainingCycles() > 0);
			mIsStepped = false;
		}
		break;
	case REALTIME:
		uint64_t cycles = (deltaTime * cyclesPerSecond) / 1000;
		for (int i = 0; i < cycles; i++)
		{
			nes->Cycle();
		}
		break;
	}
}

void Emulator::Pause()
{
	mIsPaused = true;
}

void Emulator::Unpause()
{
	mIsPaused = false;
}

bool Emulator::IsPaused()
{
	return mIsPaused;
}

void Emulator::Break()
{
	mOperatingMode = mStepType;
}

bool Emulator::CanBreak()
{
	return mOperatingMode == REALTIME;
}

void Emulator::Step()
{
	mIsStepped = true;
}

bool Emulator::CanStep()
{
	return mOperatingMode == STEP_CYCLE || mOperatingMode == STEP_CPU_INSTR;
}

void Emulator::Continue()
{
	mOperatingMode = REALTIME;
}

bool Emulator::CanContinue()
{
	return mOperatingMode != REALTIME;
}

void Emulator::SetStepTypeCycle()
{
	mStepType = STEP_CYCLE;
	if (mOperatingMode != REALTIME)
	{
		mOperatingMode = STEP_CYCLE;
	}
}

void Emulator::SetStepTypeCpuInstr()
{
	mStepType = STEP_CPU_INSTR;
	if (mOperatingMode != REALTIME)
	{
		mOperatingMode = STEP_CPU_INSTR;
	}
}

OperatingMode Emulator::GetStepType()
{
	return mStepType;
}
