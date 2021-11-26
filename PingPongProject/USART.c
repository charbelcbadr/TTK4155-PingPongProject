/*
 * PingPongProject.c
 *
 * Created: 01.09.2021 12:30:11
 * Author : charbelb
 */ 

#define F_CPU 4.9152e6
#define BAUD 9600
#define MYUBRR  F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>


/* 
Description: Function for USART transmit (the reference for the source code is from Atmega162) 
Input:  Input of the data
Output: Nothing
*/
int USART_Transmit(char data, FILE *stream)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}
/* 
Description: Function for USART recieve  (the reference for the source code is from Atmega162) 
Input:  Input of the data
Output: Returns the data received from buffer
*/
int USART_Recieve(FILE *stream)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	char data = UDR0;
	return data;
}
/* 
Description: Initialisation of UART  (the reference for the source code is from Atmega162) 
Input:  Input of the ubrr is derived from the Fosc and Baudrate
Output: Nothing
*/
void USART_Init (unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H= (ubrr>>8);
	UBRR0L=  ubrr;
	UCSR0C= (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	/* Enable receiver and transmitter */
	UCSR0B= (1<<RXEN0)|(1<<TXEN0);
	fdevopen(*USART_Transmit, *USART_Recieve);
}




