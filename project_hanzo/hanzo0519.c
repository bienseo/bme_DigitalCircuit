// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with ATmega128
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Eunseo Cho. All rights reserved.

#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h> // _delay_ms();

# define HIGH 1 // high = 1
# define LOW 0 // low = 0 

// dotmatrix display(0 = on, 1 = off)
// must remember: coloumn5, column6, column7, column8, coloumn1, column2,column3, column4
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,0x00 }; // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }; // dotmatrix cathode(GND)
unsigned int target_int[8] = { 0x66, 0x77, 0x77, 0x00, 0x00, 0x77, 0x77,0x66 }; // target

// bien: change counting number later
unsigned int count_disp[4][8] = {
   {0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c}, // 1
   {0x38, 0x44, 0x04, 0x04, 0x08, 0x10, 0x20, 0x7e}, // 2
   {0x3c, 0x42, 0x02, 0x1c, 0x02, 0x02, 0x42, 0x3c}, // 3
   {0x04, 0x0c, 0x14, 0x14, 0x24, 0x7e, 0x04, 0x04}, // 4 
}; // display count number(cnt = 0,1,2,3)

// variables 
unsigned int v_flex, v_force; // flex sensor 1 voltage, force sensor voltage
unsigned int direction; // flex sensor 2 voltage change 

// functions 
void delay(int n); // delay
void port_init(void); // port initialize
ISR(INT1_vect); // external interrupt
void dotmatrix_int(void); // dotmatrix initialize: display "H"  
void target_display(void); // display target
// bien: change counting number later
void count_display(int cnt); // display count number(cnt = 1,2,3,4)
void green_led(void);// start
void red_led(void); // stop and pause

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
   int cnt;

   port_init(); // All port initialize
   dotmatrix_int(); // Dotmatrix initialize: display "H"
   EIMSK =0x02;
   EICRA = 0xAA;
   SREG = 0x80;

   red_led();  // stop sign
   delay(500);

   target_display();

   green_led(); // start sign
   delay(500);   

   cnt = 3;
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
   PORTA = 0xFF; // dotmatrix VCC 
   PORTB = 0x00;
   PORTC = 0xFF; // dotmatrix GND
   PORTD = 0x00;
   PORTF = 0x00;
   PORTE = 0x03;
   PORTG = 0x00;

   DDRA = 0xFF; // dotmatrix VCC 
   DDRB = 0x00;
   DDRC = 0xFF; // dotmatrix GND
   DDRD = 0x00;
   DDRE = 0x00;
   DDRF = 0x03;
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

void dotmatrix_int(void) // PORTA, PORTC
{
   int i;

   while(1){
      for(i = 0; i < 8; i++){
         PORTA = dot_int[i]; // "H" for Hanzo
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
   }    
   delay(500); // bien: change while conditon later
}

void target_display(void)
{
   int i;

   while(1){  
      for(i = 0; i < 8; i++){
         PORTA = target_int[i]; // "target" for Hanzo
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
   }   
   delay(500);
}

// bien: change counting number later
void count_display(int cnt) // PORTA, PORTC
{
   int i;

   while(1){
      for(i = 0; i < 8; i++){
         PORTA = count_disp[cnt][i]; // display count number(cnt = 0,1,2,3)
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
   }
   delay(500);    
}


void green_led(void){

   DDRF = 0x01;
   PORTF = 0x01;

}

void red_led(void){

   DDRF = 0x02;
   PORTF = 0x02;

}
