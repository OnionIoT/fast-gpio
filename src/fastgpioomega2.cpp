#include <fastgpioomega2.h>

FastGpioOmega2::FastGpioOmega2(void)
{
	// setup the memory address space
	_SetupAddress(REG_BLOCK_ADDR, REG_BLOCK_SIZE);
}

FastGpioOmega2::~FastGpioOmega2(void)
{
	// nothing for now
}

void FastGpioOmega2::setGpioOffset(int gpio){
	int mod;
	mod = gpio / 32;
	if(mod == 0){
		this->ctrlOffset = REGISTER_CTRL0_OFFSET;
		this->dataOffset = REGISTER_DATA0_OFFSET;
		this->dataSetOffset = REGISTER_DSET0_OFFSET;
		this->dataClrOffset = REGISTER_DCLR0_OFFSET;
	}
	else if(mod == 1){
		this->ctrlOffset = REGISTER_CTRL1_OFFSET;
		this->dataOffset = REGISTER_DATA1_OFFSET;
		this->dataSetOffset = REGISTER_DSET1_OFFSET;
		this->dataClrOffset = REGISTER_DCLR1_OFFSET;
	} else{
		this->ctrlOffset = REGISTER_CTRL2_OFFSET;
		this->dataOffset = REGISTER_DATA2_OFFSET;
		this->dataSetOffset = REGISTER_DSET2_OFFSET;
		this->dataClrOffset = REGISTER_DCLR2_OFFSET;
	}
}
// public functions
int FastGpioOmega2::SetDirection(int pinNum, int bOutput)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal = _ReadReg(ctrlOffset);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	// set the OE for this pin
	_SetBit(regVal, gpio, bOutput);
	if (verbosityLevel > 0) printf("Direction setting write: 0x%08lx\n", regVal);

	// write the new register value
	_WriteReg(ctrlOffset, regVal);


	return (EXIT_SUCCESS);
}

int FastGpioOmega2::GetDirection(int pinNum, int &bOutput)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal 	= _ReadReg(ctrlOffset);
	if (verbosityLevel > 0) printf("Direction setting read: 0x%08lx\n", regVal);

	bOutput = _GetBit(regVal, gpio);

	return (EXIT_SUCCESS);
}

int FastGpioOmega2::Set(int pinNum, int value)
{
	unsigned long int 	regAddr;
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;

	if (value == 0 )	{
		// write to the clear register
		regAddr 	= dataClrOffset;
	}
	else {
		// write to the set register
		regAddr 	= dataSetOffset;
	}

	// put the desired pin value into the register 
	regVal = (0x1 << gpio);

	// write to the register
	_WriteReg (regAddr, regVal);

	return EXIT_SUCCESS;
}

int FastGpioOmega2::Read(int pinNum, int &value)
{
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current value of all pins
	regVal 	= _ReadReg (dataOffset);

	// find the value of the specified pin
	value 	= _GetBit(regVal, gpio);


	return EXIT_SUCCESS;
}
