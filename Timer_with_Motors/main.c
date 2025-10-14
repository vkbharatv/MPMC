/*
 * Author: Dr Bharat Verma
 * Email: bharat.verma@lnmiit.ac.in
 * Date: 2025-10-09
 * Description: Timer0-based 1 ms delay example for 8052/8051 family with stepper motor control.
 *              Change the FOSC macro in delay.h to match your hardware oscillator.
 */

#include <reg52.h>	 // Standard 8051/8052 register definitions; ensure your toolchain provides this file
#include "timer51.h" // Provides delay1ms(unsigned int ms) for millisecond delays using Timer0; set FOSC macro in delay.h to match your hardware

sbit LED = P1 ^ 0;

#define STEPPER_PORT P2 // Use P2 port for stepper control
#define STEP_DELAY 50	// Step delay in milliseconds to control speed
#define NUM_STEPS 4		// Number of steps in the sequence

// Stepper motor step sequence for full step (two adjacent phases on at a time)
// Each value represents the port pattern for one step
code unsigned char stepper_sequence[NUM_STEPS] = {
	0x03, // 0011 - First and second coils active (P2.0 = 1, P2.1 = 1)
	0x06, // 0110 - Second and third coils active (P2.1 = 1, P2.2 = 1)
	0x0C, // 1100 - Third and fourth coils active (P2.2 = 1, P2.3 = 1)
	0x09  // 1001 - Fourth and first coils active (P2.3 = 1, P2.0 = 1)
};

void stepper_steps(unsigned int steps);

void main(void)
{
	LED = 0;
	STEPPER_PORT = 0x00; // Initialize stepper motor to off state
	while (1)
	{
		LED = !LED; // Toggle LED to indicate operation
		stepper_steps(90);
	}
}

void stepper_steps(unsigned int steps)
{
	static unsigned char step_index = 0;
	unsigned int i;
	for (i = 0; i < steps; i++)
	{
		STEPPER_PORT = stepper_sequence[step_index];

		step_index++;
		if (step_index >= NUM_STEPS)
		{
			step_index = 0; // Wrap around to the beginning of the sequence
		}
		delay(STEP_DELAY);
	}
}