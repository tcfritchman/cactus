#include "NES.h"
NES::NES()
{
	ram = new RAM();
	dataBus = new DataBus(ram);
	cpu = new CPU(dataBus);
	std::printf("Created NES\n");
}

NES::~NES()
{
	delete (ram);
	delete (dataBus);
	delete (cpu);
	std::printf("Destroyed NES\n");
}
