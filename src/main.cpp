#include <main.h>


void initGpioSetup (gpioSetup* obj)
{
	obj->pinNumber	= -1;
	obj->pinValue	= 0;
	obj->pinDir		= 0;
	
	obj->bPwm		= 0;
	obj->pwmFreq	= 0;
	obj->pwmDuty	= 0;
}

void printUsage(char* progName) {
	printf("Usage:\n");
	printf("\t%s set-input <gpio>\n", progName);
	printf("\t%s set-output <gpio>\n", progName);
	printf("\t%s get-direction <gpio>\n", progName);
	printf("\t%s read <gpio>\n", progName);
	printf("\t%s set <gpio> <value: 0 or 1>\n", progName);
	printf("\t%s pwm <gpio> <freq in Hz> <duty cycle percentage>\n", progName);
	printf("\n");
}

int parseArguments(int argc, char* argv[], gpioSetup *setup)
{
	// check for the correct number of arguments
	if 	(	argc != 3 && 
			argc != 4 &&
			argc != 5
		) 
	{
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}

	// reset the gpio setup
	initGpioSetup(setup);

	// parse the command line arguments
	//	arg1 - command: read, set
	// 	arg2 - gpio pin number
	// 	arg3 - value to write in case of set
	if (strcmp(argv[1], "set-input") == 0 )	{
		setup->cmd 	= GPIO_CMD_SET_DIRECTION;
		setup->pinDir 	= 0;
	}
	else if (strcmp(argv[1], "set-output") == 0 )	{
		setup->cmd 	= GPIO_CMD_SET_DIRECTION;
		setup->pinDir 	= 1;
	}
	else if (strcmp(argv[1], "get-direction") == 0 )	{
		setup->cmd 	= GPIO_CMD_GET_DIRECTION;
	}
	else if (strcmp(argv[1], "set") == 0 )	{
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

// function to run gpio commands
int gpioRun(gpioSetup* setup)
{
	int status	= EXIT_SUCCESS;
	FastGpio	gpioObj;

	// object setup
	gpioObj.SetVerbosity(FASTGPIO_VERBOSE);
	gpioObj.SetDebugMode(FASTGPIO_DEBUG);

	// to do: add check for pwm
	// check for any pwm processes already running on this pin
	killOldProcess(setup->pinNumber);


	// object operations	
	switch (setup->cmd) {
		case GPIO_CMD_SET:
			gpioObj.SetDirection(setup->pinNumber, 1); // set to output
			gpioObj.Set(setup->pinNumber, setup->pinValue);

			printf("Setting GPIO%d to %d\n", setup->pinNumber, setup->pinValue);
			break;

		case GPIO_CMD_READ:
			gpioObj.Read(setup->pinNumber, setup->pinValue);
			printf("Read GPIO%d: %d\n", setup->pinNumber, setup->pinValue);
			break;

		case GPIO_CMD_SET_DIRECTION:
			gpioObj.SetDirection(setup->pinNumber, setup->pinDir); // set pin direction
			printf("Setting GPIO%d to %s direction\n", setup->pinNumber, (setup->pinDir == 1 ? "OUTPUT" : "INPUT") );
			break;

		case GPIO_CMD_GET_DIRECTION:
			gpioObj.GetDirection(setup->pinNumber, setup->pinDir); // find pin direction
			printf("GPIO%d direction is %s\n", setup->pinNumber, (setup->pinDir == 1 ? "OUTPUT" : "INPUT") );
			break;

		default:
			status = EXIT_FAILURE;
			break;
	}

	return status;
}

// function to run pwm commands
int pwmRun(gpioSetup* setup)
{
	FastPwm		pwmObj;

	// check for correct command
	if (setup->cmd != GPIO_CMD_PWM) {
		return EXIT_FAILURE;
	}

	// object setup
	pwmObj.SetVerbosity(FASTGPIO_VERBOSE);
	pwmObj.SetDebugMode(FASTGPIO_DEBUG);


	// object operations	
	pwmObj.Pwm(setup->pinNumber, setup->pwmFreq, setup->pwmDuty);

	return EXIT_SUCCESS;
}

// function to note the PID of the child process
int noteChildPid(int pinNum, int pid)
{
	char 	pathname[255];
	std::ofstream myfile;
	
	// determine thef file name and open the file
	snprintf(pathname, sizeof(pathname), PID_FILE, pinNum);
	myfile.open (pathname);

	// write the pid to the file
	myfile << pid;
	myfile << "\n";

	myfile.close();


	return EXIT_SUCCESS;
} 

// function to read any existing pid notes and kill the child processes
int killOldProcess(int pinNum)
{
	char 	pathname[255];
	char	line[255];
	char	cmd[255];

	int 	pid;
	std::ifstream myfile;

	// determine thef file name and open the file
	snprintf(pathname, sizeof(pathname), PID_FILE, pinNum);
	myfile.open (pathname);

	// read the file
	if ( myfile.is_open() ) {
		// file exists, check for pid
		myfile.getline(line, 255);
		pid = atoi(line);

		// kill the process
		if (pid > 0)
		{
			sprintf(cmd, "kill %d >& /dev/null", pid);
			system(cmd);
			if (FASTGPIO_VERBOSE > 0) printf("Exiting current pwm process (pid: %d)\n", pid);
		}
	}


	return EXIT_SUCCESS;
}

// function to kill any old processes, based on which command is being run
int checkOldProcess(gpioSetup *setup)
{
	switch (setup->cmd) {
		case GPIO_CMD_SET:
		case GPIO_CMD_SET_DIRECTION:
		case GPIO_CMD_PWM:
			// kill the old process
			killOldProcess(setup->pinNumber);
			break;

		default:
			// do nothing
			break;
	}

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
	int status;
	gpioSetup* setup 	= new gpioSetup;

	// parse the arguments
	if (parseArguments(argc, argv, setup) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	// check for any pwm processes already running on this pin
	status = checkOldProcess(setup);

	// run the command
	if (setup->cmd != GPIO_CMD_PWM) {
		// single gpio command
		status = gpioRun(setup);
	}
	else {
		//// continuous gpio commands, need another process

		// create the new process
		pid_t pid = fork();

		if (pid == 0) {
			// child process, run the pwm
			status = pwmRun(setup);
		}
		else {
			// parent process
			if (FASTGPIO_VERBOSE > 0) printf("Launched child pwm process, pid: %d \n", pid);
			noteChildPid(setup->pinNumber, pid);
		}
	}

	return 0;
}