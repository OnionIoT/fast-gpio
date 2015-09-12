#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <fastgpio.h>
#include <fastpwm.h>



int main(int argc, char* argv[])
{
	int status	= 0;

	int 	cmd	= -1;
	
	int 	pinNum;
	int 	value;
	int 	freq, duty;

	FastGpio	gpioObj;
	FastPwm		pwmObj;

	int 	verbosity 		= 1;
	int 	debugLevel		= 0;


	// object setup
	gpioObj.SetVerbosity(verbosity);
	gpioObj.SetDebugMode(debugLevel);
	
	pwmObj.SetVerbosity(verbosity);
	pwmObj.SetDebugMode(debugLevel);

	if 	(	argc != 3 && 
			argc != 4 &&
			argc != 5
		) 
	{
		printf("Usage:\n");
		printf("\t%s read <gpio>\n", argv[0]);
		printf("\t%s set <gpio> <value>\n", argv[0]);
		printf("\t%s pwm <gpio> <freq> <duty>\n", argv[0]);

		return EXIT_SUCCESS;
	}
	
	
	// parse the command line arguments
	//	arg1 - command: read, set
	// 	arg2 - gpio pin number
	// 	arg3 - value to write in case of set
	if (verbosity > 0) printf("Parsing arguments:\n");
	if (strcmp(argv[1], "set") == 0 )	{
		cmd = 0;

		// get the write value
		value	= atoi(argv[3]);
	}
	else if (strcmp(argv[1], "read") == 0 )	{
		cmd = 1;
	}
	else if (strcmp(argv[1], "pwm") == 0 )	{
		cmd = 2;

		// get the freq and duty values
		freq	= atoi(argv[3]);
		duty	= atoi(argv[4]);
	}
	else {
		return EXIT_FAILURE;
	}

	// get the pin number
	pinNum 	= atoi(argv[2]);

	// object operations	
	switch (cmd) {
		case 0:
			if (verbosity > 0) printf("Setting GPIO%d to %d\n", pinNum, value);

			gpioObj.SetDirection(pinNum, 1); // set to output
			gpioObj.Set(pinNum, value);
			break;
		case 1:
			gpioObj.Read(pinNum, value);
			if (verbosity > 0) printf("Read GPIO%d: %d\n", pinNum, value);
			break;
		case 2:
			pwmObj.Pwm(pinNum, freq, duty);
		default:
			break;
	}



	return 0;
}