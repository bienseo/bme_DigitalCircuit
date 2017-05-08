// Digital circuit Project 
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with atmega128

#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h>

# define HIGH 1
# define LOW 0
unsigned int dot_int[8] = { 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42 }; // dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }; // dotmatrix cathode(GND)

// delay
void delay(int n)
{
    volatile int i,j;
    for(i=1; i<n; i++)
    {
        for(j=1; j<900; j++);

    }
}

// port initialize
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

// external interrupt 
ISR(INT1_vect)
{
    for(int kk=0; kk<3; kk++)
    {
        PORTG = 0x01;
        delay(500);
        PORTG = 0x00;
        delay(500);
    }
}

// dotmatrix initialize: display "H"
void dotmatrix_int(void)
{
        int i;

        for(i = 0; i < 8; i++){
            PORTE = dot_int[i]; // "H" for Hanzo
            PORTA = dot_int_GND[i];
            delay_ms(2); 
        }
    
}

// LED
void green_led(void){ PORTC = 0x80; } // start 
void red_led(void){ PORTC = 0x40; } // stop and pause

// on going
void display(unsigned int num); //using switch and case -> make sate and flow
void get_flex(void) //unsigned int v_flex
{
    PORTC = 0x01;
    //using switch and case -> make sate and flow

}
void get_force(void) //unsigned int v_force 
{
    PORTC = 0x02;
    //using switch and case -> make sate and flow
}

// main script
int main(void)
{
    port_init();
    dotmatrix_int();
    EIMSK =0x02;
    EICRA = 0xAA;
    SREG = 0x80;



}

/*
// on going temp
void display(unsigned int num)
{
    int i,n
    for(n = 0; n < num; n++){
        for(i = 0; i < 8 i ++){

        	GND = check[i]
        	display_port2 = buf[i]
        	delay_ms(1)


        }
    }

}

void LRshift(int n, int time){
	volatile int w,v,l,i;
	unsigned char buf[8]

	for(v = 7; v > = 1; v--){
		for
	}
}

*/