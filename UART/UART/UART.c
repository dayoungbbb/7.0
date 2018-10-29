﻿/*
 * UART.c
 *
 * Created: 2018-05-15 오후 2:19:52
 *  Author: edu
 */ 
#include "UART.h"

void UART_INIT(void) {
	UCSR0A |= _BV(U2X0);
	
	UBRR0H = 0x00;
	UBRR0L = 207;
	
	// 비동기
	UCSR0C |= 0X06;
	
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}
/*
unsigned char UART_receive(void)
{
	static char buffer[80];
	static int re = 0, wr = 0;
	
	if(wr == 0)
	{
		while( !(UCSR0A & (1<<RXC0)));
		while((UCSR0A & (1<< RXC0)))
		{
			buffer[wr++] = UDR0;
		}
	}
	if(re != wr){
		return buffer[re++];
	}
	re = wr = 0;
	return 0;
}
*/

unsigned char UART_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

void UART_printString(char *str)
{
	for(int i = 0; str[i]; i++)
	UART_transmit(str[i]);
}
void UART_print8bitNumber(uint8_t no)
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(no > 0){
		for(i = 0; no != 0; i++)
		{
			
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] = '\0';
		index = i-1;
	}
	
	for(i = index; i>=0; i--)
	UART_transmit(numString[i]);
}
void UART_print16bitNumber(uint16_t no)
{
	char numString[6] = "0";
	int i, index = 0;
	
	if(no > 0){
		for(i = 0; no != 0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
			
		}
		numString[i] = '\0';
		index = i-1;
	}
	for(i = index; i >= 0; i--)
		UART_transmit(numString[i]);
}
void UART_print32bitNumber(uint32_t no)
{
	char numString[11] = "0";
	int i, index = 0;
	
	if(no > 0){
		for(i = 0; no != 0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
			
		}
		numString[i] = '\0';
		index = i - 1;
	}
	
	for(i = index; i>= 0; i--)
		UART_transmit(numString[i]);
}