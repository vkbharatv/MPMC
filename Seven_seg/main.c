#include <reg51.h> // ?? The essential correction

#define seg_data P3   // Segment data port
sbit s1 = P2^0;       // Control for display 1 (tens)
sbit s2 = P2^1;       // Control for display 2 (units)
sbit sw = P1^0;
// 7-segment codes for digits 0–9 (common cathode)
unsigned char segment_code[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Simple delay function (~1 ms per iteration)
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1275; j++);
}

// Function to display two digits using multiplexing
void display_digits(unsigned char tens, unsigned char units) {
    // Display tens digit
    s1 = 0;   // Enable display 1
    s2 = 1;   // Disable display 2
    seg_data = segment_code[tens];
    delay_ms(1);

    // Display units digit
    s1 = 1;
    s2 = 0;
    seg_data = segment_code[units];
    delay_ms(1);
}

void main(void) {
    unsigned char i, j, k;
		sw=0;
    while (1) {
        for (i = 0; i < 10; i++) {       // Tens digit 0 to 9
            for (j = 0; j < 10; j++) {   // Units digit 0 to 9
                for (k = 0; (k < 1) || (sw == 1); k++) { 
                    display_digits(i, j);
                }
            }
        }
    }
}