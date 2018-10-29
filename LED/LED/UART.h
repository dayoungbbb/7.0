/*
 * UART.h
 *
 * Created: 2018-05-15 오후 2:19:22
 *  Author: edu
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void UART_INIT(void);
unsigned char UART_receive(void);
void UART_transmit(unsigned char data);
void UART_printString(char *str);
void UART_print8bitNumber(uint8_t no);
void UART_print16bitNumber(uint16_t no);
void UART_print32bitNumber(uint32_t no);

#endif /* UART_H_ */