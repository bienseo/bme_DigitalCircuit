// example : UART printf test
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

static int Putchar(char c, FILE *stream);
void tx0Char(char message);
void tx1Char(char message);

void port_init(void);
void uart0_init(void);
void uart1_init(void);
void init_devices(void);

static int Putchar(char c, FILE *stream)
{
	tx0Char(c);
	return 0;
      
}

// UART0 을 이용한 출력
void tx0Char(char message)
{
	while (((UCSR0A>>UDRE0)&0x01) == 0) ;  // UDRE, data register empty        
    UDR0 = message;
}

void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0x00;
 PORTB = 0x00;
 DDRB  = 0x00;
 PORTC = 0x00;
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0x00;
 PORTE = 0x00;
 DDRE  = 0x00;
 PORTF = 0x00;
 DDRF  = 0x00;
 PORTG = 0x00;
 DDRG  = 0x00;
}

// UART0 initialize
// baud rate: 9600
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x67; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x18;
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 cli(); //disable all interrupts
 XMCRA = 0x00; //external memory
 XMCRB = 0x00; //external memory
 port_init();
 uart0_init();              // UART 0 초기화
 fdevopen(Putchar,0);
 
 MCUCR = 0x00;
 EICRA = 0x00; //extended ext ints
 EICRB = 0x00; //extended ext ints
 EIMSK = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 ETIMSK = 0x00; //extended timer interrupt sources
 sei(); //re-enable interrupts
 //all peripherals are now initialized
}

// 시간 지연 함수
void delay_us(int time_us)
{
   register int i;
   for(i=0; i<time_us; i++){   // 4 cycle +
      asm("PUSH   R0");        // 2 cycle +
      asm("POP    R0");        // 2 cycle +
      asm("PUSH   R0");        // 2 cycle +
      asm("POP    R0");        // 2 cycle +
   }
}

void delay_ms(int time_ms)
{
   register int i;
   for(i=0;i<time_ms;i++) delay_us(1000);
}

int main (void)
{
/*	init_devices();
	//insert your functional code here...
	while(1){
		printf("Last Class");
		delay_ms(1000);
	}*/

	int i=0;
	init_devices();
	//insert your functional code here...
	printf("\n\r\ntesting...\n\r");
	printf("printf Test Program...\n\r");
	while(1){
		printf("Test i : %d\n\r",i);
		i++;
		delay_ms(1000);
	}
}
