#ifndef _MODULE_H_
#define _MODULE_H_

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>





class Module {
public:
	Module(void);
	~Module(void);

	void 	SetVerbosity	(int input);
	void 	SetVerbosity	(bool input);

	void	SetDebugMode	(int input);
	void	SetDebugMode	(bool input);


protected:
	// protected functions
	int 				_SetupAddress	(unsigned long int blockBaseAddr, unsigned long int blockSize);
	void 				_WriteReg 		(unsigned long int registerOffset, unsigned long int value);
	unsigned long int 	_ReadReg 		(unsigned long int registerOffset);
	
	void 				_SetBit			(unsigned long int &regVal, int bitNum, int value);
	int 				_GetBit			(unsigned long int regVal, int bitNum);

	// protected members
	int				verbosityLevel;
	int 			debugLevel;

	volatile unsigned long int *regAddress;
};

#endif 	// _MODULE_H_