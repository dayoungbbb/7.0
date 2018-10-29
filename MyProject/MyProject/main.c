/*
 * project.c
 *
 * Created: 2018-05-24 오전 9:21:08
 * Author : edu
 */ 
#if 0
//13번 LED
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRB = 0x20;
    while (1) 
    {
		PORTB = 0x00;
		_delay_ms(1000);
		PORTB = 0x20;
		_delay_ms(1000);
    }
}


#endif

#if 0
//FND
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void Init_74595(void)
{
	DDRB |= 0b00011100;
}
void ShiftClock(void)
{
	PORTB |= 0b00000100; // 10번만 high
	_delay_us(1);
	PORTB &= 0b11111011; // 10번만 low로
}
void LatchClcok(void)
{
	PORTB |= 0b00010000;
	_delay_us(1);
	PORTB &= 0b11101111;
}

void ByteDataWrite(uint8_t data)
{
	for(uint8_t i = 0; i<8; i++){
		if(data & 0b10000000){ // data 8비트에서 최상위 비트 1비트만 내보내기 위해서 // 최상위 비트가 1이니깐
			PORTB |= 0b00001000; // 우리는 1비트만 쓰지만 portB는 8비트를 쓰므로 11번만 1일 떄 1로 보내고
		}
		else { // 최상위 비트가 0일 떄
			PORTB &= 0b11110111; //11번을 out으로 // 0일 때 0으로 보내는 거
		}
		ShiftClock(); // 10번이 low에서 high low로
		data = data << 1;
	}
	LatchClcok();
}

void main()
{
	uint8_t numbers[] = 
	{0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	
	int count = 0;
	Init_74595();
	while(1)
	{
		ByteDataWrite(numbers[count]);
		count = (count+1)%10;
		_delay_ms(1000);
	}
}
#endif

#if 0
//LCD 4 bit
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define RS_PIN 2
#define E_PIN 3

#define COMMAND_CLEAR_DISPLAY 0x01
#define COMMAND_4_BIT_MODE 0x28

#define COMMAND_DISPLAY_ON_OFF_BIT 2
#define COMMAND_CURSOR_ON_OFF_BIT 1
#define COMMAND_BLINK_ON_OFF_BIT 0

void enable(void);
void write_data(uint8_t data);
void write_command(uint8_t command);
void clear(void);
void init(void);
void write_string(char *string);
void got_XY(uint8_t row, uint8_t col);

void enable()
{
	PORTD |= (1 << E_PIN);
	_delay_us(1);
	PORTD &= ~(1<<E_PIN);
	_delay_ms(1);
	
}

void write_data(uint8_t data)
{
	uint8_t temp;
	PORTD |= (1<<RS_PIN);
	temp = PORTD & 0x0F;
	PORTD = temp | (data & 0xF0);
	enable();
	
	temp = PORTD & 0x0F;
	PORTD = temp | ((data << 4) & 0xF0);
	enable();
}

void write_command(uint8_t command)
{
	uint8_t temp;
	PORTD &= ~(1<<RS_PIN); //RS를 0으로 떨어뜨려야 command(명령어)
	temp = PORTD & 0x0F; //port의 0~3번까지를 살리고
	PORTD = temp | (command & 0xF0); // DATA D4~D7를 | 해서
	enable();
	
	temp = PORTD & 0x0F; //port의 0~3번까지를 살리고
	PORTD = temp | ( (command << 4) & 0xF0); //0~3번까지 
	enable();
}

void clear()
{
	write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(1);
}

void init()
{
	_delay_ms(10);
	DDRD |= 0xFC;
	
	write_command(0x02);
	write_command(COMMAND_4_BIT_MODE);
	uint8_t command = 0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT);
	write_command(command);
	clear();
	write_command(0x06);
}

void write_string(char *string)
{
	uint8_t i;
	for(i = 0; string[i]; i++)
	write_data(string[i]);
	
}

void got_XY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	write_command(command);
	
}
int main()
{
	init();
	
	write_string("hello world!");
	_delay_ms(1000);
	got_XY(1, 0);
	write_string("lcd test");
	while(1);
	
}
#endif

