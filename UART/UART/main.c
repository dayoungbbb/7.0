/*
 * UART.c
 *
 * Created: 2018-05-15 오전 10:12:47
 * Author : edu
 */ 
#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void UART_INIT(void) {
	UCSR0A |= _BV(U2X0);
	
	UBRR0H = 0x00;
	UBRR0L = 207;
	
	// 비동기
	UCSR0C |= 0X06;
	
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)
{
	
	while( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;	
}
int main(int argc, char *argv[])
{
	unsigned char data;
	
	UART_INIT();
    /* Replace with your application code */
    while (1) 
    {
		data = UART_receive();
		UART_transmit(data);
    }
	return 0;
}
#endif

#if 0
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

int main(int argc, char *argv[])
{
	unsigned char data;
	
	UART_INIT();
	while(1) {
		data = UART_receive();
		UART_transmit(data);
	}
	
	return 0;
}

#endif

#if 0
#define F_CPU 16000000L
#define TERMINATOR '$'

#include <string.h>
#include "UART.h"

int main()
{
	int counter = 100;
	int index = 0;
	int process_data = 0;
	char buffer[20] = "";
	char data;
	
	UART_INIT();
	
	UART_printString("Current Counter Value: ");	
	UART_print16bitNumber(counter);
	UART_printString("\n");
	
	while(1){
		data = UART_receive();
		if(data == 0)
			continue;
		if(data == TERMINATOR){
			buffer[index] = '\0';
			process_data = 1;
		}else {
			buffer[index] = data;
			index++;
		}
		
		if(process_data == 1){
			if(strcmp(buffer, "DOWN") == 0){
				counter--;
				UART_printString("Current Counter Value : ");
				UART_print16bitNumber(counter);
				UART_printString("\n");
			}
			else if(strcmp(buffer, "UP") == 0){
				counter++;
				UART_printString("Current Counter Value : ");
				UART_print16bitNumber(counter);
				UART_printString("\n");
				
			}
			else {
				UART_printString("** Unknown Command **");
				UART_printString("\n");
			}
			index = 0;
			process_data = 0;
			
		}
	}
}
#endif

#if 1
#include <stdio.h>
#include "UART.h"
#include <avr/io.h>

FILE OUTPUT = FDEV_SETUP_STREAM(UART_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART_receive, _FDEV_SETUP_READ);

int main(int argc, char *argv[])
{
	unsigned char data;
	
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART_INIT();
	while(1){
		scanf("%c", &data);
		printf(" typed : %c", data);
	}
	return 0;
}
#endif