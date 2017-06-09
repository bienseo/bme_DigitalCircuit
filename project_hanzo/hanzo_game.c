// Digital circuit Project
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with ATmega128
// *** Display "Arrow Shooting" using dotmatrix and flex sensor ***
// Created by Eunseo Cho on 08/05/2017.
// Copyright © 2017 Team 2. All rights reserved.

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h> // ISR
#include <util/delay.h>    // _delay_ms();

// Set variables                  
volatile int t_count = 0;  // Timer count variable

/*
  Dotmatrix display(0 = on, 1 = off)
  Must remember: coloumn5, column6, column7, column8, coloumn1, column2,column3, column4
*/ 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };  // dotmatrix cathode(GND)
unsigned int dot_int[8] = {0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x00 };       // H 
unsigned int target_int[8] = { 0x66, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x66 };   // target
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
unsigned int dot_all[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // display all

/*
    Functions
*/ 
void maintain_disp(void);                 // make delays for maintain
void port_init(void);                     // all port initialize
void led_off(void);                       // LED off
void green_led(void);                     // PB0: Green LED for "START sign"
void red_led(void);                       // PB1: Red LED for "STOP sign"
void dotmatrix_init(int disp_j);          // dotmatrix initialize: display "H"  
void target_display(int disp_j);          // display target
void count_display(int cnt, int disp_j);  // display count number(cnt = 0,1,2,3)
void count_down(int disp_j);              // count down 3, 2, 1
void arrow_display(void);                 // display arrow animation
// ADC functions 
void ADC_init(void);                 // ADC initialize
unsigned int read_ADC(int channel);  // read ADC value 
void final_display(int disp_j);      // display final results
/*
void timer_init(void) // Timer initialize
{
    TCCR0=0x07;  // 1024 from prescaler
    TIMSK=0x02;  // enable overflow(TOIE0 =1)
    ACSR=0x80;
    TCNT0=0x64;  // TCNT0 = 100
    OCR0=0x00;
    ASSR=0x00;
    ETIMSK=0x00;
    SFIOR=0x00;
}

ISR(TIMER0_OVF_vect) // if: TCNT0 overflow jump to here
{
   if(t_count > 100){ t_count = 0; }
   else{ t_count++; }
   TCNT0 = 0x64; // TCNT0 = 100
}*/ // timer doesnt work

/*
   Main script
*/ 
int main(void)
{ 
  // Set variables   
  int arrow;           // arrow column factor
  int lose_point = 0;  // failure count
  int chance = 4;      // Chance for shooting
  int cnt = 0;         // counting number 
  unsigned int flex_val;      // ADC value of flex sensor
  unsigned int final_disp[4]; // display final result

  port_init(); // all port initialize
  ADC_init();  // ADC initialize

  red_led(); // "STOP sign"
  dotmatrix_init(60); // dotmatrix initialize: display "H"  
  maintain_disp(); // make delays for maintain

  count_display(3,60); // display chance number: 4

  while(1)
  {
    green_led(); // "START sign"
    target_display(100); // display target
    maintain_disp(); // make delays for maintain

    count_down(50);  // count down 3, 2, 1
    _delay_ms(200); 

    flex_val = read_ADC(0); // read ADC value of flex sensor
    _delay_ms(200);

    if ((flex_val > 30) && (flex_val < 100)) // flex bent condition: ADC value is "60 ~ 90" (check with UART)
    { 
      led_off(); // LED off
      arrow_display(); // display arrow animation

      final_disp[cnt] = (flex_val / 10) - 5; // make ADC value as "0,1,2,3"

      if ((cnt == 0)|(cnt == 1)){
        arrow = final_disp[cnt]; // dotmatrix display(column) max: 0, min: 3
      }
      else if((cnt == 2)|(cnt == 3)){
        arrow = final_disp[cnt]; // make ADC value as "4,5,6,7"
        arrow = 7 - arrow; // dotmatrix display(column) max: 7, min: 4
      }

      // failure count with lose_point 
      // and determine the result points(kind of displaying arrow points)
      if (lose_point == 0){ dot_all[arrow] |= 0x10; }
      else if ((lose_point == 1) && ((cnt == 0)|(cnt == 1))){ dot_all[arrow] |= 0x20; }
      else if ((lose_point == 1) && ((cnt == 2)|(cnt == 3))){ dot_all[arrow] |= 0x08; }            
      else if ((lose_point == 2) && ((cnt == 0)|(cnt == 1))){ dot_all[arrow] |= 0x40; }
      else if ((lose_point == 2) && ((cnt == 2)|(cnt == 3))){ dot_all[arrow] |= 0x04; }  
      else if ((lose_point == 3) && ((cnt == 0)|(cnt == 1))){ dot_all[arrow] |= 0x80; }
      else if ((lose_point == 3) && ((cnt == 2)|(cnt == 3))){ dot_all[arrow] |= 0x02; } 
      else { dot_all[arrow] = 0x01; }        

      chance--; // reduced chance
      cnt ++;   // counting shooting number 
      final_display(50); // displaying arrow points
      lose_point = 0; // initialize failure count
    }

    // flex bent condition: ADC value is "120 ~ 240" (check with UART)
    else if ((flex_val > 100) && (flex_val < 300)){ lose_point++; } 

    if (chance == 0){ break; }

  } //first while end

  while(1){ final_display(100); } // display final results
  /*

  sei();
  timer_init();

  while(1){
    if (t_count == 100){
      count_down(t_count);
      dotmatrix_init(t_count);
      break;
    }
  }*/ // timer doesnt work
}


/* 
   Functions
*/
void maintain_disp(void)
{
  // make delays for maintain
  for (int i = 0; i < 64; i++){
    _delay_loop_2(0);
  }
}

void port_init(void)
{
  // all port initialize
  DDRA = 0xFF;   // dotmatrix VCC 
  DDRC = 0xFF;   // dotmatrix GND
  PORTA = 0xFF;  // dotmatrix VCC
  PORTC = 0xFF;  // dotmatrix GND
  DDRB = 0x00;   // LEDs PB0: GREEN, PB1: RED
  PORTB = 0x00;  // LEDs PB0: GREEN, PB1: RED
  DDRF = 0x00;   // ADC: flex PF0
}

void led_off(void)
{
  // All LEDs off
  DDRB = 0x00;
  PORTB = 0x00;
  _delay_ms(100);
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

void final_display(int disp_j) // PORTA, PORTC
{  
  // display final results
  int i,j;
  for (j = 0; j < disp_j; j++){
    for (i = 0; i < 8; i++){
      // PORTA = dot_all[i]; 
      PORTA = ~dot_all[i]; // reverse: 0 to 1
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

void target_display(int disp_j)// PORTA, PORTC
{  
  // display "target"
  int i,j;
  for (j = 0; j < disp_j; j++){
      for (i = 0; i < 8; i++){
        PORTA = target_int[i];
        PORTC = dot_int_GND[i];
        _delay_ms(2); 
      }   
  }
  PORTA = 0xFF;
}

void count_display(int cnt, int disp_j)
{
   // display count number(cnt = 0,1,2,3)
   int i,j;
   for (j = 0; j < disp_j; j++){
      for (i = 0; i < 8; i++){
         PORTA = count_disp[cnt][i];
         PORTC = dot_int_GND[i];
         _delay_ms(2); 
      }
  }
  PORTA = 0xFF;
}

void count_down(int disp_j)
{
  // count down 3, 2, 1
  int i = 3;
  while(i != 0){
    count_display(i-1, disp_j);
    i--;  
  }
}

void arrow_display(void)
{
   // display arrow animation
  int i,j,k,kk;
  for (k = 0; k < 7; k++){
      for (i = 0; i < 8; i++){
        for(kk = 0; kk < 10; kk++){
            for (j = 0; j < 8; j++){
              PORTA = arrow_disp[i][j];
              PORTC = dot_int_GND[j];
              _delay_ms(1);
            }
        }
      }
  }
  PORTA = 0xFF;
}

void ADC_init(void)
{ 
  ADMUX=0x00; 
  ADCSRA=0x87; // 0b10000111: ADEN = 1(ADC Enable), Prescaler = 128, 16MHz/128 = 125kHz  
}

unsigned int read_ADC(int channel) // PF0 channel = 0
{
  unsigned int result = 0; // ADC value initialize
  ADC_init(); // ADC initialize

  ADMUX = channel; // PF0: flex sensor
  ADMUX |= (0<<REFS1 | 1<<REFS0); // AVCC: 4.6V check with DMM
  ADCSRA |= (1<<ADSC); // ADC start conversion(single conversion)

  result += (int)ADCL + ((int)ADCH<<8); // ADC Low byte, High byte

  return result;
}
