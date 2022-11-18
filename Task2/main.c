/*
 * Task2.c
 *
 * Created: 17/11/2022 14:37:53
 * Author : Eleanor
 */ 

// Declare LCD pins
#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB3
#define EN eS_PORTB4

#define LCDPINS_DDRD 0xff
#define LCDPINS_DDRB 0xff
/*
#define LCDPINS_DDRD 0b11110000
#define LCDPINS_DDRB 0b00011000
*/
#include <avr/io.h>
#include "lcd.h"

#define BAUDRATE 9600
// System clock speed
#define FOSC 16000000UL
#define BAUDRATE_PRESCALE ( (FOSC / (BAUDRATE * 16UL)) - 1)

// Send a character over USART
void send_USART(unsigned char data)
{
	// Wait for existing data to be send
	while (! ( UCSR0A & (1 << UDRE0) ) )
	{
		// pass;
	}
	// Load data into register to transmit
	UDR0 = data;
}

unsigned char blocking_recieve_USART()
{
	// Wait for indicator of received data
	while (! ( UCSR0A & (1 << RXC0) ) )
	{
		// pass;
	}
	// Return received character
	return UDR0;
}

void init_USART()
{
	// Set USART Baudrate
	UBRR0H = (uint8_t) (BAUDRATE_PRESCALE>>8);
	UBRR0L = (uint8_t) (BAUDRATE_PRESCALE);
	// Enable Rx and Tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Frame format 8 bit data, no parity, 1 stop bit
	UCSR0C = (3 << UCSZ00) | (0 << UPM00) | (0 << USBS0);
}

// Set up input, outputs and initial program state.
void init()
{
	// Initialize Serial
	init_USART();
	// Set output pins
	DDRD = LCDPINS_DDRD;
	DDRB = LCDPINS_DDRB;
	// Initialize and clear LCD
	Lcd4_Init();
	Lcd4_Set_Cursor(1,0);
}

int main(void)
{
	init();
    while (1) 
    {
		// Wait for a new character received over serial
		unsigned char r = blocking_recieve_USART();
		switch (r)
		{
			case ';': // Special character ';' used to signal an LCD clearing event
				Lcd4_Clear();
				break;
			default:
				Lcd4_Write_Char(r);
				break;
		}
    }
}

