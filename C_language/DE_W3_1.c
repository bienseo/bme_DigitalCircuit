#include<avr/io.h>
#include<stdio.h>
#include<util/delay.h>


void port_init(void)
{
	
	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;
	PORTF = 0x00;
	PORTG = 0x00;
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	DDRE = 0x00;
	DDRF = 0x00;
	DDRG = 0x03;

}

void main(void)
{
	port_init();
	while(1)
	{
		if(PING == 0x04){
			PORTG = 0x01;
			_delay_ms(100);
		}
		else if(PING == 0x08){
			PORTG = 0x02;
			_delay_ms(100);
		}
		_delay_ms(100);
		PORTG = 0x00;

	}

}