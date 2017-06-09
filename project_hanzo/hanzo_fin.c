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
volatile int chance = 4; 			             // Chance for shooting	  
volatile unsigned char adcVectL, adcVectH;    // ADC Low byte, High byte
volatile unsigned int adc_val = 0;
volatile int t_count; // timer count variable

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
unsigned int dot_all[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // display all

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
void display_all(int adc_column);

// ADC functions 
void ADC_init(void);           // ADC port initialize
unsigned int read_ADC(unsigned int adc_port);	      // read ADC value 
unsigned int get_force(int c_num);    // PF0: get force sensor voltage
unsigned int get_flex(int c_num);      // PF1: get flex sensor voltage

// on going
void is_change(unsigned int direction);  // get direction from the switch or whatever
void make_wind(void); 				        // obstacle wind
void fin_display(void);
/*
void timer_init(void) // Timer initialize
{
   TCCR0=0x07; // 1024 from prescaler
   TIMSK=0x02; // enable overflow
   ACSR=0x80;
   TCNT0=0x64; // TCNT0 = 100
   OCR0=0x00;
   ASSR=0x00;
   ETIMSK=0x00;
   SFIOR=0x00;
}

void timer_delay(void)
{
   t_count = 0;
   while(t_count <= 30){}
}

ISR(TIMER0_OVF_vect) // if: TCNT0 overflow jump to here
{
   if(t_count > 30){ t_count = 0; }
   else{ t_count++; }
   TCNT0 = 0x64; // TCNT0 = 100
}*/

/*
ISR(INT0_vect) // PD0(INT0)
{
   arrow_display();
   get_flex(chance);
   red_led();
   green_led();
   red_led();
   green_led();
SREG = 0x80; // Global interrupt enable
   EIMSK =0x01; // External interrupt mask: INT0
   //EICRA = 0xAA; // trigger low edge 
   }
*/
ISR(ADC_vect)
{

}


/*
   Main script
*/ 
int main(void)
{
   int cnt = 3; // counting number
   int c_num = chance; // chance number
   unsigned int flex_test,flex_comp;
   // int find_col;
   //SREG = 0x80; // Global interrupt enable
   //EIMSK =0x01; // External interrupt mask: INT0
   //EICRA = 0xAA; // trigger low edge 
   port_init();
   dotmatrix_init();
   red_led();

   while(c_num != 0){
      green_led();
      target_display();

      arrow_display();
      flex_comp = adc_val;
      flex_test = get_flex(chance);

      red_led();
      green_led();
      red_led();
      green_led();

      if (flex_comp != flex_test){

         if (cnt < 0){ cnt = 3; }
         //count_display(cnt);
         cnt--;
         //final_disp[4-chance][0] = get_force(c_num);
         //final_disp[4-chance][1] = get_flex(c_num);
         c_num--;
         //flex_test = adc_val;
         if ((flex_test > 200)&&(flex_test < 400)){ // resting:256 ...12.xx?
            dotmatrix_init();
            count_display(0);
         }
         else if ((flex_test > 70)&&(flex_test < 150)){ // bent:113
            dotmatrix_init();
            count_display(1);
         }
         else{
            count_display(cnt);
         }
      }
      else{ count_display(4); }
   }

   /* other solution for case distribution 3 case (0, 45, 90)
   while(1){
      flex_test = get_flex(chance);
      find_col = flex_test / 100;
      display_all(find_col)  
   }
   */



/*
   cli();
   timer_init(); // timer initialize
   sei();

   while(1){
      if (cnt < 0){
         cnt = 3;
      }
      count_display(cnt);
      timer_delay();
      cnt--;
   }*/

   return 0;
}

/* 
   functions
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

   DDRF = 0xFE;   // ADC: flex PF0, force PF1
   DDRD = 0x00;   // External Interrupt: Switch PD0,
  // PORTF = 0x00;  // only ADC: flex PF0,..later force PF1
  // PORTE = 0x00;
  // DDRE = 0x00;
  // PORTD = 0xFF;  // External Interrupt: Switch PD0, PD1

   DDRG = 0x00;   // ? Switch: OFF = 0, ON = 1
  // PORTG = 0x00;  // ?PG2 = 0x04, PG3 = 0x08
 
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

void display_all(int adc_column) // PORTA, PORTC
{  
   dot_all[adc_column] = 0xFF;

   // display all for find adc
   int i,j;
   for (j = 0; j < 30; j++){
      for (i = 0; i < 8; i++){
         PORTA = dot_all[i]; 
         PORTC = dot_int_GND[i];
         _delay_ms(2);
      }
   }
   dot_all[adc_column] = 0x00;
   PORTA = 0x00;
}


void dotmatrix_init(void) // PORTA, PORTC
{  
   // display "H" for Hanzo
   int i,j;
   for (j = 0; j < 30; j++){
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

   for (j = 0; j < 30; j++){
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
   for (j = 0; j < 30; j++){
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
   int i,j,k,kk;
   for (k = 0; k < 15; k++){
      for (i = 0; i < 8; i++){
         for(kk = 0; kk < 15; kk++){
             for (j = 0; j < 8; j++){
               PORTA = arrow_disp[i][j];
               PORTC = dot_int_GND[j];
               _delay_ms(0.5);
            }
         }
      }
   }
   PORTA = 0x00;
}

void ADC_init(void)
{ 
   ADMUX=0x00;  // AVCC = 2.56V 1<<REFS0 //기준전압으로 AVCC 사용  
   ADCSRA=0x87; // 0b10000111: ADEN = 1(ADC Enable), Prescaler = 128, 16Mhz/128 = 125Khz  
}

unsigned int read_ADC(unsigned int adc_port)
{
   unsigned int result;
   //unsigned int result;
   ADC_init();

   ADMUX = 0x;
   ADMUX |= (0<<REFS1 | 1<<REFS0);
  
   ADCSRA |= (1<<ADSC); // ADC start conversion(single conversion)
   //ADCSRA |= 0x40; // ADC start conversion(single conversion)
 
   while(ADCSRA && (1<<ADSC)){}
   adcVectL = ADCL;
   adcVectH = ADCH;
   adc_val = (adcVectH << 8) | adcVectL;
   //adc_val = (int)adcVectL + (int)(adcVectH << 8);
   // result = adc_val * 5.0/1024*100;
   result = adc_val;

   return result;
}

unsigned int get_force(int c_num)
{  
   unsigned int force_port = 0x41; // AVCC, PF0: force sensor
   unsigned int v_force;

   if (c_num == 0){ return chance = 0; }
   else{
      v_force = read_ADC(force_port); // force sensor voltage
      return v_force;  
   }
}

unsigned int get_flex(int c_num)
{
   unsigned int flex_port = 0x42; // AVCC, PF1: flex sensor
   unsigned int v_flex;

   if (c_num == 0){ return chance = 0; }
   else{
      v_flex = read_ADC(flex_port); // flex sensor voltage
      return v_flex;
   }
}

