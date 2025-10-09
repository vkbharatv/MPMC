#include <reg51.h>
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#define adc_port P1
sbit rd   = P0 ^ 3;
sbit wr   = P0 ^ 4;
sbit intr = P0 ^ 5;

unsigned int read_adc();

void main()
{
    int adc_value;
    float V;
    char buffer[16];

    // Set ADC data bus as input and control pins as output/input
    adc_port = 0xFF; // Set P2 as an input port
    intr     = 1;    // Set INTR pin as input
    wr       = 1;
    rd       = 1;

    lcd_init(); // Initialize the LCD
    while (1) {
        adc_value = read_adc(); // Read the digital value from ADC
        V         = ((float)adc_value * 5 / 255);
        sprintf(buffer, "V = %0.2f V", V);
        lcd_gotoxy(0, 0);   // Move cursor to the second line
        lcd_string(buffer); // Display the value on LCD
        delay_ms(10);       // Wait before the next reading
        lcd_gotoxy(1, 0);
        sprintf(buffer, "Raw = %d", adc_value);
        lcd_string(buffer);
    }
}

unsigned int read_adc()
{
    int value;
    wr = 0; // 2. Send a low pulse to WR to start conversion
    delay_ms(1);
    wr = 1;

    while (intr == 1); // 3. Wait for the conversion to complete (INTR goes low)

    rd = 0; // 4. Pull RD low to read the data
    delay_ms(1);
    value = adc_port; // 5. Read the 8-bit data from the bus
    rd    = 1;        // 6. Pull RD high to finish the read cycle

    return value;
}
