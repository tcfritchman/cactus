#include "PPU.h"
#include "Util.h"
#include <iostream>

uint8_t PPU::read(uint16_t address)
{
	uint16_t wrapped_address = START_ADDRESS + (address % 8);
	switch (wrapped_address)
	{
	case PPUSTATUS:
	{
		uint8_t val = FetchPPUSTATUS();
		mVerticalBlankStarted = false; // Cleared on read
		return val;
	}
	case OAMDATA:
	{
		return FetchOAMDATA();
	}
	case PPUDATA:
	{
		uint8_t val = FetchPPUDATA();
		mPPUAddress++;
		return val;
	}
	default:
		return 0;
	}
}

void PPU::write(uint8_t data, uint16_t address)
{
	uint16_t wrapped_address = START_ADDRESS + (address % 8);
	switch (wrapped_address)
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
		return;
	}
}

uint8_t PPU::peek(uint16_t address)
{
	uint16_t wrapped_address = START_ADDRESS + (address % 8);
	switch (wrapped_address)
	{
	case PPUSTATUS:
		return FetchPPUSTATUS();
	case OAMDATA:
		return FetchOAMDATA();
	case PPUDATA:
		return FetchPPUDATA();
	default:
		return 0;
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

uint8_t PPU::FetchPPUSTATUS()
{
	return (mVerticalBlankStarted << 7)
		| (mSpriteZeroHit << 6)
		| (mSpriteOverflow << 5);
}

uint8_t PPU::FetchOAMDATA()
{
	return mOAM[mOAMAddress];
}

uint8_t PPU::FetchPPUDATA()
{
	// TODO: I think this returns the last fetched data
	return mVideoDataBus->v_read(mPPUAddress);
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
