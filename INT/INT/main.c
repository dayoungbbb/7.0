/*
 * INT.c
 *
 * Created: 2018-05-17 오전 9:45:21
 * Author : edu
 */ 
#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
	if(PIND & 0x04) PORTB = 0x00;
	else PORTB = 0x20;
}

void INIT_PORT(void)
{
	DDRB = 0x20;
	PORTB = 0x00;
	
	DDRD = 0x00;
	PORTD = 0x04;
	
}

void INIT_INT0(void)
{
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC00);
	sei();
	
}
int main(void)
{
	INIT_PORT();
	INIT_INT0();
    /* Replace with your application code */
    while (1) 
    {
		
    }
}
#endif

#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	uint8_t numbers[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	int count = 0;
	DDRD = 0xFF;
		
	while(1)
	{
		PORTD = numbers[count];
		count = (count + 1) %10;
			
		_delay_ms(1000);
			
		}
		
		return 1;
}

#endif


#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int interrupt_count = 0;

ISR(TIMER0_OVF_vect)
{
	interrupt_count++;
}

int main(void)
{
	
	uint8_t numbers[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	int count = 0;
	DDRD = 0xFF;
	PORTD = numbers[0];
	
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TIMSK0 |= (1 << TOIE0);
	sei();
	
	while(1)
	{
		if(interrupt_count == 61){
			interrupt_count = 0;
			
			count = (count + 1) %10;
			PORTD = numbers[count];
		}
	}
	return 1;
}

#endif

#if 1
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int cnt = 0;
int sec1 = 0;
int sec2 = 0;
int sec3 = 0;

ISR(TIMER0_OVF_vect)
{
	cnt++;
	if(cnt == 32){
		cnt = 0;
		if(sec1 == 9){
			sec1 = 0;
			sec2++;
		}
		else
			sec1++;
		if(sec2 == 6){
			sec2 = 0;
			sec3++;
		}
		if(sec3 == 10)
			sec3 = 0;
			
	}
}

int main(void)
{
	
	uint8_t numbers[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};

	DDRD = 0xFF;
	DDRB = 0x07;

	
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TIMSK0 |= (1 << TOIE0);
	sei();
	
	while(1)
	{
		PORTB = 0x03; //011	
		PORTD = numbers[sec1];
		_delay_ms(1);
		PORTB = 0x05; //101
		PORTD = numbers[sec2];
		_delay_ms(1);
		
		PORTB = 0x06; //110
		PORTD = numbers[sec3];
		_delay_ms(1);
	}
	return 1;
	
}

#endif