#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw( void )
{
	//0000 0000 1010 0000
	volatile int switches =  PORTD >> 8;
	switches &= 0xf;
	
	return switches;
}

int getbtns(void){
	volatile int buttons = PORTD >> 5;
	buttons &= 0x7;
	return buttons;
}