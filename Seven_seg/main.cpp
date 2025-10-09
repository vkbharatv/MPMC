#include<reg51.h>
sbit x= P3^0;

unsigned char segment_code[] = {
    0x3F, // 0 ? 00111111
    0x06, // 1 ? 00000110
    0x5B, // 2 ? 01011011
    0x4F, // 3 ? 01001111
    0x66, // 4 ? 01100110
    0x6D, // 5 ? 01101101
    0x7D, // 6 ? 01111101
    0x07, // 7 ? 00000111
    0x7F, // 8 ? 01111111
    0x6F  // 9 ? 01101111
};


void delay(){
int i;
for(i=0;i<30000;i++);
}

 
void main(void)
 { 
 int i=0;
   P3=0;
   while (1)
   {
   for(i=0;i<10;i++)
   {
      P3=segment_code[i];
      delay();
   }
   i=9;
   for(i=9;i>0;i--)
   {
      P3=segment_code[i];
      delay();
   }
   }
 }