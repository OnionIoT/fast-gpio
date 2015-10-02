#ifndef _FAST_GPIO_MAIN_H_
#define _FAST_GPIO_MAIN_H_

#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <fstream>

#include <fastgpio.h>
#include <fastpwm.h>


#define FASTGPIO_VERBOSE	0
#define	FASTGPIO_DEBUG 		0

#define PID_FILE	"/tmp/pin%d_pwm_pid"



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

	int 	pinNumber;
	int 	pinValue;
	int 	pinDir;		// 0 - input, 1 - output

	// pwm options
	int 	bPwm;
	int 	pwmFreq;
	int 	pwmDuty;
};


// function definitions
void 	initGpioSetup 		(gpioSetup* obj);

void 	printUsage			(char* progName);
int 	parseArguments		(int argc, char* argv[], gpioSetup *setup);

int 	gpioRun				(gpioSetup* setup);
int 	pwmRun				(gpioSetup* setup);

int 	noteChildPid		(int pinNum, int pid);
int 	killOldProcess		(int pinNum);



#endif 	// _FAST_GPIO_MAIN_H_