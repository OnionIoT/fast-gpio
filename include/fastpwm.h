#ifndef _FAST_PWM_H_
#define _FAST_PWM_H_

#include <module.h>
#include <fastgpio.h>

#include <unistd.h>


#define DEFAULT_FREQ				200
#define DEFAULT_DUTY_CYCLE			50


class FastPwm : public Module {
public:
	FastPwm(void);
	FastPwm(int freq, int duty);
	~FastPwm(void);

	void 	Reset 			(void);

	void 	Pwm				(int pinNum);
	void 	Pwm				(int pinNum, int freq, int duty);


private:
	// private functions
	void 	_SetupPeriods	(int frequency, int duty);
	void 	_Sleep 			(double length);

	void 	_Pwm 			(int pinNum);

	// private members
	FastGpio 	gpio;

	int 		bSetup;
	double		freq;
	double 		dutyCycle;

	double 		period;
	double 		periodLow;
	double 		periodHigh;
};


#endif 	// _FAST_PWM_H_