#include <reg51.h>
sbit m1 = P1 ^ 0; // Motor input 1 (P1.0)
sbit m2 = P1 ^ 1; // Motor input 2 (P1.1)

// Buttons
sbit btn_duty = P1 ^ 6; // INT0: change duty cycle
sbit btn_dir = P1 ^ 7;  // INT1: toggle direction

#define PWM_PERIOD 10             // number of timer ticks per PWM cycle
#define DUTY_STEP 1               // duty step in percent (0..100)
volatile unsigned char duty = 5; // duty (0..PWM_PERIOD)
volatile unsigned char pwm_cnt = 0;
volatile unsigned char direction = 1; // 1: forward, 0: reverse

void timer0_isr(void) interrupt 1
{
    pwm_cnt++;
    if (pwm_cnt >= PWM_PERIOD)
        pwm_cnt = 0;

    if (pwm_cnt < duty)
    {
        if (direction)
        { // forward: PWM on M1, M2 low
            m1 = 1;
            m2 = 0;
        }
        else
        { // reverse: PWM on M2, M1 low
            m1 = 0;
            m2 = 1;
        }
    }
    else
    {
        // coast between pulses
        m1 = 0;
        m2 = 0;
    }
}

#define FOSC 11059200UL
#define TIMER_TICK_FREQ (FOSC / 12UL)

// set desired PWM frequency in Hz; returns actual frequency achieved
unsigned long set_pwm_frequency(unsigned long freq_hz)
{
    // For 1kHz with PWM_PERIOD=100, we need 100,000 interrupts/sec
    // Timer ticks per interrupt = 921600 / 100000 = 9.216
    unsigned long ticks_per_overflow;
    unsigned int reload_value;

    ticks_per_overflow = TIMER_TICK_FREQ / (freq_hz * (unsigned long)PWM_PERIOD);

    if (ticks_per_overflow < 1)
        ticks_per_overflow = 1;
    if (ticks_per_overflow > 256)
        ticks_per_overflow = 256;

    reload_value = 256 - (unsigned int)ticks_per_overflow;

    TR0 = 0; // stop timer while changing reload
    TH0 = (reload_value & 0xFF);
    TL0 = TH0;
    TR0 = 1; // restart timer

    return TIMER_TICK_FREQ / (ticks_per_overflow * (unsigned long)PWM_PERIOD);
}
void main(void)
{
    int x;
    // Ensure outputs idle low
    m1 = 0;
    m2 = 0;

    // Timer0 Mode 2 (8-bit auto-reload)
    TMOD &= 0xF0;
    TMOD |= 0x02;

    // Enable Timer0 interrupt
    ET0 = 1;

    // External interrupts: falling-edge
    IT0 = 1; // INT0 edge triggered
    IT1 = 1; // INT1 edge triggered
    EX0 = 1; // enable INT0 (duty step)
    EX1 = 1; // enable INT1 (direction toggle)

    EA = 1; // global interrupts

    // Initial settings
    x = set_pwm_frequency(1000UL); // ~1 kHz PWM

    // Idle loop
    while (1)
    {
        if (btn_dir == 0)
        {
            direction = !direction; // toggle direction
            while (btn_dir == 0)
                ; // simple debounce
        }
        if (btn_duty == 0)
        {
            if (duty + DUTY_STEP <= PWM_PERIOD)
                duty += DUTY_STEP;
            else
                duty = 0;
            while (btn_duty == 0)
                ; // simple debounce
        }
    }
}
