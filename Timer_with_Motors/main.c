/*
 * Author: Dr Bharat Verma
 * Email: bharat.verma@lnmiit.ac.in
 * Date: 2025-10-09
 * Description: Timer0-based 1 ms delay example for 8052/8051 family.
 *              Change the FOSC macro to match your hardware oscillator.
 */

#include <reg52.h>

// Example: generate a 1 millisecond delay using Timer0 (16-bit mode)
// Default assumption: 12 MHz crystal oscillator (classic 8051 uses 12 clocks per machine cycle)
// Adjust FOSC if your board uses a different crystal.

#define FOSC 12000000UL /* oscillator frequency in Hz (change if needed) */

sbit LED = P1 ^ 0; // example output pin to observe the delay (toggle)

// delay1ms: delays approximately 1 millisecond using Timer0
// Formula: ticks_per_ms = (FOSC / 12) / 1000
// preload = 65536 - ticks_per_ms
void delay1ms(void)
{
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

void main(void)
{
	LED = 0;
	while (1)
	{
		LED = !LED; // toggle LED (or logic on P1.0)
		delay1ms();
	}
}