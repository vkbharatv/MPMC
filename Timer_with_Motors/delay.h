#include <reg52.h>

/*
 * Author: Dr Bharat Verma
 * Email: bharat.verma@lnmiit.ac.in
 * Date: 2025-10-09
 * Description: Timer0-based 1 ms delay example for 8052/8051 family with stepper motor control.
 *              Change the FOSC macro to match your hardware oscillator.
 *	Example: generate a 1 millisecond delay using Timer0 (16-bit mode)
 *	Default assumption: 12 MHz crystal oscillator (classic 8051 uses 12 clocks per machine cycle)
 */

#define FOSC 12000000UL /* oscillator frequency in Hz (change if needed) */

void delay1ms(void)
{
	// delay1ms: delays approximately 1 millisecond using Timer0
	// Formula: ticks_per_ms = (FOSC / 12) / 1000
	// preload = 65536 - ticks_per_ms
	unsigned long ticks_per_ms = (FOSC / 12UL) / 1000UL;
	unsigned int reload = (unsigned int)(65536UL - ticks_per_ms);

	// Configure Timer0 mode1 (16-bit) while preserving timer1 bits
	TMOD = (TMOD & 0xF0) | 0x01;

	TH0 = (unsigned char)(reload >> 8);
	TL0 = (unsigned char)(reload & 0xFF);

	TF0 = 0; // clear overflow flag
	TR0 = 1; // start Timer0
	while (!TF0)
		;	 // wait until timer overflows
	TR0 = 0; // stop Timer0
	TF0 = 0; // clear flag for next use
}

void delay(unsigned int ms)
{
	unsigned int i;
	for (i = 0; i < ms; i++)
	{
		delay1ms();
	}
}

void delay1us(void)
{
	// delay1us: delays approximately 1 microsecond using Timer0
	// Formula: ticks_per_us = (FOSC / 12) / 1000000 = FOSC/12/1,000,000
	// preload = 256 - ticks_per_us (8-bit timer, mode 2)
	unsigned char ticks_per_us = (unsigned char)((FOSC / 12UL) / 1000000UL);
	unsigned char reload = (unsigned char)(256 - ticks_per_us);

	// Configure Timer0 mode2 (8-bit auto-reload)
	TMOD = (TMOD & 0xF0) | 0x02;
	TH0 = reload;
	TL0 = reload;

	TF0 = 0; // clear overflow flag
	TR0 = 1; // start Timer0
	while (!TF0)
		;	 // wait until timer overflows
	TR0 = 0; // stop Timer0
	TF0 = 0; // clear flag for next use
}

void delay_us(unsigned int us)
{
	unsigned int i;
	for (i = 0; i < us; i++)
	{
		delay1us();
	}
}
