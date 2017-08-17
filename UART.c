/*
 * UART.c
 *
 * Created: 8/16/2017 1:53:22 PM
 *  Author: Karim
 */ 
#include "UART.h"

void uart_init(void)
{
	//word operation, interrupts must be disable during writing.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UBRR0 = UBRR_VALUE;
	}
	#if MODE == 0 
		UCSR0A  |= 1 << U2X0; //enable double speed mode
	#elif MODE == 2
		UCSR0A |= 1 << MPCM0; //enable Multi-processor Communication Mode
	#endif
				
	#if TX_EN == 1
	UCSR0B |= 1 << TXEN0; //enable TX
	#endif
	
	#if RX_EN == 1
	UCSR0B |= 1 << RXEN0; //enable RX
	#endif
	
	#if FRAME_SIZE == 1
	UCSR0B |= 1 << UCSZ02; //enable 9th bit
	#endif
	
	#if PARITY == 1
	UCSR0C |= 1 << UPM01 & ~(1<<UPM00); //enable even parity
	#endif
	
	#if PARITY == 2
	UCSR0C |= 1 << UPM01 | 1<< UPM00;	//enable odd parity
	#endif
	
	#if SBIT == 1	//2 stop bits
	UCSR0C |= 1 << USBS0;
	#endif
}

uint8_t uart_send(uint8_t data)
{
	#if FRAME_SIZE || TX_EN == 0
		return -1;
	#endif
	
	//Wait for buffer to be empty
	while (!(UCSR0A & (1<<UDRE0)));
	
	#if FRAME_SIZE || TX_EN == 0
		return -1;
	#endif
	
	UDR0 = data;
	return 0;
}

uint8_t uart_send9(uint16_t data)
{
	#if FRAME_SIZE == 0 || TX_EN == 0
		return -1;
	#endif
	
	//Wait for buffer to be empty
	while (!(UCSR0A & (1<<UDRE0)));

	UCSR0B |= ((data & (1 << 8)) >> 8) << TXB80;
	UDR0 = (uint8_t)data;
	return 0;
}

uint8_t uart_recieve(void)
{	
	#if FRAME_SIZE || RX_EN == 0
		return -1;
	#endif
	while (!(UCSR0A & (1<<RXC0)));
	//returns -1 if there is an error in current frame.
	if ((UCSR0A & 1 << FE0) || (UCSR0A & 1 << UPE0))
	{
		return -1;
	}
	return UDR0;
}

uint16_t uart_recieve9(void)
{
	#if FRAME_SIZE == 0 || RX_EN == 0
		return -1;
	#endif
	while (!(UCSR0A & (1<<RXC0)));
	//returns -1 if there is an error in current frame.
	if ((UCSR0A & 1 << FE0) || (UCSR0A & 1 << UPE0))
	{
		return -1;
	}
	uint8_t x = ((UCSR0B & 1 << RXB80) >> RXB80); //must read the 9th bit first
	return (x << 8) | UDR0;
}