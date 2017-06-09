
/* 
 * atmega128-adc-example.c 
 * 
 * Created: 2015-12-15 오후 1:37:05 
 * Author : webnautes 
 */   
  
#define F_CPU 16000000UL  
  
//1. baud rate를 선택  
#define USART_BAUDRATE 9600  
  
//2.시스템 클록과 원하는 baud rate를 이용하여 UBRR 값을 계산한다.  
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)  
  
#include <avr/io.h>  
#include <util/delay.h>  
#include <stdio.h>  
  
  
void usartInit()  
{  
    //3. UBRR0은 16비트 레지스터이기 때문에 8비트씩 나누어서 넣어야 한다.  
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);  
    UBRR0L = (uint8_t) UBRR_VALUE;  
      
    //4. USART 설정  
    UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); //Charecter size : 8비트  
    UCSR0C &= ~(1<<USBS0); //stop  bit : 1비트  
    UCSR0C &= ~((1<<UPM01)|(1<<UPM00)); // no parity mode  
      
    //5. 송수신을 가능하게 한다.  
    UCSR0B=(1<<RXEN0)|(1<<TXEN0);  
}  
  
  
void uart_putchar(uint8_t u8Data, FILE *stream )  
{  
    //이전 전송이 끝나기를 기다림  
    while(!(UCSR0A&(1<<UDRE0))){};  
      
    UDR0 = u8Data;  
}  
  
uint8_t uart_getchar( FILE *stream)  
{  
    // 수신 되기를 기다림  
    while(!(UCSR0A&(1<<RXC0))){};  
      
    return UDR0;  
}  
  
//http://www.appelsiini.net/2011/simple-usart-with-avr-libc  
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);  
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);  
  
  
  
  
int main(void)  
{  
    //ADC init  
    ADMUX |= (0<<REFS1 | 1<<REFS0); //기준전압으로 AVCC 사용  
      
    //ADC 입력채널 선택  ADC0(PF0)  
    ADMUX &= ~( (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0) );    
          
          
    ADCSRA |= (1<<ADEN); //ADC 기능 활성화  
      
    // 16Mhz/128 = 125Khz, ADC의 입력 클록 주파수   
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));     
      
          
          
    usartInit();  
    stdout = &uart_output;  
    stdin  = &uart_input;  
  
    while (1)   
    {         
        ADCSRA |= (1<<ADSC); //ADC 변환 시작  
          
        //ADC 변환이 완료되면 자동으로 ADSC는 0이 되기 때문에 이렇게 대기한다.  
        while(ADCSRA & (1<<ADSC));   
              
        unsigned char low  = ADCL;  
        unsigned char high = ADCH;  
        int temperature = (high << 8) | low;    
          
        float result = temperature * 5.0/1024*100;  
          
        char str[256];  
        sprintf( str, "temperature %4.2f\r\n", result );  
        printf( str );  
        _delay_ms(100);       
          
    }  
}  