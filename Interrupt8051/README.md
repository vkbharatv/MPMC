## Use of 8051 software and hardware interrupts
This repository presents an exxample of use of Timer and external interrupts. In 8051 there are the following available interrupts,
### 8051 interrupts — hardware and software (overview)

- Interrupt sources (classic 8051):
    - External 0 (INT0)
    - Timer 0 overflow
    - External 1 (INT1)
    - Timer 1 overflow
    - Serial port (UART)
    - (8052 and variants: Timer 2 and extras)

- Vector addresses (standard 8051):
    - External 0: 0x0003
    - Timer 0: 0x000B
    - External 1: 0x0013
    - Timer 1: 0x001B
    - Serial: 0x0023

- Enabling / disabling:
    - Globally enable interrupts by setting EA.
    - Enable each source through its bit in the IE SFR (EX0, ET0, EX1, ET1, ES).
    - Use the IP SFR to give an interrupt high or low priority.

- Hardware vs software activation:
    - Hardware interrupts are raised by external signals (INT0/INT1) or peripheral events (timer overflow, serial).
    - There is no dedicated single-instruction "software interrupt" in classic 8051, but software can:
        - Directly call the ISR (LCALL/LJMP) to run the handler.
        - Generate an interrupt request by setting the corresponding interrupt flag (e.g., TF0, TF1, IE0, IE1) in the SFRs — if that interrupt is enabled, the CPU will vector to the ISR.
        - For serial, set TI/RI or manipulate SCON flags to simulate conditions.

Example: Interupt51.c (PWM via Timer0 and external interrupts)
---------------------------------------------------------

This repository includes an example implementation in `Interupt51.c` that demonstrates how to use Timer0 and external interrupts (INT0 / INT1) on a classic 8051 to implement a simple software PWM for dimming an LED and change the duty cycle using external buttons.

Summary of behavior
- Timer0 runs in Mode 2 (8-bit auto-reload). The Timer0 interrupt (vector 1) is used as the PWM time base.
- A function with a configurable period (defined by PWM_PERIOD) is generated on pin P1.0 (named `led` in the source). The ISR increments a counter `pwm_cnt` each timer tick and sets/clears `led` depending on whether `pwm_cnt < duty`.
- External interrupt INT0 (P3.2) increases the duty by `DUTY_STEP` (up to PWM_PERIOD).
- External interrupt INT1 (P3.3) decreases the duty by `DUTY_STEP` (down to 0).

Key symbols and defaults from `Interupt51.c`
- `sbit led = P1 ^ 0;` — LED output on P1.0.
- `#define PWM_PERIOD 10` — number of timer ticks per PWM cycle (resolution of the duty).
- `#define DUTY_STEP 1` — increment/decrement step when pressing INT0/INT1.
- `volatile unsigned char duty = 50;` — initial duty value in source (note: initial value larger than PWM_PERIOD will be clamped by the ISRs; you can set it to a value between 0 and PWM_PERIOD for sensible behavior).
- `#define FOSC 11059200UL` — CPU clock frequency used to compute timer reload.

Frequency and timer setup
- The helper `set_pwm_frequency(unsigned long freq_hz)` computes the appropriate Timer0 reload for Mode 2 auto-reload based on the system clock and the desired PWM frequency. It uses the formula:

        TIMER_TICK_FREQ = FOSC / 12
        ticks_per_overflow = TIMER_TICK_FREQ / (PWM_PERIOD * freq_hz)

- The function clamps `ticks_per_overflow` between 1 and 256 (0x100) and programs `TH0`/`TL0` accordingly. It returns the actual PWM frequency achieved.

Hardware pinout and wiring
- P1.0 — LED output (connect LED with suitable resistor to Vcc/GND per your board).
- P3.2 — External interrupt INT0 (button or external signal to increase duty).
- P3.3 — External interrupt INT1 (button or external signal to decrease duty).

Notes about the example implementation
- The code configures INT0 and INT1 as edge-triggered (falling edge) by setting `IT0 = 1` and `IT1 = 1` and enables them with `EX0 = 1` and `EX1 = 1`.
- Global interrupts are enabled with `EA = 1` and Timer0 interrupt is enabled with `ET0 = 1`.
- The code sets Timer0 in Mode 2 and then calls `set_pwm_frequency(1000UL);` to attempt a 1 kHz PWM frequency for the overall PWM cycle (the effective LED PWM frequency depends on PWM_PERIOD).
- The main loop is an idle loop; all action happens in ISRs.

Practical tips
- Choose `PWM_PERIOD` for the brightness resolution you need; larger values increase resolution but require a faster timer tick to keep PWM frequency acceptable for visible flicker.
- Ensure `duty` initial value is within 0..PWM_PERIOD. If you want a default of 50% duty, set it to `PWM_PERIOD/2` in the source.
- Debounce external buttons in hardware or in software (e.g., ignore repeated edges for a few ms) if you observe noisy switching.

Build and run
- Open the project in your preferred Keil µVision workspace and build for your target (classic 8051). The build will generate output and intermediate files in the project folders — these are implementation/build artifacts and are safe to ignore from source control if you prefer.
- If you change `FOSC`, `PWM_PERIOD` or the requested frequency in `set_pwm_frequency()`, re-build and program your board.

Try it
1. Edit `Interupt51.c` to set `duty` to a safe initial value, e.g.:

            volatile unsigned char duty = PWM_PERIOD/2; // start at ~50%

2. Flash the generated HEX to your 8051 board. Connect an LED to P1.0 and two buttons to P3.2 and P3.3 (use pull-up and connect button to ground so the external interrupt sees a falling edge).
3. Press the INT0 button to increase brightness, INT1 to decrease.


License / Credits
- Educational example for a classic 8051 microcontroller. Use and adapt for lab exercises. No warranty—test on your hardware carefully.

