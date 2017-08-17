/*
 * UART.h
 *
 * Created: 8/16/2017 1:53:34 PM
 *  Author: Karim
 */ 


/*
setbaud.h requires F_CPU and BAUD macros to be defined. This library supports 8 and 9 bits frames.
*/

#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#define BAUD 9600

#include <util/atomic.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/setbaud.h>


//enable transmission = 1, 0 otherwise
#define TX_EN 1

//enable reception = 1, 0 otherwise
#define RX_EN 1

//UART normal mode = 1, UART double speed mode = 0, Multiprocessor mode = 2
#define MODE 1

//UART even parity = 1, 2 for odd parity, 0 is disabled
#define PARITY 1

//frame stop bit is a single bit = 0, couple of bit = 1
#define SBIT 1

//frame size. 0 = byte, 1 = 9 bit
#define FRAME_SIZE 0

void uart_init(void);
//Next two functions returns -1 if there is mismatch during initialization, 0 otherwise
uint8_t uart_send(uint8_t data); 
uint8_t uart_send9(uint16_t data);
//returns -1 if an error occurred in the current frame. UDR0 register otherwise.
uint8_t uart_recieve(void);
uint16_t uart_recieve9(void);


#endif /* UART_H_ */