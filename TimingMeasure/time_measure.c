/*
 * TimingMeasure/time_measure.c
 *
 * Measure time intervals using Timer0 and external interrupts on an 8051 microcontroller.
 * The timer counts milliseconds, and the external interrupt toggles the timer's operation.
 * The elapsed time in seconds is calculated and displayed on port P2.
 */
#include <reg51.h>

unsigned long time_ms = 0;
unsigned char t = 0;

void timer0_isr(void) interrupt 1
{
    time_ms++;
    TH0 = 0xFC;
    TL0 = 0x18;
}

void init_timer()
{
    TMOD = 0x01; // Timer0 for ~1 ms overflow at 12 MHz (mode 1, 16-bit)
    TF0 = 0;
    TR0 = 0;
    ET0 = 1; // Enable Timer0 interrupt (vector provided by timer0_isr)
    TH0 = 0xFC;
    TL0 = 0x18;
    time_ms = 0; // Reset time counter
}

void ext0_isr(void) interrupt 0
{
    // Toggle Timer0 run/stop on each external interrupt edge
    TR0 = !TR0;

    if (!TR0)
    {
        t = (time_ms + ((TH0 > 0xFC) ? 1 : 0)) / 1000; // Convert to seconds
        init_timer();
    }
}

void main()
{
    float f;
    init_timer();
    EX0 = 1; // Enable external interrupt 0
    IT0 = 1; // INT0 edge-triggered (falling edge) for clean toggling
    EA = 1;  // Enable global interrupts

    while (1)
    {
        P2 = (unsigned char)t;
        f = (t != 0) ? (1 / (float)t) : 0;
    }
}