#include <module.h>

Module::Module(void)
{
	// not verbose by default
	verbosityLevel	= 0;

	// not in debug mode by default
	debugLevel		= 0;
}

Module::~Module(void)
{
	// nothing for now
}


// Debug Functions
void Module::SetVerbosity (int input)
{
	verbosityLevel	= input;
}

void Module::SetVerbosity (bool input)
{
	verbosityLevel	= (input ? 1 : 0);
}

void Module::SetDebugMode (int input)
{
	debugLevel		= input;
}

void Module::SetDebugMode (bool input)
{
	debugLevel		= (input ? 1 : 0);
}


// Register access
int Module::_SetupAddress(unsigned long int blockBaseAddr, unsigned long int blockSize)
{
	int  m_mfd;

	if (debugLevel == 0)
	{
		if ((m_mfd = open("/dev/mem", O_RDWR)) < 0)
		{
			return EXIT_FAILURE;	// maybe return -1
		}

		regAddress = (unsigned long*)mmap	(	NULL, 
												blockSize, 
												PROT_READ|PROT_WRITE, 
												MAP_SHARED, 
												m_mfd, 
												blockBaseAddr
											);
		close(m_mfd);

		if (regAddress == MAP_FAILED)
		{
			return EXIT_FAILURE;	// maybe return -2
		}
	}

	return EXIT_SUCCESS;	// regAddress is now populated
}

void Module::_WriteReg(unsigned long int registerOffset, unsigned long int value)
{
	if (verbosityLevel > 0)	printf("Writing register 0x%08lx with data 0x%08lx \n", (regAddress + registerOffset), value);

	*(regAddress + registerOffset) = value;
}

unsigned long int Module::_ReadReg(unsigned long int registerOffset)
{
	unsigned long int 	value = 0x0;

	// read the value 
	value = *(regAddress + registerOffset);

	if (verbosityLevel > 0)	printf("Read register 0x%08lx, data: 0x%08lx \n", (regAddress + registerOffset), value);

	return(value);
}

// change the value of a single bit
void Module::_SetBit(unsigned long int &regVal, int bitNum, int value)
{
	if (value == 1) {
		regVal |= (1 << bitNum);
	}
	else {
		regVal &= ~(1 << bitNum);
	}

	// try this out
	// regVal ^= (-value ^ regVal) & (1 << bitNum);
}

// find the value of a single bit
int Module::_GetBit(unsigned long int regVal, int bitNum)
{
	int value;

	// isolate the specific bit
	value = ((regVal >> bitNum) & 0x1);

	return (value);
}
