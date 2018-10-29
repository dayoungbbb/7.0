/*
 * LED.c
 *
 * Created: 2018-05-16 오전 10:08:29
 * Author : edu
 */ 
#if 0
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void INIT_PORT(void)
{
	DDRB = 0x20;
	PORTB = 0x00;
	
	DDRD = 0x00;
	PORTD = 0x04;
	
}

int main(void)
{
	INIT_PORT();
    /* Replace with your application code */
    while (1) 
    {
		if(PIND & 0x04)	PORTB = 0x00;
		else PORTB = 0x20;
    }
}

#endif

//코드 11.4
#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

void INIT_PORT(void)
{
	DDRB = 0x20;
	PORTB = 0x00;
	
	DDRD = 0x00;
	PORTD = 0x04;
	
}

int main(void)
{
	UART_INIT();
	INIT_PORT();
	int state = 0;
	
	/* Replace with your application code */
	while (1)
	{
		if(PIND & 0x04){ //버튼이 눌리지 않은 상태
				PORTB = 0x00;
				//state = 0;
		}
		else { //버튼이 눌린 상태
			//if(state == 0) UART_transmit('*'); //UART-모니터
			UART_transmit('*');
			//state = 1;
			PORTB = 0x20;
		}
	}
	
	return 0;
}
#endif

#if 1
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

void INIT_PORT(void)
{
	DDRB = 0x20;
	PORTB = 0x00;
	
	DDRD = 0x00;
	PORTD = 0x04;
	
}
#define loop_until_bit_is_set(sfr, bit) do{}while(bit_is_clear(sfr, bit)) //스핀락
#define loop_until_bit_is_clear(sfr, bit) do{}while(bit_is_set(sfr, bit))
int main(void)
{
	UART_INIT();
	INIT_PORT();
	int state = 0;
	
	/* Replace with your application code */
	while (1)
	{
		loop_until_bit_is_set(PIND, 2); //버튼이 눌리지 않은 상태
			PORTB = 0x00;
			//state = 0;
			_delay_ms(100); 
		loop_until_bit_is_clear(PIND, 2); //버튼이 눌린 상태
			//if(state == 0) UART_transmit('*'); //UART-모니터
			UART_transmit('*');
			//state = 1;
			PORTB = 0x20;
			_delay_ms(100);
		
	}
	
	return 0;
}
#endif