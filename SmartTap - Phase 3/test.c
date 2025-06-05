/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Smart Water Tap
Version : 3
Date    : 2021-01-30
Author  : Seyyed Ali Ayati, Mina Tahaei, Danial Bazmandeh 
Company : IUST
Comments: 


Chip type               : ATmega16A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16a.h>
#include <delay.h>
#include <stdio.h>

// Declare your global variables here
int hour = 0;
int minute = 0;
int second = 0;
unsigned char is_on = 0;

/*
    LCD DRIVER By Seyyed Ali Ayati
*/
#define LCD_DATA PORTB  // Port B as data
#define ctrl PORTD      // Port D as Controller
#define en PIND.2       // Enable: D2                   
#define rw PIND.1       // RW: D1              
#define rs PIND.0       // RS: D0
void LCD_cmd(unsigned char cmd);
void init_LCD(void);
void LCD_write(unsigned char data);

void init_LCD(void)   // Initialize the LCD
{
    LCD_cmd(0x38);                     
    delay_ms(1);
    LCD_cmd(0x01); // Clear screen
    delay_ms(1);
    LCD_cmd(0x0E); // Enable display and cursor
    delay_ms(1);
    LCD_cmd(0x80); // Goto 0,0            
    delay_ms(1);
    return;
}
 
void LCD_cmd(unsigned char cmd)  // Send Command to LCD
{
    LCD_DATA=cmd;
    ctrl=0x04;               // Enable=1
    delay_ms(1);
    ctrl=0x00;                // Enable=0
    delay_ms(50);
    return;
}
 
void LCD_write(unsigned char data)   // Write data on LCD 
{
    LCD_DATA= data;
    ctrl=0x05;                 // Enable=1 and Select Registers
    delay_ms(1);
    ctrl=0x01;
    delay_ms(50);
    return ;
}

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0x85EE >> 8;
TCNT1L=0x85EE & 0xff;
// Place your code here
if(is_on)
{
    second++;
    if (second == 60)
    { 
        second = 0;
        minute++;
        if (minute == 60)
        {
            minute = 0;
            hour++;
            if(hour == 24)
            {
                hour = 0;
            }
        }
    }
}

}

void main(void)
{
// Declare your local variables here
unsigned char i,microlearn[8]="On Time";

// Input/Output Ports initialization

// Port A initialization
DDRA=0xFF; // All Output 
PORTA=0x02;

// Port B initialization 
DDRB=0xFF; // All Output 
PORTB=0x00;

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=0x00; // All Input
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=0x00;

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=0x07; // First 3 bits as Output 
PORTD=0x00;


// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 1 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x85;
TCNT1L=0xEE;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// Global enable interrupts
#asm("sei")

init_LCD(); 
LCD_cmd(0x01); // Clear screen
LCD_cmd(0x0C); // Hide Cursor
   
// Write On Time     
for(i=0;i<7;i++)                 
{
    LCD_write(microlearn[i]);   
}
for(i=7;i<20;i++)                 
{
    LCD_cmd(0x14);
}

  

while (1)
      {
      // Place your code here
      // delay_ms(1000);
      is_on = PINC.0;  
      PORTA.0 = is_on;
      
      // Show Timer
      LCD_write('0' + hour / 10);
      LCD_write('0' + hour % 10);
      LCD_write(58);             // :
      LCD_write('0' + minute / 10);
      LCD_write('0' + minute % 10);
      LCD_write(58);
      LCD_write('0' + second / 10);
      LCD_write('0' + second % 10);
      
      for(i=0;i<8;i++)                 
        {
            LCD_cmd(0x10); // Move -1
        }

      }
}
