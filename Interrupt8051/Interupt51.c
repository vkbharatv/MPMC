#include <reg51.h>
sbit led = P1 ^ 0;

#define PWM_PERIOD 10             // number of timer ticks per PWM cycle
#define DUTY_STEP 1               // step size when changing duty via INT0/INT1
volatile unsigned char duty = 5;  // duty (0..PWM_PERIOD)
volatile unsigned char pwm_cnt = 0;

void timer0_isr(void) interrupt 1
{
    pwm_cnt++;
    if (pwm_cnt >= PWM_PERIOD)
        pwm_cnt = 0;
    if (pwm_cnt < duty)
        led = 1;
    else
        led = 0;
}
// External interrupt 0 (INT0, P3.2) -> increase duty
void ext0_isr(void) interrupt 0
{
    if (duty + DUTY_STEP <= PWM_PERIOD)
        duty += DUTY_STEP;
    else
        duty = PWM_PERIOD;
}

// External interrupt 1 (INT1, P3.3) -> decrease duty
void ext1_isr(void) interrupt 2
{
    if (duty >= DUTY_STEP)
        duty -= DUTY_STEP;
    else
        duty = 0;
}
#define FOSC 11059200UL
#define TIMER_TICK_FREQ (FOSC / 12UL)

// set desired PWM frequency in Hz; returns actual frequency achieved
unsigned long set_pwm_frequency(unsigned long freq_hz)
{
    unsigned long ticks_per_overflow = TIMER_TICK_FREQ / ((unsigned long)PWM_PERIOD * freq_hz);
    if (ticks_per_overflow < 1)
        ticks_per_overflow = 1;
    if (ticks_per_overflow > 256)
        ticks_per_overflow = 256;

    TR0 = 0; // stop timer while changing reload
    TH0 = (unsigned char)(256 - ticks_per_overflow);
    TL0 = TH0;
    TR0 = 1; // restart timer

    return TIMER_TICK_FREQ / (ticks_per_overflow * PWM_PERIOD);
}

void main(void)
{

    /* Configure Timer0 in Mode 2 (8-bit auto-reload) */
    TMOD &= 0xF0; // clear timer0 bits
    TMOD |= 0x02; // timer0 mode 2 (8-bit auto-reload)

    // Default: enable timer0 interrupt and global interrupts
    // enable timer0 interrupt
    ET0 = 1;

    // Configure external interrupts: edge triggered (falling edge) and enabled
    IT0 = 1; // INT0 edge triggered
    IT1 = 1; // INT1 edge triggered
    EX0 = 1; // enable INT0
    EX1 = 1; // enable INT1

    EA = 1; // enable global interrupts

    // Set a reasonable PWM frequency for LED dimming (e.g. 200 Hz)
    // Adjust the value as needed for your application/hardware
    set_pwm_frequency(1000UL);

    // Idle loop: PWM output is handled by timer ISR; use external switches on INT0/INT1 to change duty
    while (1)
    {
        ;
    }
}
