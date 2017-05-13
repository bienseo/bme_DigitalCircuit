// Digital circuit Project 
// Team 2: Jihoon Shin, Eunseo Cho
// Project name: Hanzo game with atmega128

#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#include<util/delay.h>

# define HIGH 1
# define LOW 0
unsigned int dot_int[8] = { 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42 }; // "H" dotmatrix anode(VCC) 
unsigned int dot_int_GND[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 }; 2
unsigned int target_int[8] = { 0xff, 0x81, 0x81, 0x99, 0x99, 0x81, 0x81,0xff }; // target


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
    while(1){
        for(i = 0; i < 8; i++){
            PORTE = dot_int[i]; // "H" for Hanzo
            PORTA = dot_int_GND[i];
            delay_ms(2); 
        }
    }    
        delay(500);

    while(1){  
        for(i = 0; i < 8; i++){
            PORTE = target_int[i]; // "H" for Hanzo
            PORTA = dot_int_GND[i];
            delay_ms(2); 
        }
    }
}

// LED
void green_led(void){ PORTC = 0x80; } // start 
void red_led(void){ PORTC = 0x40; } // stop and pause

// on going
void display(unsigned int num); //using switch and case -> make sate and flow
void get_flex(unsigned int v_flex) // sign: starting the game
{
    PORTC = 0x01;
    //using switch and case -> make sate and flow

}
void get_force(unsigned int v_force) // get the value of strength
{
    PORTC = 0x02;
    //using switch and case -> make sate and flow
}
void is_change(unsigned int direction){
    return direction;
} // the flex sensor on the wrist: get direction


void ADC_int(unsigned char channel){
    ADMUX |= (1 << REFS0);              // AVCC를 기준 전압으로 선택
    ADCSRA |= 0x07; //분주비 설정
    ADCSRA |= (1 << ADEN); // ADC 활성화
    ADCSRA |= (1 << ADFR); // 프리 러닝 모드 

    ADMUX = ((ADMUX & 0xe0) | channel); // 채널 선택
    ADCSRA |= (1<<ADSC); // 변환 시작

}

int read_ADC(void){
    while(!(ADCSRA & (1 << ADIF))); //변환 종료 대기

    return ADC; // 10 비트 값을 반환
}

// main script
int main(void)
{
    unsigned int v_flex, v_force, direction;
    int i,j; // iteration term
    int read_val;

    port_init(); // All port initialize
    dotmatrix_int(); // Dotmatrix initialize: display "H"
    ADC_int(); // ADC initialize
    EIMSK =0x02;
    EICRA = 0xAA;
    SREG = 0x80;
    while(1){
        read_val = read_ADC(); // 가변 저항값 읽기
        printf("%d\n",read_val);
        _dealy_ms(1000);
    }

    red_led(); // stop sign
    if (v_flex > 0){
        get_flex(v_flex);
        get_force(v_force);
        change_dir = is_change(direction);
        if (){

        }


    }



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