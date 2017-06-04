// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with ATmega128
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Eunseo Cho. All rights reserved.

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h> // _delay_ms();

# define ADC_PF0 0x81; // AVCC, PF0: force sensor
# define ADC_PF1 0x82; // AVCC, PF1: flex sensor

// set variables
volatile int chance = 4; 			             // Chance for shooting	  
volatile unsigned char adcVectL, adcVectH;    // ADC Low byte, High byte
// volatile unsigned int direction;           // direction change: switch

// dotmatrix display(0 = on, 1 = off)
// must remember: coloumn5, column6, column7, column8, coloumn1, column2,column3, column4
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x00 };         // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };   // dotmatrix cathode(GND)
unsigned int target_int[8] = { 0x66, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x66 };     // target
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

// display final result
// column0: force(y axis), column1: direction(x axis)
unsigned int final_disp[4][2];

// functions
void maintain_disp(void);     // make delays for maintain
void port_init(void);         // all port initialize
void green_led(void);         // PB0: Green LED for "START sign"
void red_led(void);           // PB1: Red LED for "STOP sign"
void dotmatrix_init(void);    // dotmatrix initialize: display "H"  
void target_display(void);    // display target
void count_display(int cnt);  // display count number(cnt = 0,1,2,3)
void arrow_display(void);     // display arrow animation

// ADC functions 
void ADC_init(void);           // ADC port initialize
double read_ADC(unsigned int adc_port);	      // read ADC value 
void get_force(int c_num);    // PF0: get force sensor voltage
void get_flex(int c_num);      // PF1: get flex sensor voltage

// on going
void is_change(unsigned int direction);  // get direction from the switch or whatever
void make_wind(void); 				        // obstacle wind
void fin_display(void);


// main script
int main(void)
{
   int cnt = 3; // counting number
   int c_num = chance; // chance number

   port_init();
   dotmatrix_init();
   red_led();

   while(chance != 0){
      green_led();
      target_display();

      if (cnt < 0){
         cnt = 3;
      }
      count_display(cnt);
      cnt--;

      arrow_display();

      final_disp[4-chance][0] = get_force(c_num);
      final_disp[4-chance][1] = get_flex(c_num);
      chance--;
   }
   
   /*
   if (chance == 0){
      fin_display(); // after reading ADC 
   }*/
}


// functions
void maintain_disp(void)
{
   for (int i = 0; i < 64; i++){
      _delay_loop_2(0);
   }
}

void port_init(void)
{
   // GPIO
   DDRA = 0xFF;   // dotmatrix VCC 
   DDRB = 0x00;   // LEDs PB0: GREEN, PB1: RED
   DDRC = 0xFF;   // dotmatrix GND
   PORTA = 0xFF;  // dotmatrix VCC 
   PORTB = 0x00;  // LEDs PB0: GREEN, PB1: RED
   PORTC = 0xFF;  // dotmatrix GND

   DDRG = 0x00;   // Switch: OFF = 0, ON = 1
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
   _delay_ms(200);
}

void red_led(void){
   // PB1: Red LED for "STOP sign"
   DDRB = 0x02;
   PORTB = 0x02;
   _delay_ms(200);
}

void dotmatrix_init(void) // PORTA, PORTC
{  
   // display "H" for Hanzo
   int i,j;
   for (j = 0; j < 60; j++){
      for (i = 0; i < 8; i++){
         PORTA = dot_int[i]; 
         PORTC = dot_int_GND[i];
         _delay_ms(2);
      }
   }
   PORTA = 0x00;
}

void target_display(void)// PORTA, PORTC
{  
   // display "target"
   int i,j;

   for (j = 0; j < 60; j++){
      for (i = 0; i < 8; i++){
         PORTA = target_int[i];
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }   
   }
   PORTA = 0x00;
}

void count_display(int cnt)
{
   // display count number(cnt = 0,1,2,3)
   int i,j;
   for (j = 0; j < 60; j++){
      for (i = 0; i < 8; i++){
         PORTA = count_disp[cnt][i];
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
   }
   PORTA = 0x00;
}

void arrow_display(void)
{
   // display arrow animation
   int i,j,k;
   for (k = 0; k < 20; k++){
      for (i = 0; i < 8; i++){
         for (j = 0; j < 8; j++){
            PORTA = arrow_disp[i][j];
            PORTC = dot_int_GND[j];
            _delay_ms(2);
         }
      }
   }
   PORTA = 0x00;
}

void ADC_init(void)
{ 
   cli();
   ADMUX=0x80;  // AVCC = 2.56V
   ADCSRA=0x87; // 0b10000111: ADEN = 1(ADC Enable), Prescaler = 128, 16Mhz/128 = 125Khz  
   sei();
}

double read_ADC(unsigned int adc_port)
{
   double adc_val = 0;
   adcVectL = ADCL;
   adcVectH = ADCH;
   ADC_init();
   _delay_ms(10);

   ADMUX = adc_port;
   ADCSRA |= 0x40; // ADC start conversion(single conversion)

   while((ADCSRA & 0x10) == 0){
      adc_val = adcVectL + (adcVectH << 8);
   }
   return adc_val;
}

void get_force(int c_num)
{  
   if (c_num == 0){
      return chance = 0;
   }
   else{
      // PF0: force sensor
      volatile double v_force = read_ADC(ADC_PF0); // force sensor voltage
      return v_force;  
   }
}

void get_flex(int c_num)
{
   if (c_num == 0){
      return chance = 0;
   }
   else{
      // PF1: flex sensor
      volatile double v_flex = read_ADC(ADC_PF1); //flex sensor voltage
      return v_flex;
   }
}
