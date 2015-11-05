#ifndef _FAST_GPIO_MAIN_H_
#define _FAST_GPIO_MAIN_H_

#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <fstream>

#include <fastgpio.h>
#include <fastpwm.h>


#define FASTGPIO_VERBOSITY_QUIET		(0)
#define FASTGPIO_VERBOSITY_NORMAL		(1)
#define FASTGPIO_VERBOSITY_ALL			(2)
#define FASTGPIO_VERBOSITY_JSON			(3)

#define FASTGPIO_DEFAULT_VERBOSITY		(FASTGPIO_VERBOSITY_NORMAL)
#define FASTGPIO_DEFAULT_DEBUG			(0)

#define FASTGPIO_VERBOSE	0
#define	FASTGPIO_DEBUG 		0

#define PID_FILE	"/tmp/pin%d_pwm_pid"

#define FASTGPIO_STDOUT_STRING			"> %s GPIO%d: %s\n"
#define FASTGPIO_JSON_STRING			"{\"cmd\":\"%s\", \"pin\":%d, \"val\":\"%s\"}\n"

#define FASTGPIO_CMD_STRING_SET			"Set"
#define FASTGPIO_CMD_STRING_READ		"Read"
#define FASTGPIO_CMD_STRING_SET_DIR		"Set direction"
#define FASTGPIO_CMD_STRING_GET_DIR		"Get direction"
#define FASTGPIO_CMD_STRING_PWM			"Set PWM" 



// type definitions
typedef enum e_GpioCmd {
	GPIO_CMD_SET = 0,
	GPIO_CMD_READ,
	GPIO_CMD_SET_DIRECTION,
	GPIO_CMD_GET_DIRECTION,
	GPIO_CMD_PWM,
	NUM_GPIO_CMD
} gpioCmd;

struct gpioSetup {
	gpioCmd cmd; 
	char*	cmdString;

	int 	pinNumber;
	int 	pinValue;
	int 	pinDir;		// 0 - input, 1 - output

	// pwm options
	int 	bPwm;
	int 	pwmFreq;
	int 	pwmDuty;

	// general options
	int 	verbose;
	int 	debug;
};


// function definitions
void 	initGpioSetup 		(gpioSetup* obj);

void 	usage				(const char* progName);
int 	parseArguments		(const char* progName, int argc, char* argv[], gpioSetup *setup);

int 	gpioRun				(gpioSetup* setup);
int 	pwmRun				(gpioSetup* setup);

int 	noteChildPid		(int pinNum, int pid);
int 	killOldProcess		(int pinNum);
int 	checkOldProcess		(gpioSetup *setup);



#endif 	// _FAST_GPIO_MAIN_H_