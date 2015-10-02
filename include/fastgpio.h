#ifndef _FAST_GPIO_H_
#define _FAST_GPIO_H_

#include <module.h>


#define REGISTER_BLOCK_ADDR			0x18040000
#define REGISTER_BLOCK_SIZE			0x30

#define REGISTER_OE_OFFSET			0
#define REGISTER_IN_OFFSET			1
#define REGISTER_OUT_OFFSET			2
#define REGISTER_SET_OFFSET			3
#define REGISTER_CLEAR_OFFSET		4


class FastGpio : public Module {
public:
	FastGpio(void);
	~FastGpio(void);

	int 	SetDirection	(int pinNum, int bOutput);
	int 	GetDirection 	(int pinNum, int &bOutput);

	int 	Set 			(int pinNum, int value);
	int 	Read 			(int pinNum, int &value);

private:
	// private functions
	
	int 	pinNumber;
};


#endif 	// _FAST_GPIO_H_