#if 0
//UART
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);
	UBRR0H = 0x00;
	UBRR0L = 207;
	UCSR0C |= 0x06;
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}
unsigned char buffer[80] = {0,};
/*	
unsigned char UART_receive(void){
	while( !(UCSR0A & (1 << RXC0)));
	return UDR0;
}
*/
unsigned char UART_receive(void)
{
	unsigned char wr = 0;
	while(!(UCSR0A & (1<<RXC0)));
	while((UCSR0A & (1 <<RXC0)))
		buffer[wr++] = UDR0;
	buffer[wr] = '\0';
	
	return wr;
}
/*
void UART_transmit(unsigned char data){
	while( !(UCSR0A & ( 1<< UDRE0)));
	UDR0 = data;
}
*/
void UART_transmit(unsigned char data){
	unsigned char i;
	
	if(data == 0)
		return;
	for(i=0; buffer[i];i++)
	{
		while( !(UCSR0A & ( 1<< UDRE0)));
		UDR0 = buffer[i];
	}
		
}
int main()
{
	unsigned char data;
	UART_INIT();
	while(1){
		data = UART_receive();
		_delay_ms(20);		
		UART_transmit(data);
	}
	return 0;
}
#endif

#if 0
//UART
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);
	UBRR0H = 0x00;
	UBRR0L = 207;
	UCSR0C |= 0x06;
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
	UCSR0B |= _BV(RXCIE0);
}
unsigned char buffer[80] = {0,};
unsigned char wr = 0;
ISR(USART_RX_vect)
{
	while((UCSR0A & (1<<RXC0)))
		buffer[wr++] = UDR0;
	buffer[wr] = '\0';
}
/*
unsigned char UART_receive(void)
{
	unsigned char wr = 0;
	while(!(UCSR0A & (1<<RXC0)));
	while((UCSR0A & (1 <<RXC0)))
		buffer[wr++] = UDR0;
	buffer[wr] = '\0';
	
	return wr;
}
*/
/*

void UART_transmit(unsigned char data){
	unsigned char i;
	
	if(data == 0)
		return;
	for(i=0; buffer[i];i++)
	{
		while( !(UCSR0A & ( 1<< UDRE0)));
		UDR0 = buffer[i];
	}
		
}
*/
void UART_transmit()
{
	if(!wr)
		return;
	for(unsigned char i = 0; buffer[i]; i++){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = buffer[i];
	}
	wr = 0;
}

int main()
{
	unsigned char data;
	UART_INIT();
	sei();
	while(1){
		//data = UART_receive();
		_delay_ms(100);		
		UART_transmit();
	}
	return 0;
}
#endif


#if 0
//UART & LCD
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//--------------------------------------------------

#define RS_PIN		2
#define E_PIN		3

#define COMMAND_CLEAR_DISPLAY	0x01
#define COMMAND_4_BIT_MODE		0x28

#define COMMAND_DISPLAY_ON_OFF_BIT	2
#define COMMAND_CUSOR_ON_OFF_BIT	1
#define COMMAND_BLINK_ON_OFF_BIT	0

void enable(void);
void write_data(uint8_t data);
void write_command(uint8_t command);
void clear(void);
void init(void);
void write_string(char *string);
void got_XY(uint8_t row, uint8_t col);

void enable()
{
	PORTD |=(1<<E_PIN);
	_delay_us(1);
	PORTD &=~(1<<E_PIN);
	_delay_ms(1);
}

void write_data(uint8_t data)
{
	uint8_t temp;
	PORTD |= (1<<RS_PIN);
	temp = PORTD & 0x0F;
	PORTD = temp |(data & 0xF0);
	enable();
	
	temp = PORTD & 0x0F;
	PORTD = temp | ((data <<4) & 0xF0);
	enable();
}

void write_command(uint8_t command)
{
	uint8_t temp;
	PORTD &= ~(1<<RS_PIN);
	temp = PORTD & 0x0F;
	PORTD = temp | (command & 0xF0);
	enable();
	
	temp = PORTD & 0x0F;
	PORTD = temp | ( (command << 4) & 0xF0);
	enable();
}
void clear()
{
	write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(1);
}
void init()
{
	_delay_ms(10);
	DDRD |= 0xFC;
	
	write_command(0x02);
	write_command(COMMAND_4_BIT_MODE);
	uint8_t command = 0x08 | ( 1 << COMMAND_DISPLAY_ON_OFF_BIT);
	write_command(command);
	clear();
	write_command(0x06);
}
void write_string(char *string)
{
	uint8_t i;
	for(i=0;string[i]; i++)
	write_data(string[i]);
}
void got_XY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	write_command(command);
}

//-----------------------------






