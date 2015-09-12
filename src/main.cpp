#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <fastgpio.h>
#include <fastpwm.h>

#define VERBOSE		1
#define	DEBUG 		0

typedef enum e_GpioCmd {
	GPIO_CMD_SET = 0,
	GPIO_CMD_READ,
	GPIO_CMD_PWM,
	NUM_GPIO_CMD
} gpioCmd;

struct gpioSetup {
	gpioCmd cmd; 

	int 	pinNumber;
	int 	pinValue;

	// pwm options
	int 	bPwm;
	int 	pwmFreq;
	int 	pwmDuty;
};

void initGpioSetup (gpioSetup* obj)
{
	obj->pinNumber	= -1;
	obj->pinValue	= 0;
	
	obj->bPwm		= 0;
	obj->pwmFreq	= 0;
	obj->pwmDuty	= 0;
}

int parseArguments(int argc, char* argv[], gpioSetup *setup)
{
	// check for the correct number of arguments
	if 	(	argc != 3 && 
			argc != 4 &&
			argc != 5
		) 
	{
		printf("Usage:\n");
		printf("\t%s read <gpio>\n", argv[0]);
		printf("\t%s set <gpio> <value>\n", argv[0]);
		printf("\t%s pwm <gpio> <freq> <duty>\n", argv[0]);

		return EXIT_FAILURE;
	}

	// parse the command line arguments
	//	arg1 - command: read, set
	// 	arg2 - gpio pin number
	// 	arg3 - value to write in case of set
	if (strcmp(argv[1], "set") == 0 )	{
		setup->cmd 	= GPIO_CMD_SET;

		// get the write value
		setup->pinValue	= atoi(argv[3]);
	}
	else if (strcmp(argv[1], "read") == 0 )	{
		setup->cmd 	= GPIO_CMD_READ;
	}
	else if (strcmp(argv[1], "pwm") == 0 )	{
		setup->cmd 	= GPIO_CMD_PWM;

		// get the freq and duty values
		setup->pwmFreq	= atoi(argv[3]);
		setup->pwmDuty	= atoi(argv[4]);
	}
	else {
		return EXIT_FAILURE;
	}

	// get the pin number
	setup->pinNumber 	= atoi(argv[2]);

	return EXIT_SUCCESS;
}

// function to accept gpio and pwm commands
int gpioRun(gpioSetup* setup)
{
	int status	= 0;

	FastGpio	gpioObj;
	FastPwm		pwmObj;

	int 	verbosity 		= 1;
	int 	debugLevel		= 0;

	// object setup
	gpioObj.SetVerbosity(VERBOSE);
	gpioObj.SetDebugMode(DEBUG);
	
	pwmObj.SetVerbosity(VERBOSE);
	pwmObj.SetDebugMode(DEBUG);


	// object operations	
	switch (setup->cmd) {
		case GPIO_CMD_SET:
			if (verbosity > 0) printf("Setting GPIO%d to %d\n", setup->pinNumber, setup->pinValue);

			gpioObj.SetDirection(setup->pinNumber, 1); // set to output
			gpioObj.Set(setup->pinNumber, setup->pinValue);
			break;

		case GPIO_CMD_READ:
			gpioObj.Read(setup->pinNumber, setup->pinValue);
			if (verbosity > 0) printf("Read GPIO%d: %d\n", setup->pinNumber, setup->pinValue);
			break;

		case GPIO_CMD_PWM:
			pwmObj.Pwm(setup->pinNumber, setup->pwmFreq, setup->pwmDuty);

		default:
			break;
	}

	return EXIT_SUCCESS;
}



int main(int argc, char* argv[])
{
	gpioSetup* setup 	= new gpioSetup;

	// parse the arguments
	if (parseArguments(argc, argv, setup) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	// run the command
	gpioRun(setup);

	return 0;
}