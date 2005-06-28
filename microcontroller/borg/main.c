
#include <inttypes.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "config.h"
#include "scrolltext.h"
#include "pixel.h"
#include "programm.h"

#define ROWPORT  PORTA
#define ROWDDR   DDRA
#define COLPORT  PORTC
#define COLDDR   DDRC


unsigned char pixmap[NUMPLANE][8];

SIGNAL(SIG_OUTPUT_COMPARE0)
{
	static unsigned char plane = 0;
	static unsigned char count = 0;
	
	count ++;
	if ((COLPORT <<= 1) == 0){
		count = 0;
		COLPORT =1;

		if ( ++plane == NUMPLANE ) 
			plane = 0;

	}
	ROWPORT = pixmap[plane][count];
}

void timer0_on(){
/* 	TCCR0: FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
		CS02 CS01 CS00
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/64
		 1    0    0       clk/256
		 1    0    1       clk/1024
	
*/
	TCCR0 = 0x0B;	// CTC Mode, clk/8
	TCNT0 = 0;	// reset timer
	OCR0  = 0x10;	// Compare with this value
	TIMSK = 0x02;	// Compare match Interrupt on
}

void timer0_off(){
	TIMSK = 0;
}

void wait(int ms){
/* 	TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
		CS22 CS21 CS20
		 0    0    0	       stop
		 0    0    1       clk
		 0    1    0       clk/8
		 0    1    1       clk/32
		 1    0    0       clk/64
		 1    0    1       clk/128
		 1    1    0       clk/256
		 1    1    1       clk/1024	
*/
	TCCR2 = 0x0D;	//CTC Mode, clk/8
	OCR2 = 125;	//1000Hz
	for(;ms>0;ms--){
		while(!(TIFR&0x80));	//wait for compare matzch flag
		TIFR=0x80;		//reset flag
	}
}

void init_Ports(){
	ROWDDR = 0xFF;
	COLDDR = 0xFF;
	ROWPORT = 0;
	COLPORT = 0;
}

void labor_borg(){
	unsigned int delay = 40;

	shift_in(&pix_L, delay);
	shift_in(&pix_A, delay);
	shift_in(&pix_B, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_R, delay);

	shift_out(5,delay);

	shift_in(&pix_W, delay);
	shift_in(&pix_O, delay);
	shift_in(&pix_R, delay);
	shift_in(&pix_G, delay);
	
	shift_out(16,delay);
}



int
main (void){
	clear_screen(0);
	init_Ports();
	timer0_on();
	sei();
	for(;;){
		labor_borg();
		matrix1();
		fadein();
		joern1();
		snake();
		joern2();
		schachbrett(20);
		spirale(20);
		labor_borg();
		feuer();


//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
	}
}