void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0); //_BV는 저장한 비트마을 1로 하고 나머지를 0으로 하는 비트 마스크를 생성하는 메크로 //U2X0는 UCSR0A 레지스터의 1번 비트
	UBRR0H = 0x00; //USART Baud
	UBRR0L = 207;
	UCSR0C |= 0x06; //UART 통신에서의 데이터 형식 및 통신 방법을 결정하기 위해 사용
	UCSR0B |= _BV(RXEN0); //수신 가능 //UCSR0B 레지스터는 데이터 송수신을 가능하도록 설정하기 위해 사용
	UCSR0B |= _BV(TXEN0); //송신 가능
	UCSR0B |= _BV(RXCIE0); //RX Complete Interrupt Enable : 수신 완료 인터럽트 발생을 허용한다. 
}
unsigned char buffer[80] = {0,};
unsigned char wr = 0;
unsigned char lcd_state =0;
ISR(USART_RX_vect) //Interrupt Service Routine(UART Receive)
{
	while((UCSR0A & (1<<RXC0))) // UCSR0A (USART Control and Status Register 0 A) //7번 RXC0(Receive Complete) 비트는 데이터 수신 버퍼에 데이터가 도착하고 읽지 않은 상태인 경우 1로 설정되므로 데이터가 수신되었는지 여부를 판단하기 위해 사용할 수 있다.
		buffer[wr++] = UDR0;
	buffer[wr] = '\0';
	
	lcd_state = 1;
}

void UART_transmit()
{
	if(!wr)
		return;
	for(unsigned char i = 0; buffer[i]; i++){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = buffer[i];
	}
	wr = 0;
	//buffer[wr]= 0;
}




//---------------------------------------------




void Init_74595(void)
{
	DDRB |= 0b00011100;
}
void ShiftClock(void)
{
	PORTB |= 0b00000100; // 10번만 high
	_delay_us(1);
	PORTB &= 0b11111011; // 10번만 low로
}
void LatchClcok(void)
{
	PORTB |= 0b00010000;
	_delay_us(1);
	PORTB &= 0b11101111;
}

void ByteDataWrite(uint8_t data)
{
	for(uint8_t i = 0; i<8; i++){
		if(data & 0b10000000){ // data 8비트에서 최상위 비트 1비트만 내보내기 위해서 // 최상위 비트가 1이니깐
			PORTB |= 0b00001000; // 우리는 1비트만 쓰지만 portB는 8비트를 쓰므로 11번만 1일 떄 1로 보내고
		}
		else { // 최상위 비트가 0일 떄
			PORTB &= 0b11110111; //11번을 out으로 // 0일 때 0으로 보내는 거
		}
		ShiftClock(); // 10번이 low에서 high low로
		data = data << 1;
	}
	LatchClcok();
}



//-----------------------------------------------

ISR(TIMER0_OVF_vect)
{
	
}
int main()
{
	//unsigned char data;
	unsigned char  i;
	UART_INIT();
	init();
	sei();
	
	
	uint8_t numbers[] =
	{0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	
	DDRB = 0x20;
	
	int count = 0;
	Init_74595();
	
	
	
	
	while(1){
		//data = UART_receive();
		_delay_ms(50);
		UART_transmit();	
		if(lcd_state)
		{
			lcd_state = 0;
			clear();
			got_XY(0,0);
			for(i=0;i<16;i++){
				if(buffer[i] == 0)
					break;
				write_data(buffer[i]);
			}
			got_XY(1,0);
			for(;i<32;i++){
				if(buffer[i] == 0)
					break;
				write_data(buffer[i]);
			}
		}
		
		ByteDataWrite(numbers[count]);
		count = (count+1)%10;
		
		
		
		PORTB = 0x00;
		
		_delay_ms(1000);
	
		PORTB = 0x20;
		
		_delay_ms(1000);
	}
	return 0;
}
#endif





#if 1
//ISR UART FND LED
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//--------------------------------------------------

#define RS_PIN		2
#define E_PIN		3

#define COMMAND_CLEAR_DISPLAY	0x01
#define COMMAND_4_BIT_MODE		0x28

#define COMMAND_DISPLAY_ON_OFF_BIT	2
#define COMMAND_CUSOR_ON_OFF_BIT	1
#define COMMAND_BLINK_ON_OFF_BIT	0

void enable(void);
void write_data(uint8_t data);
void write_command(uint8_t command);
void clear(void);
void init(void);
void write_string(char *string);
void got_XY(uint8_t row, uint8_t col);

void enable()
{
	PORTD |=(1<<E_PIN);
	_delay_us(1);
	PORTD &=~(1<<E_PIN);
	_delay_ms(1);
}

void write_data(uint8_t data)
{
	uint8_t temp;
	PORTD |= (1<<RS_PIN);
	temp = PORTD & 0x0F;
	PORTD = temp |(data & 0xF0);
	enable();
	
	temp = PORTD & 0x0F;
	PORTD = temp | ((data <<4) & 0xF0);
	enable();
}

void write_command(uint8_t command)
{
	uint8_t temp;
	PORTD &= ~(1<<RS_PIN);
	temp = PORTD & 0x0F;
	PORTD = temp | (command & 0xF0);
	enable();
	
	temp = PORTD & 0x0F;
	PORTD = temp | ( (command << 4) & 0xF0);
	enable();
}
void clear()
{
	write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(1);
}
void init()
{
	_delay_ms(10);
	DDRD |= 0xFC;
	
	write_command(0x02);
	write_command(COMMAND_4_BIT_MODE);
	uint8_t command = 0x08 | ( 1 << COMMAND_DISPLAY_ON_OFF_BIT);
	write_command(command);
	clear();
	write_command(0x06);
}
void write_string(char *string)
{
	uint8_t i;
	for(i=0;string[i]; i++)
	write_data(string[i]);
}
void got_XY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	write_command(command);
}

