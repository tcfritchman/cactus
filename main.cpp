#include <iostream>
#include "DataBus.h"
#include "CPU.h"

int main()
{
	std::cout << "Hello, World!" << std::endl;

	const DataBus& dataBus = DataBus();
	CPU cpu(dataBus);
	cpu.Print();
	return 0;
}
