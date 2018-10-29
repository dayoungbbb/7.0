/*
 * ADC.c
 *
 * Created: 2018-05-16 오후 3:33:18
 * Author : edu
 */ 

#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"

void ADC_INIT(unsigned char channel)
{
	ADMUX |= 0x40;
	
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= _BV(ADATE);
	
	ADMUX = ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
	
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));
	
	return ADC;
	
}

void int_to_string(int n, char * buffer)
{
	sprintf(buffer, "%04d", n);
	buffer[4] = '\0';
	
}

int main(void)
{
    /* Replace with your application code */
	int read;
	char buffer[5];
	
	UART_INIT();
	ADC_INIT(0);
    while (1) 
    {
		read = read_ADC();
		int_to_string(read,buffer);
		UART_printString(buffer);
		UART_printString("\n");
		
		_delay_ms(1000);
    }
	
	return 0;
}

#endif

#if 1
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
void ADC_INIT(unsigned char channel)
{
	ADMUX |= 0x40;
	
	ADCSRA |= 0x07;
	ADCSRA |= (1 << ADEN);
	ADCSRA |= _BV(ADATE);
	
	ADMUX = ((ADMUX & 0xE0) | channel);
	ADCSRA |= (1 << ADSC);
	
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));
	
	return ADC;
	
}

void int_to_string(int n, char * buffer)
{
	sprintf(buffer, "%04d", n);
	buffer[4] = '\0';
	
}

void PORT_INIT(void)
{
	DDRD = 0xff;
	PORTD = 0x00;
}
int main(void)
{
	/* Replace with your application code */
	int read;
	char buffer[5];
	uint8_t on_off;
	int value;
	
	UART_INIT();
	ADC_INIT(0);
	PORT_INIT();
	while (1)
	{
		read = read_ADC();
		int_to_string(read,buffer);
		UART_printString(buffer);
		UART_printString("\n");
		
		_delay_ms(1000);
		value = read >> 7;
		on_off = 0;
		for(int i = 0; i<=value; i++)
			on_off |= (0x01 << i)
			
		PORTD = on_off;
	}
	
	return 0;
}

#endif