//-----------------------------






void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0); //_BV는 저장한 비트마을 1로 하고 나머지를 0으로 하는 비트 마스크를 생성하는 메크로 //U2X0는 UCSR0A 레지스터의 1번 비트
	UBRR0H = 0x00; //USART Baud
	UBRR0L = 207;
	UCSR0C |= 0x06; //UART 통신에서의 데이터 형식 및 통신 방법을 결정하기 위해 사용
	UCSR0B |= _BV(RXEN0); //수신 가능 //UCSR0B 레지스터는 데이터 송수신을 가능하도록 설정하기 위해 사용
	UCSR0B |= _BV(TXEN0); //송신 가능
	UCSR0B |= _BV(RXCIE0); //RX Complete Interrupt Enable : 수신 완료 인터럽트 발생을 허용한다.
}
unsigned char buffer[80] = {0,};
unsigned char wr = 0;
unsigned char lcd_state =0;
ISR(USART_RX_vect) //Interrupt Service Routine(UART Receive)
{
	while((UCSR0A & (1<<RXC0))) // UCSR0A (USART Control and Status Register 0 A) //7번 RXC0(Receive Complete) 비트는 데이터 수신 버퍼에 데이터가 도착하고 읽지 않은 상태인 경우 1로 설정되므로 데이터가 수신되었는지 여부를 판단하기 위해 사용할 수 있다.
	buffer[wr++] = UDR0;
	buffer[wr] = '\0';
	
	lcd_state = 1;
}

void UART_transmit()
{
	if(!wr)
	return;
	for(unsigned char i = 0; buffer[i]; i++){
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = buffer[i];
	}
	wr = 0;
	//buffer[wr]= 0;
}




//---------------------------------------------




void Init_74595(void)
{
	DDRB |= 0b00011100;
}
void ShiftClock(void)
{
	PORTB |= 0b00000100; // 10번만 high
	_delay_us(1);
	PORTB &= 0b11111011; // 10번만 low로
}
void LatchClcok(void)
{
	PORTB |= 0b00010000;
	_delay_us(1);
	PORTB &= 0b11101111;
}

void ByteDataWrite(uint8_t data)
{
	for(uint8_t i = 0; i<8; i++){
		if(data & 0b10000000){ // data 8비트에서 최상위 비트 1비트만 내보내기 위해서 // 최상위 비트가 1이니깐
			PORTB |= 0b00001000; // 우리는 1비트만 쓰지만 portB는 8비트를 쓰므로 11번만 1일 떄 1로 보내고
		}
		else { // 최상위 비트가 0일 떄
			PORTB &= 0b11110111; //11번을 out으로 // 0일 때 0으로 보내는 거
		}
		ShiftClock(); // 10번이 low에서 high low로
		data = data << 1;
	}
	LatchClcok();
}



//-----------------------------------------------
int countLed = 0;
int state = 0;
ISR(TIMER0_OVF_vect)
{
	countLed++;
	if(countLed == 32) {
		countLed = 0;
		state = !state;
		if(state) PORTB = 0x20;
		else PORTB = 0x00;
	}
}
int main()
{
	//unsigned char data;
	unsigned char  i;
	UART_INIT();
	init();
	
	sei();
	
	uint8_t numbers[] =
	{0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	
	DDRB = 0x20;
	
	PORTB = 0x00;
	int count = 0;
	Init_74595();
	
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	TIMSK0 |= ( 1 << TOIE0);
	

	while(1){
		//data = UART_receive();
	
		UART_transmit();
		if(lcd_state)
		{
			lcd_state = 0;
			clear();
			got_XY(0,0);
			for(i=0;i<16;i++){
				if(buffer[i] == 0)
				break;
				write_data(buffer[i]);
			}
			got_XY(1,0);
			for(;i<32;i++){
				if(buffer[i] == 0)
				break;
				write_data(buffer[i]);
			}
		}
		
		//ByteDataWrite(numbers[count]);
	//	count = (count+1)%10;
		
	//	_delay_ms(500);
		
	}
	return 0;
}
#endif