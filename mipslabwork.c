/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
volatile int *LEDctrl = (volatile int*) 0xbf886110;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int *trise = (volatile int*) 0xbf886100;
  *trise = *trise & 0xff00;
  *LEDctrl = 0x00000000;
  TRISD = 0x0fe0;
  return;
}



/* This function is called repetitively from the main program */
void labwork( void ) 
{
	int switches = getsw();
	int button = getbtns();
	
	// knapp 2 001 = 1
	if(button == 1 ) 
	{
		mytime = mytime & 0xFF0F; 
		mytime = (switches << 4) | mytime;
	}
	// knapp 3 010 = 2
	if(button == 2 ) 
	{
		mytime = mytime & 0xF0FF;
		mytime = (switches << 8) | mytime;
	}
	// knapp 4 100 = 4
	if(button == 4 ) 
	{
		mytime = mytime & 0x0FFF;
		mytime = (switches << 12) | mytime;
	}

	delay( 1000 );
	time2string( textstring, mytime );
	display_string( 3, textstring );
  	(*LEDctrl)++;
	display_update();
	tick( &mytime ); 
	//display_image(96, icon); 
}
