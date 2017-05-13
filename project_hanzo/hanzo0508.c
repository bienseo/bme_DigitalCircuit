// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with atmega128
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Eunseo Cho. All rights reserved.

#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h> // _delay_ms();

# define HIGH 1 // high = 1
# define LOW 0 // low = 0 

// dotmatrix display 
unsigned int dot_int[8] = { 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42 }; // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }; // dotmatrix cathode(GND)
unsigned int target_int[8] = { 0xff, 0x81, 0x81, 0x99, 0x99, 0x81, 0x81,0xff }; // target
unsigned int count_disp[4][8] = {
   {0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c}, // 1
   {0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x20, 0x7e}, // 2
   {0x3c, 0x42, 0x02, 0x1c, 0x02, 0x02, 0x42, 0x3c}, // 3
   {0x04, 0x0c, 0x14, 0x14, 0x24, 0x7e, 0x04, 0x04}, // 4 
}; // display count number(cnt = 1,2,3,4)

// variables 
unsigned int v_flex, v_force; // flex sensor 1 voltage, force sensor voltage
unsigned int direction; // flex sensor 2 voltage change 

// functions 
void delay(int n); // delay
void port_init(void); // port initialize
ISR(INT1_vect); // external interrupt
void green_led(void){ PORTC = 0x80; } // start
void red_led(void){ PORTC = 0x40; } // stop and pause
void dotmatrix_int(void); // dotmatrix initialize: display target after showing "H"  
void count_display(int cnt); // display count number(cnt = 1,2,3,4)

// on going
void display(unsigned int num); //using switch and case -> make sate and flows
void ADC_int(unsigned char channel); // ADC port initialize
int read_ADC(void); // read value
void get_flex(unsigned int v_flex); //using switch and case -> make sate and flow
void get_force(unsigned int v_force); //using switch and case -> make sate and flow
void is_change(unsigned int direction); // the flex sensor on the wrist: get direction


// main script
int main(void)
{
   int i,j,cnt; // iteration term

   port_init(); // All port initialize
   dotmatrix_int(); // Dotmatrix initialize: display "H"
   EIMSK =0x02;
   EICRA = 0xAA;
   SREG = 0x80;

   red_led(); // stop sign
   delay(500);
   green_led(); // start sign
   delay(500);

   cnt = 4;
   count_display(cnt); // display: 4
   delay(500);
   cnt--;
   count_display(cnt); // display: 3
   delay(500);
   cnt--;
   count_display(cnt); // display: 2
   delay(500);
   cnt--;
   count_display(cnt); // display: 1
   delay(500);

}


// functions 
void delay(int n)
{
   volatile int i,j;
   for(i=1; i<n; i++){
      for(j=1; j<900; j++);
   }
}

void port_init(void)
{
   PORTA = 0x00;
   PORTB = 0x00;
   PORTC = 0x00;
   PORTD = 0x00;
   PORTF = 0x00;
   PORTE = 0x00;
   PORTG = 0x00;

   DDRA = 0x00;
   DDRB = 0x00;
   DDRC = 0x00;
   DDRD = 0x00;
   DDRE = 0x00;
   DDRF = 0x00;
   DDRG = 0x03;
}


ISR(INT1_vect)
{
   for(int kk=0; kk<3; kk++){
      PORTG = 0x01;
      delay(500);
      PORTG = 0x00;
      delay(500);
   }
}

void dotmatrix_int(void)
{
   int i;
   while(1){
      for(i = 0; i < 8; i++){
         PORTE = dot_int[i]; // "H" for Hanzo
         PORTA = dot_int_GND[i];
         _delay_ms(2); 
      }
   }    
   delay(500);

   while(1){  
      for(i = 0; i < 8; i++){
         PORTE = target_int[i]; // "H" for Hanzo
         PORTA = dot_int_GND[i];
         _delay_ms(2); 
      }
   }
}

void count_display(int cnt)
{
   int i;
   while(1){
      for(i = 0; i < 8; i++){
         PORTE = count_disp[cnt][i]; // display count number(cnt = 1,2,3,4)
         PORTA = dot_int_GND[i];
         _delay_ms(2); 
      }
   }    
}
