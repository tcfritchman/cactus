#include "PPU.h"
#include "Util.h"
#include <iostream>

uint8_t PPU::read(uint16_t address)
{
	switch (address)
	{
	case PPUSTATUS:
		return ReadPPUSTATUS();
	case OAMDATA:
		return ReadOAMDATA();
	case PPUDATA:
		return ReadPPUDATA();
	default:
		nes::log("Invalid read PPU: 0x%x", address);
		return 0;
	}
}

void PPU::write(uint8_t data, uint16_t address)
{
	switch (address)
	{
	case PPUCTRL:
		WritePPUCTRL(data);
		return;
	case PPUMASK:
		WritePPUMASK(data);
		return;
	case OAMADDR:
		WriteOAMADDR(data);
		return;
	case OAMDATA:
		WriteOAMDATA(data);
		return;
	case PPUSCROLL:
		WritePPUSCROLL(data);
		return;
	case PPUADDR:
		WritePPUADDR(data);
		return;
	case PPUDATA:
		WritePPUDATA(data);
		return;
	case OAMDMA:
		WriteOAMDMA(data);
		return;
	default:
		nes::log("Invalid write PPU: 0x%x", address);
		return;
	}
}

PPU::PPU(std::shared_ptr<VideoDataBus> videoDataBus) : mVideoDataBus(std::move(videoDataBus))
{
	std::printf("Created PPU\n");
}

PPU::~PPU()
{
	std::printf("Destroyed PPU\n");
}

void PPU::Cycle()
{
	// Reset NMI flag
	HasNMIOccurred = false;

	// Set vertical blank started flag after post-render line
	if (mCurrentScanline == 241 && mCurrentLineCycle == 1)
	{
		mVerticalBlankStarted = true;

		if (mGenerateNMIOnVBlank)
		{
			HasNMIOccurred = true;

			// Render something
			for (int i = 0; i < RENDER_PIXEL_COUNT; i++)
			{
				mRenderPixels[i] = rand() & 0x3F;
			}
		}
	}

	// Clear vertical blank started flag on pre-render line
	if (mCurrentScanline == 261 && mCurrentLineCycle == 0)
	{
		mVerticalBlankStarted = false;
	}

	mCurrentLineCycle++;
	if (mCurrentLineCycle > 340)
	{
		mCurrentLineCycle = 0;
		mCurrentScanline++;
		if (mCurrentScanline > 261)
		{
			mCurrentScanline = 0;
		}
	}
}

uint8_t PPU::ReadPPUSTATUS()
{
	uint8_t val = (mVerticalBlankStarted << 7)
		| (mSpriteZeroHit << 6)
		| (mSpriteOverflow << 5);
	mVerticalBlankStarted = false; // Cleared on read
	return val;
}

uint8_t PPU::ReadOAMDATA()
{
	return mOAM[mOAMAddress];
}

uint8_t PPU::ReadPPUDATA()
{
	// TODO: I think this returns the last fetched data
	uint8_t val = mVideoDataBus->v_read(mPPUAddress);
	mPPUAddress++;
	return val;
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
	mOAMAddress = static_cast<uint16_t>(value);
}

void PPU::WriteOAMDATA(uint8_t value)
{
	mOAM[mOAMAddress] = value;
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
	mVideoDataBus->v_write(mPPUAddress, value);
	mPPUAddress++;
}

void PPU::WriteOAMDMA(uint8_t value)
{
	mOAMDMAAddress = static_cast<uint16_t>(value) << 8;
	// TODO: Initiate DMA
}
