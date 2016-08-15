#ifndef _FAST_GPIO_H_
#define _FAST_GPIO_H_

#include <module.h>
#include <string.h> 

//Define Macros in derived class. 
// #define REGISTER_BLOCK_ADDR			0x18040000
// #define REGISTER_BLOCK_SIZE			0x30

// #define REGISTER_OE_OFFSET			0
// #define REGISTER_IN_OFFSET			1
// #define REGISTER_OUT_OFFSET			2
// #define REGISTER_SET_OFFSET			3
// #define REGISTER_CLEAR_OFFSET		4

//Define DEVICE_TYPE Here for now. 

#define DEVICE_TYPE			"omega2"

class FastGpio : public Module {
public:
	virtual int 	SetDirection	(int pinNum, int bOutput)=0;
	virtual int 	GetDirection 	(int pinNum, int &bOutput)=0;

	virtual int 	Set 			(int pinNum, int value)=0;
	virtual int 	Read 			(int pinNum, int &value)=0;

private:
	// private functions
	int 	pinNumber;
};


#endif 	// _FAST_GPIO_H_