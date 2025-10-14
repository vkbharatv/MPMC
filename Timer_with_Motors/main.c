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

/* Assignments
 * 1. Write a program to blink an LED at 5Hz connected at PIN1.0 when a stepper mottor completes 180 degree rotations.
 * 2. Write a program to rotate stepper motor 90 degree when a switch is pressed one time.
 * 3. Write a program to rotate a stepper motor 180 degree when a switch connected to PIN1.0 and -180 degree when another switch connected to PIN1.2 is pressed.
 * 4. Write a program to rotate stepper motor at clockwise when one switch is pressed and anti-clockwise when another switch is pressed. Also toggle an LED when motor shaft completes the 45 degree angle.
 */