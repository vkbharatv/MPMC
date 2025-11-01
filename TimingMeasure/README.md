# TimingMeasure (8051)

Measure the period (time between two consecutive falling edges) of a digital signal on INT0 (P3.2) using Timer0 with a 1 ms tick on a standard 12 MHz 8051/AT89C51.

This project contains a single C file `time_measure.c`.

## How it works

- Timer0 is configured in Mode 1 (16‑bit). With a 12 MHz crystal, the 8051 timer increments every 12 cycles (1 µs).
- The timer ISR fires every ~1 ms. In the ISR:
	- `time_ms++` accumulates elapsed milliseconds.
	- `TH0/TL0` are reloaded to `0xFC18` for the next 1 ms interval.
- External Interrupt 0 (INT0, pin P3.2) is set to falling‑edge triggered (`IT0 = 1`). Each falling edge toggles the timer run state (`TR0 = !TR0`):
	- First falling edge: starts timing.
	- Next falling edge: stops timing and computes the elapsed time.
- On stop, the code computes an integer number of seconds into `t`:
	- `t = (time_ms + ((TH0 > 0xFC) ? 1 : 0)) / 1000;`
	- Then it reinitializes the timer for the next measurement.
- In the main loop:
	- Port P2 outputs `t` (integer seconds, 0–255), which you can view on LEDs or a logic probe.
	- A float `f = 1.0 / t` is computed (in Hz) but not displayed.
    - Use the LCD or Serial communication to display/print the frequency.

Result: The measured interval is the full period of the input waveform between two consecutive falling edges.

## Timer math (reload value)

Timer tick frequency on a classic 12T 8051 is `Fosc / 12`. For a 1 ms interrupt period, the reload value is:

$$\text{reload} = 65536 - \left( \frac{F_{osc}}{12} \times \frac{1}{1000} \right)$$

Examples:

- 12.000 MHz: counts per ms = 1000 → reload = 65536 − 1000 = 64536 = `0xFC18`.
- 11.0592 MHz: counts per ms = 921.6 → use 922 → reload ≈ 65536 − 922 = 64614 = `0xFC66` (≈0.04% error).

If your MCU uses a different clock or a 6T core, adjust `TH0/TL0` accordingly.

## Hardware setup

- INT0 input: P3.2 — connect your signal (0–5 V TTL/CMOS) and ensure a common GND.
- Output display: P2 carries the integer seconds (`t`). You can connect 8 LEDs (with resistors) to P2 for a simple display.
- 
- Debounce: If using a push‑button to generate edges, add debouncing. The code assumes a clean falling edge.

## Build and run (Keil µVision)

1. Create a project Keil µVision.
2. Add the `time_measure.c` in the project sourse folder.
3. Select an 8051/AT89C51‑compatible device and ensure the oscillator frequency matches your hardware (e.g., 12 MHz).
4. Build the project and program your target.
5. Provide a periodic signal to P3.2 (INT0). After two falling edges, P2 updates with the measured period in whole seconds.

## Usage notes and limitations

- Resolution: Base time resolution is 1 ms, but `t` stores only whole seconds (integer division). Periods < 1 s will show `t = 0` on P2.
- Range: `t` is `unsigned char` → 0–255 s max displayable period. The internal millisecond counter is 32‑bit (`unsigned long`).
- Frequency readout: `f = 1.0 / t` is computed but not presented on any interface. For `t = 0` (period = 0 s), `f` is set to 0.
- Edge selection: Only falling edges on INT0 are used. Measurement spans falling‑to‑falling (full period).
- Clock dependency: The 1 ms tick assumes 12 MHz and classic 12T timing. Recalculate reload for other clocks/cores.

## Improving this example (ideas)

- Display sub‑second periods: expose millisecond resolution on LCD or via UART.
- Print results: send `time_ms` and computed frequency over serial at 9600 baud (easy with 11.0592 MHz).
- Avoid ISR reload: switch to auto‑reload (Mode 2, 8‑bit) for lower ISR overhead if 1 ms resolution is acceptable.
- Debounce/qualify INT0: add a simple digital filter if the source is noisy.


---


