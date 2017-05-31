// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with ATmega128
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Eunseo Cho. All rights reserved.

#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h> // _delay_ms();

// set variables
int chance = 4; 			  // Chance for shooting
int cnt = 3; 				  // counting number
volatile unsigned char adcVectL, adcVectH;
volatile unsigned int ADC_val; 		       // ADC value
volatile unsigned int v_flex, v_force;     // flex sensor voltage, force sensor voltage
volatile unsigned int direction;           // direction change: switch

// dotmatrix display(0 = on, 1 = off)
// must remember: coloumn5, column6, column7, column8, coloumn1, column2,column3, column4
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,0x00 };         // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };   // dotmatrix cathode(GND)
unsigned int target_int[8] = { 0x66, 0x77, 0x77, 0x00, 0x00, 0x77, 0x77,0x66 };     // target
unsigned int count_disp[4][8] = {
   { 0xF7, 0xF7, 0xF7, 0xC3, 0xF7, 0xE7, 0xF7, 0xF7 },  // 1
   { 0xF7, 0xEF, 0xDF, 0x81, 0xC7, 0xBB, 0xFB, 0xFB },  // 2
   { 0xFD, 0xFD, 0xBD, 0xC3, 0xC3, 0xBD, 0xFD, 0xE3 },  // 3
   { 0xDB, 0x81, 0xFB, 0xFB, 0xFB, 0xF3, 0xEB, 0xEB }   // 4 
}; // display count number(cnt = 0,1,2,3)

// display final result
// column0: force(y axis), column1: direction(x axis)
unsigned int final_disp[4][2];


// functions
void port_init(void);         // all port initialize
void green_led(void);         // PB0: Green LED for "START sign"
void red_led(void);           // PB1: Red LED for "STOP sign"
void dotmatrix_int(void);     // dotmatrix initialize: display "H"  
void target_display(void);    // display target
void count_display(int cnt);  // display count number(cnt = 0,1,2,3)
void ADC_int(void);           // ADC port initialize
int read_ADC(void); 	         // read ADC value 
ISR(INT0_vect);               // Interrupt Service Routine: external interrupt INT0
void timer_set(void);         // Timer setting and initialize

// on going
void get_flex(unsigned int v_flex); 	  // PF0: using switch and case -> make sate and flow
void get_force(unsigned int v_force);    // PF1: using switch and case -> make sate and flow
void is_change(unsigned int direction);  // get direction from the switch or whatever
void make_wind(void); 				        // obstacle wind
void fin_display(void);


// main script
int main(void)
{
   timer_set(); // Timer setting and initialize
   SREG = 0x80; // Global interrupt enable
   EIMSK =0x01; // External interrupt mask: INT0
   EICRA = 0xAA; // trigger low edge 

   port_init(); // All port initialize
   dotmatrix_int(); // Dotmatrix initialize: display "H"
   red_led(); // stop sign

   sei(); // enable interrupts

   while(chance > 0){
      cli(); // disable all interrupts
      green_led(); // start sign
      target_display(); // display "target"
      _delay_ms(500);

      while(cnt > 0){
         count_display(cnt); // display: 4,3,2,1
         _delay_ms(500);
         cnt --;
      }

      read_ADC();
      if (ADC_val < 0){
         read_ADC();
      }
      else{
         chance --;
      }
   }

}


// functions
void port_init(void)
{
   // GPIO
   DDRA = 0xFF;   // dotmatrix VCC 
   DDRB = 0x03;   // LEDs PB0: GREEN, PB1: RED
   DDRC = 0xFF;   // dotmatrix GND
   PORTA = 0xFF;	// dotmatrix VCC 
   PORTB = 0x03;	// LEDs PB0: GREEN, PB1: RED
   PORTC = 0xFF; 	// dotmatrix GND

   DDRG = 0x03;   // Switch: OFF = 0, ON = 1
   PORTG = 0x00;  // PG2 = 0x04, PG3 = 0x08

   DDRF = 0x00;   // ADC: flex PF0, force PF1
   PORTF = 0x00;  // ADC: flex PF0, force PF1

   PORTD = 0x00;
   PORTE = 0x00;
   DDRD = 0x00;
   DDRE = 0x00;
}

void green_led(void){
   // PB0: Green LED for "START sign"
   DDRB = 0x01;
   PORTB = 0x01;
   _delay_ms(500);
}

void red_led(void){
   // PB1: Red LED for "STOP sign"
   DDRB = 0x02;
   PORTB = 0x02;
   _delay_ms(500);
}

void dotmatrix_int(void) // PORTA, PORTC
{  
   // display "H" for Hanzo
   int i,j;
   for(j = 0; j < 3; j++){
      for(i = 0; i < 8; i++){
         PORTA = dot_int[i]; 
         PORTC = dot_int_GND[i];
         _delay_ms(2);
      }
      PORTA = 0x00;
      _delay_ms(200);
   }
   _delay_ms(500);
}

void target_display(void)// PORTA, PORTC
{  
   // display "target"
   int i;

   for(i = 0; i < 8; i++){
      PORTA = target_int[i];
      PORTC = dot_int_GND[i];
      _delay_ms(2); 
   }   
   _delay_ms(500);
}

void count_display(int cnt){
   // display count number(cnt = 0,1,2,3)
   int i;
   
   for(i = 0; i < 8; i++){
      PORTA = count_disp[cnt][i];
      PORTC = dot_int_GND[i];
      _delay_ms(2); 
   }
   _delay_ms(500);
}

void ADC_int(void){ // add ADC_Input later 
   ADMUX=0x00; //  bien: PF0 ::내부전압 사용, PORTF0 사용
   ADCSRA=0xA7; // free running mode
   _delay_ms(500);
}

int read_ADC(void){

   ADC_int();
   ADCSRA |= 0x40; // ADC start conversion

   adcVectL = ADCL;
   adcVectH = ADCH;
   ADC_val = adcVectL + (adcVectH << 8);
   printf("ADC value is : %d\n", ADC_val);
   _delay_ms(500);

   return ADC_val; // Is it Okay with that: just return ADC?
}

/*
// Interrupt Service Routine INT0
ISR(INT0_vect)
{


}*/

void timer_set(void){
   ASSR=0x00;
   TCCR0=0x07; // clk/1024
   TCNT0=0x00;
   OCR0=0x00;
   TIMSK=0x02;
   ETIMSK=0x00;
   ACSR=0x80;
   SFIOR=0x00;
}
/*
ISR(TIMER0_COMP_vect){

}

*/
