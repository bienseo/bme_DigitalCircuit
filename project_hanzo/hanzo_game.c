// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with ATmega128
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Eunseo Cho. All rights reserved.

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h> // _delay_ms();

// set variables		             
volatile int t_count; // timer count variable

// volatile unsigned int direction;           // direction change: switch

/*
	Dotmatrix display(0 = on, 1 = off)
	Must remember: coloumn5, column6, column7, column8, coloumn1, column2,column3, column4
*/ 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };   // dotmatrix cathode(GND)
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x00 };        // "H" dotmatrix anode(VCC) 
unsigned int target_int[8] = { 0x66, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x66 };    // target
unsigned int count_disp[4][8] = {
   { 0xF7, 0xF7, 0xF7, 0xC3, 0xF7, 0xE7, 0xF7, 0xF7 },  // 1
   { 0xF7, 0xEF, 0xDF, 0x81, 0xC7, 0xBB, 0xFB, 0xFB },  // 2
   { 0xFD, 0xFD, 0xBD, 0xC3, 0xC3, 0xBD, 0xFD, 0xE3 },  // 3
   { 0xDB, 0x81, 0xFB, 0xFB, 0xFB, 0xF3, 0xEB, 0xEB }   // 4 
}; // display count number(cnt = 0,1,2,3)
unsigned int arrow_disp[8][8] = {
   { 0xEF, 0xEF, 0xD7, 0xFF, 0xEF, 0xC7, 0xEF, 0xEF },
   { 0xEF, 0xD7, 0xFF, 0xFF, 0xC7, 0xEF, 0xEF, 0xEF },
   { 0xD7, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xEF },
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xEF, 0xD7 },
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xEF, 0xD7, 0xFF },
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xD7, 0xFF, 0xFF },
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xD7, 0xFF, 0xFF, 0xFF },
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
}; // arrow animation

//unsigned int dot_all[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // display all

// display final result
// column0: force(y axis), column1: direction(x axis)
//unsigned int final_disp[4][2];

/*
    Functions
*/ 
void maintain_disp(void);    	 // make delays for maintain
void port_init(void);        	 // all port initialize
void led_off(void);              // LED off
void green_led(void);         	 // PB0: Green LED for "START sign"
void red_led(void);          	 // PB1: Red LED for "STOP sign"
void dotmatrix_init(void);   	 // dotmatrix initialize: display "H"  
void target_display(void);    	 // display target
void count_display(int cnt); 	 // display count number(cnt = 0,1,2,3)
void arrow_display(void);     	 // display arrow animation
// ADC functions 
void ADC_init(void);           		 // ADC port initialize
unsigned int read_ADC(int channel);	 // read ADC value 
void display_all(void);    // display all LED



/*
   Main script
*/ 
int main(void)
{	
	int chance = 4; // Chance for shooting
	int cnt = 0;	  
	unsigned int flex_val; // ADC value of flex sensor
	unsigned int final_disp[4]; // display final result

	port_init();
	ADC_init();

	red_led();
	dotmatrix_init(50);
	maintain_disp();
	count_display(3);

	while(1)
	{
		green_led();
		target_display();

		flex_val = read_ADC(0);
		if ((flex_val > 50) && (flex_val < 100)){ // flex bent
			led_off();
			arrow_display();
			final_disp[cnt] = flex_val;
			chance--;
			cnt ++;
		}
		else if ((flex_val > 100) && (flex_val < 300)){ // flex resting
			target_display();
		}
		else {
			red_led();
			display_all(30);
			dotmatrix_init(20);
		}

		if (chance == 0){
			break;
		}
	}

	while(1){

	}

}


/* 
   Functions
*/
void maintain_disp(void)
{
   for (int i = 0; i < 64; i++){
      _delay_loop_2(0);
   }
}

void port_init(void)
{
   DDRA = 0xFF;   // dotmatrix VCC 
   DDRC = 0xFF;   // dotmatrix GND
   PORTA = 0xFF;  // dotmatrix VCC
   PORTC = 0xFF;  // dotmatrix GND
   DDRB = 0x00;   // LEDs PB0: GREEN, PB1: RED
   PORTB = 0x00;  // LEDs PB0: GREEN, PB1: RED
   DDRF = 0x00;   // ADC: flex PF0, force PF1
   // DDRD = 0x00;   // External Interrupt: Switch PD0,
   // PORTD = 0xFF;  // External Interrupt: Switch PD0, PD1
}

void led_off(void)
{
	DDRB = 0x00;
    PORTB = 0x00;
    _delay_ms(10);
}

void green_led(void)
{
   // PB0: Green LED for "START sign"
   DDRB = 0x01;
   PORTB = 0x01;
   _delay_ms(100);
}

void red_led(void)
{
   // PB1: Red LED for "STOP sign"
   DDRB = 0x02;
   PORTB = 0x02;
   _delay_ms(100);
}

void display_all(int disp_j) // PORTA, PORTC
{  
   // display all LED
   int i,j;
   for (j = 0; j < disp_j; j++){
      for (i = 0; i < 8; i++){
         PORTA = dot_all[i]; 
         PORTC = dot_int_GND[i];
         _delay_ms(2);
      }
   }
   PORTA = 0xFF;
}

void dotmatrix_init(int disp_j) // PORTA, PORTC
{  
   // display "H" for Hanzo
   int i,j;
   for (j = 0; j < disp_j; j++){
      for (i = 0; i < 8; i++){
         PORTA = dot_int[i]; 
         PORTC = dot_int_GND[i];
         _delay_ms(2);
      }
   }
   PORTA = 0xFF;
}

void target_display(void)// PORTA, PORTC
{  
   // display "target"
   int i,j;

   for (j = 0; j < 50; j++){
      for (i = 0; i < 8; i++){
         PORTA = target_int[i];
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }   
   }
   PORTA = 0xFF;
}

void count_display(int cnt)
{
   // display count number(cnt = 0,1,2,3)
   int i,j;
   for (j = 0; j < 50; j++){
      for (i = 0; i < 8; i++){
         PORTA = count_disp[cnt][i];
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
   }
   PORTA = 0xFF;
}

void arrow_display(void)
{
   // display arrow animation
   int i,j,k,kk;
   for (k = 0; k < 10; k++){
      for (i = 0; i < 8; i++){
         for(kk = 0; kk < 20; kk++){
             for (j = 0; j < 8; j++){
               PORTA = arrow_disp[i][j];
               PORTC = dot_int_GND[j];
               _delay_ms(0.5);
            }
         }
      }
   }
   PORTA = 0xFF;
}

void ADC_init(void)
{ 
   ADMUX=0x00;  // AVCC = 2.56V 1<<REFS0 //기준전압으로 AVCC 사용  
   ADCSRA=0x87; // 0b10000111: ADEN = 1(ADC Enable), Prescaler = 128, 16Mhz/128 = 125Khz  
}

unsigned int read_ADC(int channel) // PF0 channel = 0
{
   unsigned int result = 0;
   ADC_init();

   ADMUX = channel; // AVCC: 4.6V, PF0: flex sensor
   ADMUX |= (0<<REFS1 | 1<<REFS0); 
   ADCSRA |= (1<<ADSC); // ADC start conversion(single conversion)

   result += (int)ADCL + ((int)ADCH<<8); // ADC Low byte, High byte

   return result;
}