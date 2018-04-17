/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
int prime = 1234567;
volatile unsigned int* porte = (unsigned int*) 0xbf886110;
int mytime = 0x5957;
int first_tick = 1;
int timeoutcount = 1;
volatile int score= 0;
volatile int isTravelingLeft = 1;
volatile int maxScore = 0;
volatile int servRight = 0;
volatile int* count = (int*)0xbf886110;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
	
	return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	
	
	volatile int * trise = (int*)0xbf886100;	//gör äns den här något, kan nämligen ta bort den och programet kör på samma sätt?
	//*trise = *trise &0xfff1;
	*trise &= 0xffffff00; /* Port E is used for the LED Set bits 0 through 7 to 0 (output) */
	TRISD |= ~0xFE0;
	//T2CON = 0x60;  //initialization of timer (I think) = 0x0800;
	
	//testcode
	T2CON = 0x0; 		// Stop timer2
    T2CONSET = 0x0070; 	// Prescale 1:256 (timer will increment by 1 each 256 clock cycles.)
    TMR2 = 0; 			// Timer2 value starts at 0

	// 80000000/256/10 We calculate PR2 by taking the amount of clocks/sec
	// divide it with the prescale value and the amount of times needed to
	// reach one second.
    PR2 = 31250; 		// Timer2 value ticks to a suitable number
    T2CONSET = 0x8000;
	//testcode
	
	return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	
	
	volatile int switches = getsw();
	volatile int switches2 = getsw();
	volatile int slask;
	volatile int slask2;
	switch(getbtns()){
	  
		case 1:
			mytime = (mytime & ~0xF0) | (switches <<4);
			break;
		case 2:
			mytime = (mytime & ~0xF00) | (switches <<8);
			break;
		case 3:
			mytime = (mytime & ~0xF0) | (switches <<4);
			switches = getsw();
			mytime = (mytime & ~0xF00) | (switches <<8);
			break;
		case 4:
			mytime = (mytime & ~0xF000) | (switches <<12);
			break;
		case 5:
			slask = mytime;
			slask &=~0xF0;
			switches=switches <<4;
			slask = slask | switches;
		
			slask &=~0xF000;
			switches=switches <<8;
			slask = slask | switches;
		
			mytime = slask;
			break;
		case 6:
			mytime = (mytime & ~0xF00) | (switches <<8);
			switches = getsw();
			mytime = (mytime & ~0xF000) | (switches <<12);
			break;
		case 7:
			slask = mytime;
			slask &=~0xF0;
			switches=switches <<4;
			slask = slask | switches;
		
			slask &=~0xFF00;
			switches=switches <<8;
			slask = slask | switches;
			switches=switches >>4;
			slask = slask | switches;
		
			mytime = slask;
			break;
	
	}		
	//delay( 1000 );
	unsigned int overflow = IFS(0);
    overflow &= 0x000100;
    overflow >>= 8;
	if (overflow == 1) {
		if (timeoutcount == 10) { // Helps getting the 'delay' correct
			timeoutcount = 1;
			if (overflow == 1) {
				if (timeoutcount == 10) {
					if (first_tick == 1){
						first_tick = 0;
					} 
				}
				timeoutcount++;
			}

  //test for projekt
 
			if( isTravelingLeft == 1){
				*count = *count <<1;
				if(*count == 0)
				{
					*count += 0x1;
				}
			}

			if(isTravelingLeft ==0 ){
				*count = *count >>1;
			}	
            
		}
		else {
			timeoutcount++;
		}
        IFSCLR(0) = 0x0100; // Set flag to 0 again.


		if(getbtns()==1 && *count <= 0x01){
			isTravelingLeft=1;
			maxScore = 1;
			if(maxScore == 1){
				score++;
				maxScore = 0;
			}
		}
		display_string( 0, "  " );
		display_update();
	}		
 
	if(getsw()== 1){

		if(/*getbtns()==4 &&*/   *count == 0x80){
			isTravelingLeft=0;
			maxScore = 1;
		}		
		if(maxScore == 1 && *count < 0x01){
			display_string( 0, "Game over" );
			display_update();
			score=0;
		}
	}
	if(getsw()==0){
		if(getbtns()==4 && *count == 0x80){
			isTravelingLeft=0;
			maxScore = 1;
		}
	
		if(*count >= 0x80 && servRight == 1){
			*count = 0x80;
			if(maxScore = 1)
				isTravelingLeft=0;
		}	
		if(*count < 0x7A)
			servRight = 1;
		
		if(*count > 0x01)
			servRight = 0;
	}	
	

	if(*count == 0x80){
		isTravelingLeft=0;
		maxScore = 1;
	}		
	if(maxScore == 1 && *count < 0x01){
		display_string( 0, "Game over" );
		display_update();
		score=0;
	}
	
  
	time2string( textstring, score );
	display_string( 3, textstring );
	display_update();
  
}
