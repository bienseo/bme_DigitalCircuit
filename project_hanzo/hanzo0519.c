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
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,0x00 };         // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };   // dotmatrix cathode(GND)
unsigned int target_int[8] = { 0x66, 0x77, 0x77, 0x00, 0x00, 0x77, 0x77,0x66 };     // target
unsigned int count_disp[4][8] = {
   { 0xF7, 0xF7, 0xF7, 0xC3, 0xF7, 0xE7, 0xF7, 0xF7 },  // 1
   { 0xF7, 0xEF, 0xDF, 0x81, 0xC7, 0xBB, 0xFB, 0xFB },  // 2
   { 0xFD, 0xFD, 0xBD, 0xC3, 0xC3, 0xBD, 0xFD, 0xE3 },  // 3
   { 0xDB, 0x81, 0xFB, 0xFB, 0xFB, 0xF3, 0xEB, 0xEB }   // 4 
}; // display count number(cnt = 0,1,2,3)

// ADC variables 
unsigned int v_flex, v_force; // flex sensor 1 voltage, force sensor voltage
unsigned int direction;       // flex sensor 2 voltage change 

// functions 
void delay(int n);            // delay function
void port_init(void);         // all port initialize
ISR(INT1_vect);               // external interrupt enable
void green_led(void);         // PF0: Green LED for "START sign"
void red_led(void);           // PF1: Red LED for "STOP sign"
void dotmatrix_int(void);     // dotmatrix initialize: display "H"  
void target_display(void);    // display target
void count_display(int cnt);  // display count number(cnt = 1,2,3,4)

// on going
void display(unsigned int num); // using switch and case -> make sate and flows
void ADC_int(unsigned char channel); // ADC port initialize
int read_ADC(void); // read value
void get_flex(unsigned int v_flex); // using switch and case -> make sate and flow
void get_force(unsigned int v_force); // using switch and case -> make sate and flow
void is_change(unsigned int direction); // the flex sensor on the wrist: get direction


// main script
int main(void)
{
   int cnt = 3;

   port_init(); // All port initialize
   dotmatrix_int(); // Dotmatrix initialize: display "H"
   delay(500);

   EIMSK =0x02;
   EICRA = 0xAA;
   SREG = 0x80;

   // all LED ON 
   PORTF = 0x03;
   delay(500);

   do{

      red_led(); // stop sign

      target_display(); // display "target"

      green_led(); // start sign

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
      cnt--;

   }while(cnt > 0);
}


// functions 

// delay function
void delay(int n)
{
   volatile int i,j;
   for(i=1; i<n; i++){
      for(j=1; j<900; j++);
   }
}

// all port initialize
void port_init(void)
{
   PORTA = 0xFF; // dotmatrix VCC 
   PORTB = 0x00;
   PORTC = 0xFF; // dotmatrix GND
   PORTD = 0x00;
   PORTF = 0x00;
   PORTE = 0x00;
   PORTG = 0x00;

   DDRA = 0xFF; // dotmatrix VCC 
   DDRB = 0x00;
   DDRC = 0xFF; // dotmatrix GND
   DDRD = 0x00;
   DDRE = 0x00;
   DDRF = 0xFF; // LEDs and ADC(flex and force) 
   DDRG = 0x03;
}

// external interrupt enable
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
   // display "H" for Hanzo
   int i;

   for(i = 0; i < 8; i++){
      PORTA = dot_int[i]; 
      PORTC = dot_int_GND[i];
      _delay_ms(2); 
   }    
   delay(500);
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
   delay(500);
}

void count_display(int cnt) // PORTA, PORTC
{  
   // display count number(cnt = 0,1,2,3)
   int i;
   
   for(i = 0; i < 8; i++){
      PORTA = count_disp[cnt][i];
      PORTC = dot_int_GND[i];
      _delay_ms(2); 
   }
   delay(500);    
}

void green_led(void){
   // PF0: Green LED for "START sign"
   DDRF = 0x01;
   PORTF = 0x01;
   delay(500);
}

void red_led(void){
   // PF1: Red LED for "STOP sign"
   DDRF = 0x02;
   PORTF = 0x02;
   delay(500);
}
