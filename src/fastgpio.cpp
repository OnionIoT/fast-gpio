#include <fastgpio.h>

FastGpio::FastGpio(void)
{
	// setup the memory address space
	_SetupAddress(REGISTER_BLOCK_ADDR, REGISTER_BLOCK_SIZE);
}

FastGpio::~FastGpio(void)
{
	// nothing for now
}

// public functions
int FastGpio::SetDirection(int pinNum, int bOutput)
{
	unsigned long int regVal;

	// read the current input and output settings
	regVal = _ReadReg(REGISTER_OE_OFFSET);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	// set the OE for this pin
	_SetBit(regVal, pinNum, bOutput);
	if (verbosityLevel > 0) printf("Direction setting write: 0x%08lx\n", regVal);

	// write the new register value
	_WriteReg(REGISTER_OE_OFFSET, regVal);


	return (EXIT_SUCCESS);
}

int FastGpio::GetDirection(int pinNum, int &bOutput)
{
	unsigned long int regVal;

	// read the current input and output settings
	regVal 	= _ReadReg(REGISTER_OE_OFFSET);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	// find the OE for this pin
	bOutput = _GetBit(regVal, pinNum);


	return (EXIT_SUCCESS);
}

int FastGpio::Set(int pinNum, int value)
{
	unsigned long int 	regAddr;
	unsigned long int 	regVal;

	if (value == 0 )	{
		// write to the clear register
		regAddr 	= REGISTER_CLEAR_OFFSET;
	}
	else {
		// write to the set register
		regAddr 	= REGISTER_SET_OFFSET;
	}

	// put the desired pin value into the register 
	regVal = (0x1 << pinNum);

	// write to the register
	_WriteReg (regAddr, regVal);


	return EXIT_SUCCESS;
}

int FastGpio::Read(int pinNum, int &value)
{
	unsigned long int 	regVal;

	// read the current value of all pins
	regVal 	= _ReadReg (REGISTER_IN_OFFSET);

	// find the value of the specified pin
	value 	= _GetBit(regVal, pinNum);


	return EXIT_SUCCESS;
}


