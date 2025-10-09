#include<reg51.h>
#include<stdio.h>
#include<string.h>

//==================================================
// LCD interfacing with 8051/52 micro controller
// by Dr. Bharat Verma, LNMIIT
//==================================================


//=================
// PIN DEFINITIONS
//=================
#define lcd_data_port P2  // LCD Data Bus connected to Port 1

sbit en = P0^0;           // Enable pin
sbit rw = P0^1;           // Read/Write pin
sbit rs = P0^2;           // Register Select pin

void delay_ms(unsigned int ms);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char dat);
void lcd_init();
void lcd_string(unsigned char *str);
void lcd_gotoxy(unsigned char row, unsigned char col);


void lcd_init()
{

    delay_ms(20);         // Wait for LCD power-on
    lcd_command(0x38);    // Function Set: 8-bit mode, 2 lines, 5x7 font
    lcd_command(0x0C);    // Display Control: Display ON, Cursor OFF, Blink OFF
    lcd_command(0x01);    // Clear Display Screen
    lcd_command(0x06);    // Entry Mode Set: Increment cursor, no display shift
    lcd_command(0x80);    // Force cursor to the beginning of the 1st line
}


void lcd_command(unsigned char cmd)
{
    lcd_data_port = cmd;  // Load command on the data port
    rs = 0;               // Select Command Register (RS=0)
    rw = 0;               // Select Write operation (RW=0)
    en = 1;               // Pulse the Enable pin
    delay_ms(1);
    en = 0;
}


void lcd_data(unsigned char dat)
{
    lcd_data_port = dat;  // Load data on the data port
    rs = 1;               // Select Data Register (RS=1)
    rw = 0;               // Select Write operation (RW=0)
    en = 1;               // Pulse the Enable pin
    delay_ms(1);
    en = 0;
}


void lcd_string(unsigned char *str)
{
    while(*str != '\0')   // Loop until the end of the string
    {
        lcd_data(*str);   // Send the character to the LCD
        str++;            // Move to the next character
    }
}

void lcd_gotoxy(unsigned char row, unsigned char col)
{
    unsigned char position;
    if(row == 0)
    {
        position = 0x80 + col; // For 1st line, address starts from 0x80
    }
    else if(row == 1)
    {
        position = 0xC0 + col; // For 2nd line, address starts from 0xC0
    }
    lcd_command(position);
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 1275; j++); // This loop count provides an approximate 1ms delay
    }
}

