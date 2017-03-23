#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>

void delay(int n);

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

void delay(int n)
{
	volatile int i,j;
	for(i=1; i<n; i++)
	{
		for(j=1; j<900; j++);

	}
}

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

int main(void)
{
	port_init();
	EIMSK =0x02;
	EICRA = 0xAA;
	SREG = 0x80;

	while(1)
	{
		if(PING == 0x04)
		{
			for(int k=0; k<5; k++)
			{
				PORTG = 0x01;
				delay(500);
				PORTG = 0x02;
				delay(500);
				PORTG = 0x00;
				delay(500);
			}
		  PORTG = 0x00;
		  delay(500);
  
		}
	}
}
