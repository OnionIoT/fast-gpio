#include <fastpwm.h>

FastPwm::FastPwm(void)
{
	Reset();
}

FastPwm::FastPwm(int freq, int duty)
{
	Reset();
	
	// setup the pwm info
	_SetupPeriods(freq, duty);
}

FastPwm::~FastPwm(void)
{
	// nothing for now
}

void FastPwm::Reset(void)
{
	bSetup 	= 0;
}


void FastPwm::_SetupPeriods(int frequency, int duty)
{
	double 	dutyCycleInv;

	// convert the datatypes
	freq 		= (double) frequency;
	dutyCycle 	= (double)duty / 100.0f;

	// find the period (in ms)
	period 		= (1.0f/freq) * 1000;

	// find the low and high periods based on the duty-cycle
	periodHigh	= period * dutyCycle;
	periodLow	= period - periodHigh; //can also be: period * (1.0f - dutyCycle);

	// note that setup has occured
	bSetup = 1;

	if (verbosityLevel > 0) {
		printf ( 	"PWM Setup:: frequency = %d, duty-cycle = %d%%\nperiod = %.2f, period hi = %.2f, period lo = %.2f\n", 
					frequency,
					duty,
					period,
					periodHigh,
					periodLow
				);
	}
}

void FastPwm::Pwm (int pinNum)
{
	if (bSetup == 0) {
		_SetupPeriods(DEFAULT_FREQ, DEFAULT_DUTY_CYCLE);
	}

	// run the pwm
	_Pwm(pinNum);
}

void FastPwm::Pwm (int pinNum, int freq, int duty)
{
	_SetupPeriods(freq, duty);

	// run the pwm
	_Pwm(pinNum);
}

void FastPwm::_Pwm (int pinNum)
{
	// set the pin to output
	gpio.SetDirection(pinNum, 1);

	// start the loop
	while (1) {
		//// HIGH part of cycle
		gpio.Set(pinNum, 1);
		_Sleep(periodHigh);

		// LOW part of cycle
		if (dutyCycle <= 0.95f) {
			gpio.Set(pinNum, 0);
			_Sleep(periodLow);
		}
	}
}

void FastPwm::_Sleep (double length)
{
	// sleep function uses microseconds
	int value	= (int)(length * 1000);	

	usleep(value);
}
