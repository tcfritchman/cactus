#include "PPU.h"
#include "Util.h"
#include <iostream>

uint8_t PPU::read(uint16_t address)
{
	return 0;
}

void PPU::write(uint8_t data, uint16_t address)
{

}

PPU::PPU(std::shared_ptr<VideoDataBus> dataBus) : mDataBus(std::move(dataBus))
{
	std::printf("Created PPU\n");
}

PPU::~PPU()
{
	std::printf("Destroyed PPU\n");
}

void PPU::Cycle()
{
	// TODO
}

uint8_t PPU::ReadPPUSTATUS()
{
	// TODO
	return 0;
}

uint8_t PPU::ReadOAMDATA()
{
	// TODO
	return 0;
}

uint8_t PPU::ReadPPUDATA()
{
	// TODO
	return 0;
}

void PPU::WritePPUCTRL(uint8_t value)
{
	mBaseNametableAddress = 0x2000 + (0x400 * (value & 0x3));
	mVRAMAddressIncrement = nes::test_bit(value, 2) ? 1 : 32;
	mSpritePatternTableAddress = nes::test_bit(value, 3) ? 0x0000 : 0x1000;
	mBackgroundPatternTableAddress = nes::test_bit(value, 4) ? 0x0000 : 0x1000;
	mSpriteHeight = nes::test_bit(value, 5) ? 8 : 16;
	mGenerateNMIOnVBlank = nes::test_bit(value, 7);
}

void PPU::WritePPUMASK(uint8_t value)
{
	mDisplayGrayscale = nes::test_bit(value, 0);
	mShowBackgroundLeftmostColumn = nes::test_bit(value, 1);
	mShowSpritesLeftmostColumn = nes::test_bit(value, 2);
	mShowBackground = nes::test_bit(value, 3);
	mShowSprites = nes::test_bit(value, 4);
	mEmphasizeRed = nes::test_bit(value, 5);
	mEmphasizeGreen = nes::test_bit(value, 6);
	mEmphasizeBlue = nes::test_bit(value, 7);
}

void PPU::WriteOAMADDR(uint8_t value)
{
	mOAMAddress = value;
}

void PPU::WriteOAMDATA(uint8_t value)
{
	// TODO
}

void PPU::WritePPUSCROLL(uint8_t value)
{
	if (mPPUScrollWriteCount == 0)
	{
		mHorizontalScrollPosition = value;
		mPPUScrollWriteCount = 1;
	}
	else
	{
		mVerticalScrollPosition = value < 240 ? value : value - 256;
		mPPUScrollWriteCount = 0;
	}
}

void PPU::WritePPUADDR(uint8_t value)
{
	if (mPPUAddrWriteCount == 0)
	{
		mPPUAddress = (mPPUAddress & 0x00FF) | (static_cast<uint16_t>(value) << 8);
		mPPUAddrWriteCount = 1;
	}
	else
	{
		mPPUAddress = (mPPUAddress & 0xFF00) | static_cast<uint16_t>(value);
		mPPUAddrWriteCount = 0;
	}
}

void PPU::WritePPUDATA(uint8_t value)
{
	// TODO
}

void PPU::WritePPUDMA(uint8_t value)
{
	// TODO
}
