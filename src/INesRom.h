#ifndef NES_EMULATOR_SRC_INESROM_H_
#define NES_EMULATOR_SRC_INESROM_H_

#include <vector>
#include <cstdint>

class INesRom
{

 public:
	int GetProgramRomSize() const;
	int GetCharacterRomSize() const;
	int GetMapperNumber() const;
	bool HasTrainer() const;
	std::vector<uint8_t> GetProgramRomData() const;
	std::vector<uint8_t> GetCharacterRomData() const;

	explicit INesRom(const std::vector<uint8_t>& bytes);
	virtual ~INesRom();

 private:
	const std::vector<uint8_t> bytes;
	int prg_rom_start;
	int chr_rom_start;

	static const int HEADER_SIZE = 16;
	static const int TRAINER_SIZE = 512;
	static const int PRG_ROM_BLOCK_SIZE = 16384;
	static const int CHR_ROM_BLOCK_SIZE = 8192;
	static const int PRG_ROM_SIZE_BYTE = 4;
	static const int CHR_ROM_SIZE_BYTE = 5;
	static const int FLAGS_6_BYTE = 6;
	static const int FLAGS_7_BYTE = 7;
	static const int TRAINER_BIT = 2;
};

#endif //NES_EMULATOR_SRC_INESROM_H_
