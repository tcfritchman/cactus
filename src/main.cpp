#include <iostream>
#include "DataBus.h"
#include "CPU.h"

int main()
{
	std::cout << "Hello, World!" << std::endl;

	DataBus dataBus = DataBus();
	CPU cpu(dataBus);
	cpu.Print();

	for (int i = 0; i < 2; ++i)
	{
		cpu.Cycle();
		cpu.Print();
	}

	return 0;
}
