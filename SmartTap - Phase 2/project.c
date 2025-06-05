//Mina Tahaei , Danial Bazmandeh, Seyyed Ali Ayati

#include <mega16.h>
#include <alcd.h>
#include <stdio.h>

char buffer[20];
int second = 0 , minute = 0 , hour = 0 , flag1 = 0 , flag2 = 0;

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// bargardoondane maaghadir timer b 0
TCNT1H=0x85EE >> 8;
TCNT1L=0x85EE & 0xff;

//har bar k vaghfe timer etefagh miofte bayad sanie ezafe she
second++;
if (second == 60) {second = 0;minute++;}
if (minute == 60) {minute = 0;hour++;}
if (hour == 24) hour = 0;
}

void main(void)
{

//tanzim portA b onvane voroodi - bekhatere ADC
DDRA=0x00;
PORTA=0x00;
//tanzim portB b onvane khoorooji 
DDRB=0xff;
PORTB=0x00;
// tanzim portC b onvane khoorooji barae etesale LCD
DDRC=0xff;
PORTC=0x00;
//tanzim portD b onvane vooroodi - bekhatere vaqfe hae khareji
DDRD=0x00;
PORTD=0x00;

//tanzimate timer - har 1 sanie vaqfe khahim dasht ta betoonim 1 sanie hae daqiq hesab konim
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
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

//tanzim LCD
lcd_init(20);


while (1)
      { 
        if (PIND.0 == 0)
        {
            #asm("cli")
            
            if (flag1 == 0)
            { 
                lcd_clear();
                flag1 = 1; 
                flag2 = 0;
            }
            lcd_gotoxy(6,0);
            lcd_puts("Tap OFF");
            PORTB.0 = 0; 
            
        }   
        if (PIND.0 == 1)
        {
            #asm("sei")
            if (flag2 == 0)
            { 
                lcd_clear();
                flag1 = 0; 
                flag2 = 1;
            }
            lcd_gotoxy(6,0);
            lcd_puts("Tap ON");
            PORTB.0 = 1;
        }
        
        
        sprintf(buffer,"%02d:%02d:%02d",hour,minute,second);
        lcd_gotoxy(6,1);
        lcd_puts(buffer);

      }
}
